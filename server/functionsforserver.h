#ifndef FUNCTIONSFORSERVER_H
#define FUNCTIONSFORSERVER_H

#include <QString>
#include <QList>
#include <QDebug>
#include <QPair>
#include <QSqlQuery>


// Объявляем функции
bool regUser(const QString& username, const QString& password); //регистрация пользователя
bool logUser(const QString& username, const QString& password); //авторизация
bool createPost(const QString& username, const QString& data); //создание поста
bool deletePost(const QString& username, int postId); //удаление поста
QList<QString> getAllPosts(); //все посты
QList<QString> getPostsByUser(const QString& username); //получение постов по пользователю
QList<QString> getPostsByText(const QString& data); //получение постов по тексту
QString parse(const QString& command); //парсинг команд

#endif // FUNCTIONSFORSERVER_H
