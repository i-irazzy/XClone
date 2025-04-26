#include "databasemanager.h"

//инициализация статических переменных
DatabaseManager* DatabaseManager::p_instance = nullptr;  //инициализация указателя на экземпляр
DatabaseManagerDestroyer DatabaseManager::destroyer;  //инициализация объекта-уничтожителя

//приватный конструктор
DatabaseManager::DatabaseManager() {
    //инициализация базы данных
    db = QSqlDatabase::addDatabase("QSQLITE");
}

//деструктор
DatabaseManager::~DatabaseManager() {
    //если база данных открыта, закрываем её
    if (db.isOpen()) {
        db.close();
    }
}

//метод для получения единственного экземпляра класса
DatabaseManager* DatabaseManager::getInstance() {
    //если экземпляр ещё не создан, создаём его
    if (!p_instance) {
        p_instance = new DatabaseManager();
        destroyer.initialize(p_instance);  //инициализируем уничтожитель
    }
    return p_instance;
}

bool DatabaseManager::initializeDatabase(const QString& dbPath) {
    if (!connect(dbPath)) {
        return false;
    }

    const QString createUsersTable = "CREATE TABLE IF NOT EXISTS users ("
                                     "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                     "username TEXT UNIQUE NOT NULL, "
                                     "password TEXT NOT NULL);";
    const QString createTweetsTable = "CREATE TABLE IF NOT EXISTS tweets ("
                                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                      "username TEXT NOT NULL, "
                                      "tweet TEXT NOT NULL);";
    const QString createFollowsTable = "CREATE TABLE IF NOT EXISTS follows ("
                                       "follower TEXT NOT NULL, "
                                       "followee TEXT NOT NULL);";

    return executeQuery(createUsersTable) &&
           executeQuery(createTweetsTable) &&
           executeQuery(createFollowsTable);
}

//подключение к базе данных по указанному пути
bool DatabaseManager::connect(const QString& dbPath) {
    // Устанавливаем имя базы данных (путь к файлу)
    db.setDatabaseName(dbPath);

    //пытаемся открыть базу данных
    if (!db.open()) {
        qDebug() << "Error" << db.lastError().text();
        return false;
    }
    return true;
}

// Отключение от базы данных
void DatabaseManager::disconnect() {
    // Если база данных открыта, закрываем её
    if (db.isOpen()) {
        db.close();
        qDebug() << "Disconnected";
    }
}

//выполнение SQL-запроса
bool DatabaseManager::executeQuery(const QString& query) {
    QSqlQuery sqlQuery;

    //пытаемся выполнить запрос
    if (!sqlQuery.exec(query)) {
        qDebug() << "Error" << sqlQuery.lastError().text();
        return false;
    }

    return true;
}

//получение результата SQL-запроса
QSqlQuery DatabaseManager::getQueryResult(const QString& query) {
    QSqlQuery sqlQuery;

    //пытаемся выполнить запрос
    if (!sqlQuery.exec(query)) {
        qDebug() << "Error" << sqlQuery.lastError().text();
    }

    // Возвращаем результат запроса
    return sqlQuery; //QStringList
}

//авторизация пользователя
QByteArray DatabaseManager::auth(const QString& log, const QString& pass) {
    //формируем SQL-запрос для поиска пользователя с указанным логином и паролем
    //.arg подставляет в запрос нужные данные
    QString query = QString("SELECT username FROM users WHERE username = '%1' AND password = '%2';").arg(log, pass);
    //выполняем запрос к бд
    QSqlQuery result = getQueryResult(query);

    if (result.next()) {
        return "log+";
    } else {
        return "error";
    }
}

