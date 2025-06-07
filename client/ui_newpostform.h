/********************************************************************************
** Form generated from reading UI file 'newpostform.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWPOSTFORM_H
#define UI_NEWPOSTFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NewPostForm
{
public:
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonCancel;
    QPushButton *pushButtonPost;
    QTextEdit *textEditTextBodyFild;
    QLineEdit *lineEditHeaderFild;

    void setupUi(QWidget *NewPostForm)
    {
        if (NewPostForm->objectName().isEmpty())
            NewPostForm->setObjectName("NewPostForm");
        NewPostForm->resize(400, 300);
        horizontalLayoutWidget = new QWidget(NewPostForm);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(40, 200, 301, 131));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButtonCancel = new QPushButton(horizontalLayoutWidget);
        pushButtonCancel->setObjectName("pushButtonCancel");

        horizontalLayout->addWidget(pushButtonCancel);

        pushButtonPost = new QPushButton(horizontalLayoutWidget);
        pushButtonPost->setObjectName("pushButtonPost");

        horizontalLayout->addWidget(pushButtonPost);

        textEditTextBodyFild = new QTextEdit(NewPostForm);
        textEditTextBodyFild->setObjectName("textEditTextBodyFild");
        textEditTextBodyFild->setGeometry(QRect(40, 70, 301, 121));
        lineEditHeaderFild = new QLineEdit(NewPostForm);
        lineEditHeaderFild->setObjectName("lineEditHeaderFild");
        lineEditHeaderFild->setGeometry(QRect(40, 30, 301, 31));

        retranslateUi(NewPostForm);

        QMetaObject::connectSlotsByName(NewPostForm);
    } // setupUi

    void retranslateUi(QWidget *NewPostForm)
    {
        NewPostForm->setWindowTitle(QCoreApplication::translate("NewPostForm", "Form", nullptr));
        pushButtonCancel->setText(QCoreApplication::translate("NewPostForm", "Cancel", nullptr));
        pushButtonPost->setText(QCoreApplication::translate("NewPostForm", "Post", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NewPostForm: public Ui_NewPostForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWPOSTFORM_H
