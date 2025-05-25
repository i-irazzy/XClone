#include "authregform.h"
#include "ui_authregform.h"


AuthRegForm::AuthRegForm(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AuthForm)
{
   ui->setupUi(this);
   change_type_to_reg(false);

}

AuthRegForm::~AuthRegForm()
{
    delete ui;
}

void AuthRegForm::change_type_to_reg(bool is_reg){

    ui->labelPassword_Check->setVisible(is_reg);
    ui->labelEmail->setVisible(is_reg);
    ui->lineEdit_PasswordCheck->setVisible(is_reg);
    ui->lineEditEmail->setVisible(is_reg);
    ui->pushButtonSignUp->setVisible(is_reg);

    ui->pushButtonLogIn->setVisible(!is_reg);
    ui->pushButtonToReg->setText(is_reg? "to Log" : "to Reg");
}



void AuthRegForm::on_pushButtonToReg_clicked()
{
    this->change_type_to_reg(ui->pushButtonLogIn->isVisible());
}

void AuthRegForm::on_pushButtonLogIn_clicked() {
    QString username = ui->lineEditLogin->text().trimmed();
    QString password = ui->lineEditPassword->text().trimmed();

    if (auth(username, password)) {
        currentUsername = username;
        qDebug() << "Авторизация успешна! currentUsername=" << currentUsername;
        emit auth_ok(username);
        this->close();
    } else {
        QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль.");
    }
}


void AuthRegForm:: clear(){
    ui->lineEditLogin->clear();
    ui->lineEditPassword->clear();
    ui->lineEditEmail->clear();
    ui->lineEdit_PasswordCheck->clear();

}

void AuthRegForm::on_pushButtonSignUp_clicked()
{
    QString username = ui->lineEditLogin->text().trimmed();
    QString password = ui->lineEditPassword->text().trimmed();
    QString passwordCheck = ui->lineEdit_PasswordCheck->text().trimmed();
    QString email = ui->lineEditEmail->text().trimmed();

    if (username.isEmpty() || password.isEmpty() || email.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, заполните все поля.");
        return;
    }

    if (password != passwordCheck) {
        QMessageBox::warning(this, "Ошибка", "Пароли не совпадают.");
        clear();
        return;
    }

    QString request = QString("REGISTER|%1|%2|%3").arg(username, password, email);
    QByteArray response = ClientAPI::getInstance()->query_to_server(request);

    if (response.trimmed() == "reg+") {
        emit auth_ok(username);
        this->accept();
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось зарегистрироваться.");
        clear();
    }
}

