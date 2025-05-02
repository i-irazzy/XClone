#ifndef MANAGERFORMS_H
#define MANAGERFORMS_H

#include <QMainWindow>
#include "authregform.h"
#include "mainwindow.h"

namespace Ui {
class ManagerForms;  // Объявление класса, сгенерированного Qt для интерфейса
}

class ManagerForms : public QMainWindow
{
    Q_OBJECT

public:
    explicit ManagerForms(QWidget *parent = nullptr);
    ~ManagerForms();

private slots:
    void onAuthSuccess(QString username);

private:
    Ui::ManagerForms *ui;  // Указатель на сгенерированный интерфейс
    AuthRegForm* curr_auth;  // Указатель на форму авторизации/регистрации
    MainWindow* main_form;   // Указатель на основное окно с постами
};

#endif
