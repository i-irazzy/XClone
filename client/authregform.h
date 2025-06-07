#ifndef AUTHREGFORM_H
#define AUTHREGFORM_H

#include <QDialog>
#include "functionsforclient.h"
#include <QMessageBox>

namespace Ui {
class AuthForm;
}

/**
 * @brief Диалоговое окно для авторизации и регистрации пользователей.
 */
class AuthRegForm : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор класса AuthRegForm.
     * @param parent Родительский виджет.
     */
    explicit AuthRegForm(QWidget *parent = nullptr);

    /**
     * @brief Деструктор класса AuthRegForm.
     */
    ~AuthRegForm();

private slots:
    /**
     * @brief Слот для переключения на форму регистрации.
     */
    void on_pushButtonToReg_clicked();

    /**
     * @brief Слот обработки попытки входа в систему.
     */
    void on_pushButtonLogIn_clicked();

    /**
     * @brief Слот обработки попытки регистрации нового пользователя.
     */
    void on_pushButtonSignUp_clicked();

signals:
    /**
     * @brief Сигнал об успешной авторизации.
     * @param username Имя пользователя.
     */
    void auth_ok(const QString &username);

private:
    /**
     * @brief Указатель на UI-интерфейс формы.
     */
    Ui::AuthForm *ui;

    /**
     * @brief Изменяет форму между авторизацией и регистрацией.
     * @param isReg true — форма регистрации, false — форма входа.
     */
    void change_type_to_reg(bool isReg);

    /**
     * @brief Очищает поля ввода.
     */
    void clear();
};

#endif // AUTHREGFORM_H
