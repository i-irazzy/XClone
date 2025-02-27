#ifndef FUNCTIONSFORSERVER_H
#define FUNCTIONSFORSERVER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QDebug>

class FunctionsForServer : public QObject
{
    Q_OBJECT

public:
    explicit FunctionsForServer(QObject *parent = nullptr); //explicit для предотвращения неявного преобразования типов при вызове конструктора

    bool regUser(const QString& username, const QString& password); //регистрация пользователя

    bool logUser(const QString& username, const QString& password); //авторизация

    bool createTweet(const QString& username, const QString& tweet); //создания поста

    QList<QString> getTweets(const QString& username); //просмотр постов пользователя. Список элементов типа QString

    bool delTweet(const QString& username, int tweetId); //удаление поста

    QList<QString> getAllTweets(const QString& username); //лента твитов (твиты от пользователей, на которых подписан текущий пользователь). Также список
    //элементов типа QString

    bool followUser(const QString& follower, const QString& followee); //подписка на пользователя

    QString parse(const QString& command); //парсинг

private:
    // заглушка для хранения пользователей
    QList<QPair<QString, QString>> users;  //регистрация и авторизация

    // заглушка для хранения твитов
    QList<QPair<QString, QString>> tweets;  //создание, удаление поста + просмотр постов пользователя

    //заглушка для хранения подписок
    QList<QPair<QString, QString>> follows;  //лента твитов + подписка на пользователя
};

#endif // FUNCTIONSFORSERVER_H
