#include "cryptography.h"


cryptography::cryptography(std::vector<uint8_t> iv)
    : m_iv(move(iv)) {
}

//обработчик ошибок
static void throw_if_error(int res = 1, const char* file = nullptr, uint64_t line = 0) {
    unsigned long errc = ERR_get_error();
    if (res <= 0 || errc != 0) {
        if (errc == 0) {
            return;
        }
        std::vector<std::string> errors;
        while (errc != 0) {
            std::vector<uint8_t> buf(256);
            ERR_error_string(errc, (char*) buf.data());
            errors.push_back(std::string(buf.begin(), buf.end()));
            errc = ERR_get_error();
        }
        std::stringstream ss;
        ss << "\n";
        for (auto&& err : errors) {
            if (file != nullptr) {
                ss << file << ":" << (line - 1) << " ";
            }
            ss << err << "\n";
        }
        const std::string err_all = ss.str();
        throw openssl_error(errc, err_all);
    }
}


//функция шифрования
void cryptography::encrypt(const std::vector<uint8_t>& key, const std::vector<uint8_t>& message, std::vector<uint8_t>& output) const {
    output.resize(message.size() * AES_BLOCK_SIZE);
    int inlen = message.size();
    int outlen = 0;
    size_t total_out = 0;

    EVP_CIPHER_CTX_t ctx(EVP_CIPHER_CTX_new());
    throw_if_error(1, __FILE__, __LINE__);

    const std::vector<uint8_t> enc_key = key;

    int res;
    res = EVP_EncryptInit(ctx.get(), EVP_aes_256_cbc(), enc_key.data(), m_iv.data());
    throw_if_error(res, __FILE__, __LINE__);
    res = EVP_EncryptUpdate(ctx.get(), output.data(), &outlen, message.data(), inlen);
    throw_if_error(res, __FILE__, __LINE__);
    total_out += outlen;
    res = EVP_EncryptFinal(ctx.get(), output.data()+total_out, &outlen);
    throw_if_error(res, __FILE__, __LINE__);
    total_out += outlen;

    output.resize(total_out);
}

//функция расшифрования
void cryptography::decrypt(const std::vector<uint8_t>& key, const std::vector<uint8_t>& message, std::vector<uint8_t>& output) const{
    output.resize(message.size() * 3);
    int outlen = 0;
    size_t total_out = 0;
    EVP_CIPHER_CTX_t ctx(EVP_CIPHER_CTX_new());
    throw_if_error();
    const std::vector<uint8_t> enc_key = key;
    std::vector<uint8_t> target_message;
    std::vector<uint8_t> iv;
    iv = m_iv;
    target_message = message;
    int inlen = target_message.size();
    int res;
    res = EVP_DecryptInit(ctx.get(), EVP_aes_256_cbc(), enc_key.data(), iv.data());
    throw_if_error(res, __FILE__, __LINE__);
    res = EVP_DecryptUpdate(ctx.get(), output.data(), &outlen, target_message.data(), inlen);
    throw_if_error(res, __FILE__, __LINE__);
    total_out += outlen;
    res = EVP_DecryptFinal(ctx.get(), output.data()+outlen, &outlen);
    throw_if_error(res, __FILE__, __LINE__);
    total_out += outlen;
    output.resize(total_out);

}

//функция преобразования до шифрования или расшифрования, чтобы можно было корректно зашифровать
static std::vector<uint8_t> str_to_bytes(const std::string& message) {
    std::vector<uint8_t> out(message.size());
    for(size_t n = 0; n < message.size(); n++) {
        out[n] = message[n];
    }
    return out;
}

// перевод кодировки в base64
static QByteArray bytes_to_base64(const std::vector<uint8_t>& bytes) {
    QByteArray *arr = new QByteArray(reinterpret_cast<const char*>(bytes.data()), bytes.size());
    QByteArray array_base64 = arr->toBase64();
    return array_base64;
}

//функция преобразования после шифрования или расшифрования в строку, чтобы можно было корректно отображать
static std::string bytes_to_str(const std::vector<uint8_t>& bytes) {
    return std::string(bytes.begin(), bytes.end());
}

// для шифрования логинов и паролей
std::string cryptography::encrypt_obj(std::string key_pin, std::string message_json) {
    const std::string iv = "1838187512345678";
    const std::string message = message_json;
    const std::string key = key_pin.substr(0,31);
    const cryptography encryptor(str_to_bytes(iv));
    std::vector<uint8_t> enc_result;
    encryptor.encrypt(str_to_bytes(key), str_to_bytes(message), enc_result);
    QByteArray result = bytes_to_base64(enc_result);
    QString q_enc_result = QString(result);
    std::string str_enc_result;
    str_enc_result = q_enc_result.toStdString();
    return str_enc_result;
}

//функция шифрования файла
void cryptography::encrypt_file(std::string key_pin, std::string message_json){
    const std::string iv = "1838187512345678";
    const std::string message = message_json;
    const std::string key = key_pin.substr(0,31);
    const cryptography encryptor(str_to_bytes(iv));
    std::vector<uint8_t> enc_result;
    encryptor.encrypt(str_to_bytes(key), str_to_bytes(message), enc_result);
    QByteArray result = bytes_to_base64(enc_result);
    QString q_enc_result = QString(result);
    std::string str_enc_result;
    str_enc_result = q_enc_result.toStdString();
    std::ofstream enc_file("cridentials_new.json", std::ios::out);
    enc_file.write(reinterpret_cast<const char*>(str_enc_result.data()), str_enc_result.size());
    enc_file.close();

}

//функция расшифрования файла
std::string cryptography::decrypt(std::string key_pin, std::vector<uint8_t> enc_result){
    const std::string iv = "1838187512345678";
    const std::string key = key_pin.substr(0,31);
    const cryptography encryptor(str_to_bytes(iv));
    std::vector<uint8_t> dec_result;
    encryptor.decrypt(str_to_bytes(key), enc_result, dec_result);
    return bytes_to_str(dec_result);
}
