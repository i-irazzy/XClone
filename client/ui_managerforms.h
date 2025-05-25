/********************************************************************************
** Form generated from reading UI file 'managerforms.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MANAGERFORMS_H
#define UI_MANAGERFORMS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ManagerForms
{
public:
    QMenuBar *menubar;
    QWidget *centralwidget;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ManagerForms)
    {
        if (ManagerForms->objectName().isEmpty())
            ManagerForms->setObjectName("ManagerForms");
        ManagerForms->resize(800, 600);
        menubar = new QMenuBar(ManagerForms);
        menubar->setObjectName("menubar");
        ManagerForms->setMenuBar(menubar);
        centralwidget = new QWidget(ManagerForms);
        centralwidget->setObjectName("centralwidget");
        ManagerForms->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(ManagerForms);
        statusbar->setObjectName("statusbar");
        ManagerForms->setStatusBar(statusbar);

        retranslateUi(ManagerForms);

        QMetaObject::connectSlotsByName(ManagerForms);
    } // setupUi

    void retranslateUi(QMainWindow *ManagerForms)
    {
        ManagerForms->setWindowTitle(QCoreApplication::translate("ManagerForms", "MainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ManagerForms: public Ui_ManagerForms {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MANAGERFORMS_H