//регистрация пользователя
QByteArray DatabaseManager::reg(const QString& log, const QString& pass) {
    QString checkQuery = QString("SELECT username FROM users WHERE username = '%1';").arg(log);
    //запрос к бд
    QSqlQuery result = getQueryResult(checkQuery);

    //если пользователь с таким логином уже существует, возвращаем "error"
    if (result.next()) {
        return "error";
    }

    //если пользователь не существует, формируем запрос на добавление нового пользователя
    QString insertQuery = QString("INSERT INTO users (username, password) VALUES ('%1', '%2');").arg(log, pass);

    if (executeQuery(insertQuery)) {
        return "reg+";
    } else {
        return "error";
    }
}

//получение постов пользователя
QByteArray DatabaseManager::getposts(const QString& log, const QString& pass) {
    //проверяем авторизацию
    if (auth(log, pass) == "error") {
        return "error";
    }
    //создаем список для хранения твитов
    QList<QString> tweets;

    QString query = QString("SELECT tweet FROM tweets WHERE username = '%1';").arg(log);
    QSqlQuery result = getQueryResult(query);

    //перебираем результат запроса и добавляем твиты в список
    while (result.next()) {
        tweets.append(result.value(0).toString());
    }
    //возвращаем твиты в формате строки, разделенной " | "
    return "TWEETS: " + tweets.join(" | ").toUtf8();
}

//создание твита
QByteArray DatabaseManager::postTweet(const QString& log, const QString& tweet) {
    QString insertQuery = QString("INSERT INTO tweets (username, tweet) VALUES ('%1', '%2');").arg(log, tweet);
    if (executeQuery(insertQuery)) {
        return "succes";
    } else {
        return "error";
    }
}

// Подписка на пользователя
QByteArray DatabaseManager::followUser(const QString& follower, const QString& followee) {
     //проверяем, существует ли пользователь, на которого подписываются
    QString checkQuery = QString("SELECT username FROM users WHERE username = '%1';").arg(followee);
    QSqlQuery result = getQueryResult(checkQuery);

    //если пользователь не существует
    if (!result.next()) {
        return "error";
    }

    //формируем SQL-запрос для добавления подписки
    QString insertQuery = QString("INSERT INTO follows (follower, followee) VALUES ('%1', '%2');").arg(follower, followee);
    if (executeQuery(insertQuery)) {
        return "success";
    } else {
        return "error";
    }
}

// Получение ленты твитов
QByteArray DatabaseManager::getFeed(const QString& log) {
    //создаем список для хранения твитов
    QList<QString> board;
    //формируем SQL-запрос для получения списка пользователей, на которых подписан текущий пользователь
    QString followeesQuery = QString("SELECT followee FROM follows WHERE follower = '%1';").arg(log);
    QSqlQuery followeesResult = getQueryResult(followeesQuery);

    //перебираем результат запроса (список подписанных пользователей)
    while (followeesResult.next()) {
        //получаем имя пользователя, на которого подписан текущий пользователь
        QString followee = followeesResult.value(0).toString();
        //формируем SQL-запрос для получения твитов этого пользователя
        QString tweetsQuery = QString("SELECT tweet FROM tweets WHERE username = '%1';").arg(followee);
        QSqlQuery tweetsResult = getQueryResult(tweetsQuery);

        while (tweetsResult.next()) {
            //добавляем твит в ленту
            board.append(tweetsResult.value(0).toString());
        }
    }
    //возвращаем ленту твитов в формате строки, разделенной " | "
    return "POSTS: " + board.join(" | ").toUtf8();
}

//удаление твита
QByteArray DatabaseManager::delTweet(const QString& username, int tweetId) {
    //проверяем, принадлежит ли твит указанному пользователю
    QString checkQuery = QString("SELECT id FROM tweets WHERE id = %1 AND username = '%2';").arg(tweetId).arg(username);
    QSqlQuery result = getQueryResult(checkQuery);

    if (!result.next()) {
        return "error";  //твит не принадлежит пользователю или не существует
    }

    //удаляем твит
    QString deleteQuery = QString("DELETE FROM tweets WHERE id = %1;").arg(tweetId);

    if (executeQuery(deleteQuery)) {
        return "success";
    } else {
        return "error";
    }
}
