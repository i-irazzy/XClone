#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QByteArray>


//класс-уничтожитель для управления временем жизни единственного экземпляра DatabaseManager
class DatabaseManagerDestroyer;

//класс DatabaseManager реализует singleton для управления подключением к базе данных
class DatabaseManager
{
private:
    static DatabaseManager* p_instance;  //указатель на единственный экземпляр класса
    static DatabaseManagerDestroyer destroyer;  //объект-уничтожитель для корректного удаления экземпляра

    QSqlDatabase db;  //объект базы данных

    //приватные конструкторы и операторы (для предотвращения создания копий)
    DatabaseManager();  //конструктор по умолчанию
    DatabaseManager(const DatabaseManager&) = delete;  //запрещаем копирование
    DatabaseManager& operator=(const DatabaseManager&) = delete;  //запрещаем присваивание
    ~DatabaseManager();  //деструктор

    friend class DatabaseManagerDestroyer;  //разрешаем доступ к приватным методам для уничтожителя

    // приватные методы для работы с базой данных
    bool connect(const QString& dbPath);  //подключение к базе данных
    void disconnect();  //отключение от базы данных
    bool executeQuery(const QString& query);  //выполнение SQL-запроса
    QSqlQuery getQueryResult(const QString& query);  //получение результата SQL-запроса

public:
    //метод для получения единственного экземпляра класса
    static DatabaseManager* getInstance();

    //публичные методы для обработки запросов
    QByteArray auth(const QString& log, const QString& pass);  //авторизация пользователя
    QByteArray reg(const QString& log, const QString& pass);  //регистрация пользователя
    QByteArray getposts(const QString& log, const QString& pass);  //получение постов пользователя
    QByteArray postTweet(const QString& log, const QString& tweet);  //создание твита
    QByteArray followUser(const QString& follower, const QString& followee);  //подписка на пользователя
    QByteArray getFeed(const QString& log);  //получение ленты твитов
    QByteArray delTweet(const QString& username, int tweetId); //удаление твита
};

//класс-уничтожитель для управления временем жизни экземпляра DatabaseManager
class DatabaseManagerDestroyer
{
private:
    DatabaseManager* p_instance;  //указатель на экземпляр DatabaseManager

public:
    ~DatabaseManagerDestroyer() { delete p_instance; }  // дструктор, который удаляет экземпляр
    void initialize(DatabaseManager* p) { p_instance = p; }  // инициализация указателя
};

#endif // DATABASEMANAGER_H
