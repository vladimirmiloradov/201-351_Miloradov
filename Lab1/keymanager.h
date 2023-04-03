#ifndef KEYMANAGER_H
#define KEYMANAGER_H
#include <QDialog>
#include <QTableWidgetItem>
#include <QLineEdit>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include "cryptography.h"

QT_BEGIN_NAMESPACE
namespace Ui { class keymanager; }
QT_END_NAMESPACE

class keymanagerData;

class keymanager : public QDialog
{
    Q_OBJECT

public:
    keymanager(QWidget *parent = nullptr);
    ~keymanager();
    void open_pass();
    void createUI(const QStringList &headers);
    std::string converthash_key();



private:
    Ui::keymanager *ui;
    cryptography *aes_json;
    void cridentials(QString search_param);
    void encr_log_pass_and_file();
    QJsonArray decrypted_crid(QLineEdit* lineEdit);
    bool eventFilter(QObject* watched, QEvent* event);
    QJsonDocument doc;
    QJsonObject PasswordManager;
    QJsonArray PasswordManagerArray;
    QJsonDocument doc2;
    QJsonObject PasswordManager2;
    QJsonArray PasswordManagerArray2;

signals:
    void ShowMW();

private slots:
    void on_search_buttom_clicked();
};

#endif // KEYMANAGER_H

