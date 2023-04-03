#include "keymanager.h"
#include "ui_keymanager.h"
#include "ui_mainwindow.h"
#include <QDialog>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>
#include <QFile>
#include <QCoreApplication>
#include <QItemDelegate>
#include <iostream>
#include <QClipboard>
#include <QCryptographicHash>


keymanager::keymanager(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::keymanager)
{
    ui->setupUi(this);
}

//функция хэширования
std::string keymanager::converthash_key(){
    QString pin = "1234";
    QByteArray hash = QCryptographicHash::hash
            (pin.toUtf8(),
             QCryptographicHash::Sha256);
    hash = hash.toBase64();
    QString m = QString((char *)hash.data());
    std::string str_hash = m.toStdString();
    return str_hash;
}


void keymanager::encr_log_pass_and_file(){
    QFile jsonFile("C:/Users/SIXTECH/Desktop/Development/lab1/cridentials.json");
    if (!jsonFile.open(QIODevice::ReadOnly ))
    {
        return;
    }
    QByteArray saveData = jsonFile.readAll();
    QJsonDocument jsonDocument(QJsonDocument::fromJson(saveData));

    std::string key_pin = converthash_key();

    PasswordManager = jsonDocument.object();
    PasswordManagerArray = PasswordManager["cridentials"].toArray();

    QJsonArray Cridentials;

    for (const QJsonValue & str : PasswordManagerArray)
    {
        if (str.isObject())
        {
            QJsonObject obj = str.toObject();
            QString site = obj["site"].toString();
            QString login = obj["login"].toString();
            QString password = obj["password"].toString();

            std::string login_new = login.toUtf8().constData();
            std::string password_new = password.toUtf8().constData();

            std::vector<std::string> crid_new;
            crid_new.push_back("\"login\":\"");
            crid_new.push_back(login_new);
            crid_new.push_back("\",\"password\":\"");
            crid_new.push_back(password_new);
            crid_new.push_back("\"");

            std::stringstream str;
            for (auto it = crid_new.begin(); it != crid_new.end(); it++)    {
                if (it != crid_new.begin()) {
                    str << "";
                }
                str << *it;
            }
            std::string str_crid_new = str.str();
            std::string encr_crid_new = aes_json->encrypt_obj(key_pin, str_crid_new);
            QString encr_qstr_crid_new = QString::fromStdString(encr_crid_new);

            QJsonObject PasswordManagerObject;

            PasswordManagerObject.insert("site", site);
            PasswordManagerObject.insert("crid_new", encr_qstr_crid_new);

            Cridentials.append(PasswordManagerObject);
            }
    }

    //На данном этапе у нас зашифрованы логины и пароли

    QJsonObject new_log_pass_json;
    new_log_pass_json.insert("cridentials", QJsonValue(Cridentials));

    QJsonDocument doc(new_log_pass_json);

    QString new_qstring_log_pass_json(doc.toJson(QJsonDocument::Compact));


    //шифруем весь файл

    std::string new_string_log_pass_json = new_qstring_log_pass_json.toUtf8().constData();
    aes_json->encrypt_file(key_pin, new_string_log_pass_json);

}

