#ifndef FUNCTIONSFORSERVER_H
#define FUNCTIONSFORSERVER_H

#include <QString>
#include <QList>
#include <QDebug>
#include <QPair>
#include <QSqlQuery>

/**
 * @brief Регистрирует нового пользователя в базе данных.
 *
 * Использует DatabaseManager для добавления нового пользователя.
 *
 * @param username Имя пользователя.
 * @param password Пароль пользователя.
 * @return true, если регистрация прошла успешно; false — если пользователь уже существует или произошла ошибка.
 */
bool regUser(const QString& username, const QString& password);

/**
 * @brief Авторизует пользователя, проверяя логин и пароль.
 *
 * Использует DatabaseManager для проверки данных авторизации.
 *
 * @param username Имя пользователя.
 * @param password Пароль пользователя.
 * @return true при успешной авторизации; false — при ошибке или неверных данных.
 */
bool logUser(const QString& username, const QString& password);

/**
 * @brief Создаёт новый пост от имени пользователя.
 * @param username Имя пользователя, создающего пост.
 * @param header Название поста.
 * @param data Содержимое поста.
 * @return true, если пост успешно сохранён; false — при ошибке.
 */
bool createPost(const QString& username, const QString& header, const QString& post);


/**
 * @brief Удаляет пост пользователя по идентификатору.
 * @param username Имя пользователя, которому принадлежит пост.
 * @param postId Идентификатор поста.
 * @return true, если удаление прошло успешно; false — при ошибке.
 */
bool deletePost(const QString& username, int postId);

/**
 * @brief Получает список всех постов из базы данных.
 * @return Список строк, каждая из которых представляет отдельный пост.
 */
QList<QString> getAllPosts();

/**
 * @brief Получает все посты, опубликованные указанным пользователем.
 * @param username Имя пользователя.
 * @return Список постов данного пользователя.
 */
QList<QString> getPostsByUser(const QString& username);

/**
 * @brief Получает все посты, содержащие заданный текст.
 * @param data Текст для поиска в постах.
 * @return Список постов, содержащих указанный текст.
 */
QList<QString> getPostsByText(const QString& data);

/**
 * @brief Интерпретирует команду клиента и вызывает соответствующую функцию.
 *
 * Поддерживает команды REGISTER, LOGIN, CREATE_POST, DELETE_POST,
 * GET_POSTS_BY_USER, GET_POSTS_BY_TEXT, GET_ALL_POSTS.
 *
 * @param command Строка команды.
 * @return Ответ в виде строки (например: "reg+", "log+", "POSTS: ..." или "error").
 */
QString parse(const QString& command);


#endif // FUNCTIONSFORSERVER_H
