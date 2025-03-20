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
bool createTweet(const QString& username, const QString& tweet); //создание твита
QList<QString> getTweets(const QString& username); //получение твитов пользователя
bool delTweet(const QString& username, int tweetId); //удаление твита
QList<QString> getAllTweets(const QString& username); //лента твитов
bool followUser(const QString& follower, const QString& followee); //подписка на пользователя
QString parse(const QString& command); //парсинг команд

#endif // FUNCTIONSFORSERVER_H
