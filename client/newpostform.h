#ifndef NEWPOSTFORM_H
#define NEWPOSTFORM_H

#include <QWidget>

namespace Ui {
class NewPostForm;
}

/**
 * @brief Виджет формы создания нового поста.
 *
 * Позволяет пользователю ввести заголовок и текст поста и опубликовать его.
 */
class NewPostForm : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор формы нового поста.
     * @param parent Родительский виджет.
     */
    explicit NewPostForm(QWidget *parent = nullptr);

    /**
     * @brief Деструктор формы.
     */
    ~NewPostForm();

    /**
     * @brief Получить имя текущего пользователя.
     * @return Имя пользователя.
     */
    QString getUsername() const;

    /**
     * @brief Получить заголовок поста.
     * @return Заголовок поста.
     */
    QString getHeader() const;

    /**
     * @brief Получить текст поста.
     * @return Текст поста.
     */
    QString getText() const;

    /**
     * @brief Отобразить форму как отдельное окно и поднять её на передний план.
     */
    void showAndRaise();

public slots:
    /**
     * @brief Установить текущего пользователя, создающего пост.
     * @param username Имя пользователя.
     */
    void setCurrentUser(const QString &username);

private slots:
    /**
     * @brief Обработчик кнопки "Опубликовать".
     *
     * Проверяет заполненность полей, отправляет сигнал и закрывает форму.
     */
    void on_pushButtonPost_clicked();

    /**
     * @brief Обработчик кнопки "Отмена".
     *
     * Закрывает форму без сохранения.
     */
    void on_pushButtonCancel_clicked();

private:
    /**
     * @brief Указатель на интерфейс формы, сгенерированный Qt Designer.
     */
    Ui::NewPostForm *ui;

    /**
     * @brief Имя текущего пользователя, создающего пост.
     */
    QString currentUser;

    /**
     * @brief Очистка всех полей формы.
     */
    void clearFields();

signals:
    /**
     * @brief Сигнал, испускаемый после успешного создания нового поста.
     * @param username Имя пользователя.
     * @param header Заголовок поста.
     * @param text Тело поста.
     */
    void newPostCreated(const QString &username, const QString &header, const QString &text);
};

#endif // NEWPOSTFORM_H
