#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QJsonDocument>
#include <windows.h>
#include <QProcess>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pass = new keymanager;
    //коннектор для перехода в окно
    connect(pass, &keymanager::ShowMW,this,&MainWindow::show);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_Ok_clicked()
{
    // В переменную код считываем введенный пин код, если он верный то открываем файл
    QString kod;
    kod = ui->code_text->text();
    if (kod == "1234")
    {
        this->close();
        pass->open_pass();
        pass->show();
        pass->createUI(QStringList()
                        << tr("Id")
                        << tr("Site")
                        << tr("Login")
                        << tr("Password")
      );

    }
    else if (kod != "1234")
    {
        QMessageBox::critical(this,"Авторизация","Неправильный пин-код!");
    }
}
