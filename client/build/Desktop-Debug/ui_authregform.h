/********************************************************************************
** Form generated from reading UI file 'authregform.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUTHREGFORM_H
#define UI_AUTHREGFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AuthForm
{
public:
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonLogIn;
    QPushButton *pushButtonSignUp;
    QPushButton *pushButtonToReg;
    QWidget *formLayoutWidget;
    QGridLayout *gridLayout;
    QLineEdit *lineEditEmail;
    QLineEdit *lineEdit_PasswordCheck;
    QLabel *labelPassword_Check;
    QLabel *labelLogin;
    QLabel *labelEmail;
    QLabel *labelPassword;
    QLineEdit *lineEditLogin;
    QLineEdit *lineEditPassword;

    void setupUi(QDialog *AuthForm)
    {
        if (AuthForm->objectName().isEmpty())
            AuthForm->setObjectName("AuthForm");
        AuthForm->resize(400, 300);
        horizontalLayoutWidget = new QWidget(AuthForm);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(70, 200, 259, 80));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButtonLogIn = new QPushButton(horizontalLayoutWidget);
        pushButtonLogIn->setObjectName("pushButtonLogIn");

        horizontalLayout->addWidget(pushButtonLogIn);

        pushButtonSignUp = new QPushButton(horizontalLayoutWidget);
        pushButtonSignUp->setObjectName("pushButtonSignUp");

        horizontalLayout->addWidget(pushButtonSignUp);

        pushButtonToReg = new QPushButton(horizontalLayoutWidget);
        pushButtonToReg->setObjectName("pushButtonToReg");

        horizontalLayout->addWidget(pushButtonToReg);

        formLayoutWidget = new QWidget(AuthForm);
        formLayoutWidget->setObjectName("formLayoutWidget");
        formLayoutWidget->setGeometry(QRect(30, 20, 311, 162));
        gridLayout = new QGridLayout(formLayoutWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        lineEditEmail = new QLineEdit(formLayoutWidget);
        lineEditEmail->setObjectName("lineEditEmail");

        gridLayout->addWidget(lineEditEmail, 3, 1, 1, 1);

        lineEdit_PasswordCheck = new QLineEdit(formLayoutWidget);
        lineEdit_PasswordCheck->setObjectName("lineEdit_PasswordCheck");

        gridLayout->addWidget(lineEdit_PasswordCheck, 2, 1, 1, 1);

        labelPassword_Check = new QLabel(formLayoutWidget);
        labelPassword_Check->setObjectName("labelPassword_Check");

        gridLayout->addWidget(labelPassword_Check, 2, 0, 1, 1);

        labelLogin = new QLabel(formLayoutWidget);
        labelLogin->setObjectName("labelLogin");

        gridLayout->addWidget(labelLogin, 0, 0, 1, 1);

        labelEmail = new QLabel(formLayoutWidget);
        labelEmail->setObjectName("labelEmail");

        gridLayout->addWidget(labelEmail, 3, 0, 1, 1);

        labelPassword = new QLabel(formLayoutWidget);
        labelPassword->setObjectName("labelPassword");

        gridLayout->addWidget(labelPassword, 1, 0, 1, 1);

        lineEditLogin = new QLineEdit(formLayoutWidget);
        lineEditLogin->setObjectName("lineEditLogin");

        gridLayout->addWidget(lineEditLogin, 0, 1, 1, 1);

        lineEditPassword = new QLineEdit(formLayoutWidget);
        lineEditPassword->setObjectName("lineEditPassword");

        gridLayout->addWidget(lineEditPassword, 1, 1, 1, 1);


        retranslateUi(AuthForm);

        QMetaObject::connectSlotsByName(AuthForm);
    } // setupUi

    void retranslateUi(QDialog *AuthForm)
    {
        AuthForm->setWindowTitle(QCoreApplication::translate("AuthForm", "Dialog", nullptr));
        pushButtonLogIn->setText(QCoreApplication::translate("AuthForm", "Log In", nullptr));
        pushButtonSignUp->setText(QCoreApplication::translate("AuthForm", "Sign up", nullptr));
        pushButtonToReg->setText(QCoreApplication::translate("AuthForm", "To reg", nullptr));
        labelPassword_Check->setText(QCoreApplication::translate("AuthForm", "Password", nullptr));
        labelLogin->setText(QCoreApplication::translate("AuthForm", "Login", nullptr));
        labelEmail->setText(QCoreApplication::translate("AuthForm", "Email", nullptr));
        labelPassword->setText(QCoreApplication::translate("AuthForm", "Password", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AuthForm: public Ui_AuthForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUTHREGFORM_H
