/********************************************************************************
** Form generated from reading UI file 'keymanager.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KEYMANAGER_H
#define UI_KEYMANAGER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_keymanager
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLineEdit *search;
    QPushButton *search_buttom;
    QTableWidget *pass_manager;
    QGridLayout *gridLayout_2;

    void setupUi(QDialog *keymanager)
    {
        if (keymanager->objectName().isEmpty())
            keymanager->setObjectName("keymanager");
        keymanager->resize(554, 344);
        gridLayout = new QGridLayout(keymanager);
        gridLayout->setObjectName("gridLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        search = new QLineEdit(keymanager);
        search->setObjectName("search");

        horizontalLayout->addWidget(search);

        search_buttom = new QPushButton(keymanager);
        search_buttom->setObjectName("search_buttom");
        search_buttom->setMinimumSize(QSize(0, 0));
        search_buttom->setMouseTracking(true);

        horizontalLayout->addWidget(search_buttom);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        pass_manager = new QTableWidget(keymanager);
        pass_manager->setObjectName("pass_manager");

        gridLayout->addWidget(pass_manager, 2, 0, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName("gridLayout_2");

        gridLayout->addLayout(gridLayout_2, 1, 0, 1, 1);


        retranslateUi(keymanager);

        QMetaObject::connectSlotsByName(keymanager);
    } // setupUi

    void retranslateUi(QDialog *keymanager)
    {
        keymanager->setWindowTitle(QCoreApplication::translate("keymanager", "Dialog", nullptr));
        search_buttom->setText(QCoreApplication::translate("keymanager", "\320\237\320\276\320\270\321\201\320\272 \321\201\320\260\320\271\321\202\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class keymanager: public Ui_keymanager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KEYMANAGER_H
