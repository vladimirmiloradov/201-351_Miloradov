#ifndef CRYPTOGRAPHY_H
#define CRYPTOGRAPHY_H

#include <openssl/aes.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/evperr.h>
#include <openssl/crypto.h>
#include <QCryptographicHash>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <cstring>
#include <fstream>
#include <conio.h>
#include <QString>
#include <QDebug>

//using namespace std;

#define DECL_OPENSSL_PTR(tname, free_func) \
    struct openssl_##tname##_dtor {            \
        void operator()(tname* v) {        \
            free_func(v);              \
        }                              \
    };                                 \
    typedef std::unique_ptr<tname, openssl_##tname##_dtor> tname##_t

DECL_OPENSSL_PTR(EVP_CIPHER_CTX, EVP_CIPHER_CTX_free);

struct error : public std::exception {
private:

public:
    std::string m_msg;

    error(const std::string& message)
       : m_msg(message) {
    }

    error(const char* msg)
        : m_msg(msg, msg + strlen(msg)) {
    }

    virtual const char* what() const noexcept override {
        return m_msg.c_str();
    }
};

struct openssl_error: public virtual error {
private:


public:
    int m_code = -1;
    std::string m_msg;
    openssl_error(int code, const std::string& message)
        : error(message),
          m_code(code) {
        std::stringstream ss;
        ss << "[" << m_code << "]: " << message;
        m_msg = ss.str();
    }

    openssl_error(int code, const char* msg)
        : error(msg),
          m_code(code) {
        std::stringstream ss;
        ss << "[" << m_code << "]: " << msg;
        m_msg = ss.str();
    }

    const char* what() const noexcept override {
        return m_msg.c_str();
    }
};


class aes
{
public:
    aes(std::vector<uint8_t> iv);

    std::vector<uint8_t> m_iv;
    void encrypt(const std::vector<uint8_t>& key, const std::vector<uint8_t>& message, std::vector<uint8_t>& output) const;
    void decrypt(const std::vector<uint8_t>& key, const std::vector<uint8_t>& message, std::vector<uint8_t>& output) const;
    void encrypt_file(std::string key_pin, std::string message_json);
    std::string encrypt_obj(std::string key_pin, std::string message_json);
    std::string decrypt(std::string key_pin, std::vector<uint8_t> enc_result);

};

#endif // CRYPTOGRAPHY_H
