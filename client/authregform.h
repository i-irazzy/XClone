#ifndef AUTHREGFORM_H
#define AUTHREGFORM_H

#include <QDialog>
#include "functionsforclient.h"
#include <QMessageBox>

namespace Ui {
class AuthForm;
}

class AuthRegForm : public QDialog
{
    Q_OBJECT

public:
    explicit AuthRegForm(QWidget *parent = nullptr);
    ~AuthRegForm();

private slots:
    void on_pushButtonToReg_clicked();

    void on_pushButtonLogIn_clicked();

    void on_pushButtonSignUp_clicked();

signals:
    void auth_ok(const QString &username);

private:
    Ui::AuthForm *ui;
    void change_type_to_reg(bool);
    void clear();
};

#endif
