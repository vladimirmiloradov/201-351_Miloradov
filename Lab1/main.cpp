#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <windows.h>
#include <QMessageBox>
#include <QProcess>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //определяем, где в виртуальной памяти начало сегмента .text
    unsigned long long modulBase = (unsigned long long)GetModuleHandle(NULL);// определяем смещение в виртуальной где расположен сегмент
    unsigned long long text_segment_start = modulBase + 0x1000; //адрес сегмента .text
    // определяем какой он длины(парсим заголовки)
    PIMAGE_DOS_HEADER pIDH = reinterpret_cast<PIMAGE_DOS_HEADER>(modulBase);
    PIMAGE_NT_HEADERS pINH = reinterpret_cast<PIMAGE_NT_HEADERS>(modulBase+ pIDH->e_lfanew);
    unsigned long long size_of_text = pINH->OptionalHeader.SizeOfCode; //размер сегмента .text
    //выгружаем содержимое контейнера в QByteArray
    QByteArray text_segment_contents = QByteArray((char*)text_segment_start, size_of_text);
    //считаем хеш
    QByteArray current_hash = QCryptographicHash::hash(text_segment_contents, QCryptographicHash::Sha256);
    QByteArray current_hash_base64 = current_hash.toBase64();

    qDebug() << "current_hash_base64 = " << current_hash_base64;
    //сравниваем полученный хеш с эталонным
    const QByteArray hash0_base64 = QByteArray("JaUbiE0ebOPr6fIrnBtOA0slt7KwOm3gI2got2EZ/Xs=");

    if (current_hash_base64!=hash0_base64)
    {
        QMessageBox::critical(nullptr, "Внимание!", "Приложение модифицированно!");
    }
    else
    {
        w.show();
        return a.exec();
    }
}


