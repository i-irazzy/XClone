#include "newpostform.h"
#include "clientapi.h"
#include "ui_newpostform.h"

void NewPostForm::showAndRaise()
{
    this->setWindowFlags(Qt::Window); // Сделаем виджет отдельным окном
    this->show();
    this->raise();
    this->activateWindow();
    this->updateGeometry(); // Обновляем расположение виджета
}



NewPostForm::NewPostForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NewPostForm)
{
    ui->setupUi(this);
    this->setVisible(false);
    connect(ui->pushButtonCancel, &QPushButton::clicked, this, &NewPostForm::close);
}

NewPostForm::~NewPostForm()
{
    delete ui;
}

void NewPostForm::setCurrentUser(const QString &username) {

    currentUser = username;
}

QString NewPostForm::getHeader() const
{
    return ui->lineEditHeaderFild->text();
}

QString NewPostForm::getText() const
{
    return ui->textEditTextBodyFild->toPlainText();
}

QString NewPostForm::getUsername() const {

    return currentUser;
}

void NewPostForm::clearFields()
{
    ui->lineEditHeaderFild->clear();
    ui->textEditTextBodyFild->clear();
}

void NewPostForm::on_pushButtonPost_clicked()
{
    QString header = getHeader().trimmed();
    QString text = getText().trimmed();
    if (header.isEmpty() || text.isEmpty()) {
        return;
    }

    emit newPostCreated(currentUser, header, text);// Отправляем сигнал с данными поста

    close();
    clearFields();
}

void NewPostForm::on_pushButtonCancel_clicked()
{
    this->close();
}
