#ifndef FUNCTIONSFORCLIENT_H
#define FUNCTIONSFORCLIENT_H

#include <QString>
#include "mainwindow.h"  ///< Для структуры Post

struct Post;

/**
 * @brief Проверяет логин и пароль пользователя.
 *
 * В текущей реализации возвращает true только для login == "user" и password == "123".
 * Реальная отправка запроса на сервер закомментирована.
 *
 * @param login Логин пользователя.
 * @param password Пароль пользователя.
 * @return true, если введены допустимые тестовые данные; false — иначе.
 */
bool auth(QString login, QString password);

/**
 * @brief Регистрирует нового пользователя.
 *
 * В текущей реализации возвращает true, если login не равен "user" и password не "123".
 * Настоящая серверная регистрация закомментирована.
 *
 * @param login Логин пользователя.
 * @param password Пароль.
 * @param email Адрес электронной почты.
 * @return true, если пользователь может быть зарегистрирован; false — если логин/пароль совпадают с тестовыми.
 */
bool reg(QString login, QString password, QString email);

/**
 * @brief Отправляет запрос на создание нового поста на сервере.
 *
 * Отправляет строку вида "CREATE|username|header|text" и ожидает ответ "OK".
 *
 * @param username Имя пользователя, создающего пост.
 * @param header Заголовок поста.
 * @param text Содержимое поста.
 * @return true, если сервер подтвердил создание поста; false — иначе.
 */
bool createNewPost(const QString& username, const QString& header, const QString& text);

/**
 * @brief Получает список всех постов указанного пользователя с сервера.
 *
 * Отправляется запрос "GET_POSTS_BY_USER|username". Сервер возвращает посты построчно.
 * Каждая строка содержит username|header|text.
 *
 * @param username Имя пользователя, чьи посты нужно получить.
 * @return Список постов пользователя.
 */
QList<Post> getPostsByUser(const QString& username);

/**
 * @brief Получает список постов, содержащих заданный текст.
 *
 * Отправляется строка "GET_POSTS_BY_TEXT|text", сервер возвращает совпадающие посты.
 * Формат каждой строки: username|header|text.
 *
 * @param text Строка поиска (по содержимому).
 * @return Список найденных постов.
 */
QList<Post> getPostsByText(const QString& text);

/**
 * @brief Отправляет на сервер запрос на удаление поста по ID.
 *
 * Отправляется строка "DELETE_POST|username|postId". Сервер должен вернуть "OK" при успешном удалении.
 *
 * @param username Имя пользователя (владелец поста).
 * @param postId Уникальный идентификатор поста.
 * @return true, если пост успешно удалён; false — иначе.
 */
bool deletePost(const QString& username, int postId);

#endif // FUNCTIONSFORCLIENT_H
