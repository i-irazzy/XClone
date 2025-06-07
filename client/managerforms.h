#ifndef MANAGERFORMS_H
#define MANAGERFORMS_H

#include <QMainWindow>
#include "authregform.h"
#include "mainwindow.h"

namespace Ui {
class ManagerForms;
}

/**
 * @brief Главное окно приложения, управляющее переходом между формой авторизации и основной формой.
 */
class ManagerForms : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор окна ManagerForms.
     * @param parent Родительский виджет.
     */
    explicit ManagerForms(QWidget *parent = nullptr);

    /**
     * @brief Деструктор.
     */
    ~ManagerForms();

private slots:
    /**
     * @brief Слот, вызываемый после успешной авторизации.
     * @param username Имя пользователя, вошедшего в систему.
     */
    void onAuthSuccess(QString username);

private:
    /**
     * @brief Интерфейс, сгенерированный Qt Designer.
     */
    Ui::ManagerForms *ui;

    /**
     * @brief Указатель на форму авторизации и регистрации.
     */
    AuthRegForm* curr_auth;

    /**
     * @brief Указатель на основное окно приложения (список постов).
     */
    MainWindow* main_form;
};

#endif // MANAGERFORMS_H