void keymanager::open_pass(){

    //вызываем функцию шифрования логинов и паролей
    encr_log_pass_and_file();
    QFile file("cridentials_new.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);

//    избавляемся от кодировки Base64
    QByteArray buffer = QByteArray::fromBase64(file.readAll());
    std::vector<uint8_t> data(buffer.begin(), buffer.end());

    // получаем хэш ключа
    std::string key_pin = converthash_key();
    //расшифровываем содержимое файла в переменную for_parsing
    std::string for_parsing = aes_json->decrypt(key_pin, data);
    qDebug() << QString::fromStdString(for_parsing);


    //конвертация в QString
    QString parsed_json = QString::fromUtf8(for_parsing.c_str());

    QJsonParseError error;
    doc = QJsonDocument::fromJson(parsed_json.toUtf8(), &error);
    PasswordManager = doc.object();
    PasswordManagerArray = PasswordManager["cridentials"].toArray();

}


void keymanager::createUI(const QStringList &headers){
    ui->pass_manager->setColumnCount(4);
    ui->pass_manager->setShowGrid(true);
    ui->pass_manager->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->pass_manager->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->pass_manager->setHorizontalHeaderLabels(headers);
    ui->pass_manager->horizontalHeader()->setStretchLastSection(true);
    ui->pass_manager->hideColumn(0);
    ui->pass_manager->setEditTriggers(QAbstractItemView::AnyKeyPressed);
    QString search_param = "";
    cridentials(search_param);
}

QJsonArray keymanager::decrypted_crid(QLineEdit* lineEdit) {

    QString crids = lineEdit->text();

    std::string crids_string = crids.toStdString();
    const char *crids_char = crids_string.c_str();

    QByteArray buffer = QByteArray::fromBase64(crids_char);
    std::vector<uint8_t> data(buffer.begin(), buffer.end());

    std::string key_pin = converthash_key();

    std::string crids_decrypt = aes_json->decrypt(key_pin, data);

    QString crids_decrypt2 = QString::fromStdString(crids_decrypt);

    std::string crids_decrypt_str = crids_decrypt2.toStdString();

    std::vector<std::string> crid_new2;
    crid_new2.push_back("{\"crids\":[{");
    crid_new2.push_back(crids_decrypt_str);
    crid_new2.push_back("}]}");

    std::stringstream str;
    for (auto it = crid_new2.begin(); it != crid_new2.end(); it++)    {
        if (it != crid_new2.begin()) {
            str << "";
        }
        str << *it;
    }
    std::string str_crid_new2 = str.str();
    QString qstr_str_crid_new2 = QString::fromStdString(str_crid_new2);

    QJsonParseError error;
    doc2 = QJsonDocument::fromJson(qstr_str_crid_new2.toUtf8(), &error);
    PasswordManager2 = doc2.object();
    PasswordManagerArray2 = PasswordManager2["crids"].toArray();
    return PasswordManagerArray2;
}

bool keymanager::eventFilter(QObject* watched, QEvent* event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        if(QLineEdit* lineEdit = qobject_cast<QLineEdit*>(watched))
        {
            Q_FOREACH(const QLineEdit* i, findChildren<QLineEdit*>())
            i->disconnect();

            // для понимания, что клик на нужную ячуйку(логин или пароль)
            int column = lineEdit->property("column").toInt();

            if (column != 1)
            {
                QJsonArray PasswordManagerArray = decrypted_crid(lineEdit);

                QClipboard *click = QGuiApplication::clipboard();

                if (column == 2) {
                    for (const QJsonValue & str : PasswordManagerArray)
                    {
                         if (str.isObject())
                         {
                             QJsonObject obj = str.toObject();
                             QString login = obj["login"].toString();
                             click->setText(login);
                         }
                     }
                }
                else if (column == 3) {
                    for (const QJsonValue & str : PasswordManagerArray)
                    {
                         if (str.isObject())
                         {
                             QJsonObject obj = str.toObject();
                             QString password = obj["password"].toString();
                             click->setText(password);
                          }
                     }
                }
            }
        }
    }
    return QObject::eventFilter(watched, event);
}


void keymanager::cridentials(QString search_param) {
        ui->pass_manager->setRowCount(0); //очищаем поле поиска
        QString array;
        int i = 0;
        for (const QJsonValue & str : PasswordManagerArray)
        {
            if (str.isObject())
            {
                QJsonObject obj = str.toObject();

                QString site = obj["site"].toString();
                QString crids = obj["crid_new"].toString();

                QString search = search_param;
                std::string string_search = search.toUtf8().constData();
                std::string string_site = site.toUtf8().constData();

                int pos = string_site.find(string_search);

                if (pos > -1) {
                    ui->pass_manager->insertRow(i);

                    QLineEdit *lineEdit_site = new QLineEdit(site);
                    QLineEdit *lineEdit_login = new QLineEdit(crids);
                    QLineEdit *lineEdit_password = new QLineEdit(crids);

                    lineEdit_site->setProperty("column", 1);
                    lineEdit_site->setReadOnly(true);
                    lineEdit_site->installEventFilter(this);

                    lineEdit_login->setProperty("column", 2);
                    lineEdit_login->setReadOnly(true);
                    lineEdit_login->installEventFilter(this);

                    lineEdit_password->setProperty("column", 3);
                    lineEdit_password->setReadOnly(true);
                    lineEdit_password->installEventFilter(this);

                    lineEdit_login->setEchoMode(QLineEdit::Password);
                    lineEdit_password->setEchoMode(QLineEdit::Password);

                    ui->pass_manager->setCellWidget(i, 1, lineEdit_site);
                    ui->pass_manager->setCellWidget(i, 2, lineEdit_login);
                    ui->pass_manager->setCellWidget(i, 3, lineEdit_password);
                    i++;
                }
           }
     }
}

void keymanager::on_search_buttom_clicked()
{
    QString search_param = ui->search->text();
    qDebug() << search_param;
    cridentials(search_param);
}


keymanager::~keymanager()
{
    delete ui;
}


