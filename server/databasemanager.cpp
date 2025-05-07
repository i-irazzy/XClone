#include "databasemanager.h"

///< Инициализация указателя на экземпляр DatabaseManager
DatabaseManager* DatabaseManager::p_instance = nullptr;

///< Инициализация объекта-уничтожителя
DatabaseManagerDestroyer DatabaseManager::destroyer;

///< Приватный конструктор
DatabaseManager::DatabaseManager() {
    db = QSqlDatabase::addDatabase("QSQLITE"); ///< Указываем тип СУБД — SQLite
}

///< Деструктор — закрытие базы данных при удалении
DatabaseManager::~DatabaseManager() {
    if (db.isOpen()) {
        db.close(); ///< Закрываем базу данных, если она была открыта
    }
}

///< Получение единственного экземпляра DatabaseManager
DatabaseManager* DatabaseManager::getInstance() {
    if (!p_instance) {
        p_instance = new DatabaseManager(); ///< Создаем новый экземпляр
        destroyer.initialize(p_instance);   ///< Инициализируем уничтожитель
    }
    return p_instance;
}

///< Инициализация базы данных и создание таблиц
bool DatabaseManager::initializeDatabase(const QString& dbPath) {
    if (!connect(dbPath)) {
        return false;
    }

    ///< Создаем таблицу пользователей
    const QString createUsersTable = "CREATE TABLE IF NOT EXISTS users ("
                                     "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                     "username TEXT UNIQUE NOT NULL, "
                                     "password TEXT NOT NULL);";

    ///< Создаем таблицу постов
    const QString createTweetsTable = "CREATE TABLE IF NOT EXISTS tweets ("
                                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                      "username TEXT NOT NULL, "
                                      "tweet TEXT NOT NULL);";

    return executeQuery(createUsersTable) && executeQuery(createTweetsTable);
}

///< Подключение к SQLite-базе данных
bool DatabaseManager::connect(const QString& dbPath) {
    db.setDatabaseName(dbPath); ///< Указываем путь к базе

    if (!db.open()) {
        qDebug() << "Ошибка подключения:" << db.lastError().text();
        return false;
    }
    return true;
}

///< Отключение от базы данных
void DatabaseManager::disconnect() {
    if (db.isOpen()) {
        db.close();
        qDebug() << "Отключение от базы данных.";
    }
}

///< Выполнение SQL-запроса без получения результата
bool DatabaseManager::executeQuery(const QString& query) {
    QSqlQuery sqlQuery;
    if (!sqlQuery.exec(query)) {
        qDebug() << "Ошибка выполнения запроса:" << sqlQuery.lastError().text();
        return false;
    }
    return true;
}

///< Получение результата SQL-запроса
QSqlQuery DatabaseManager::getQueryResult(const QString& query) {
    QSqlQuery sqlQuery;
    if (!sqlQuery.exec(query)) {
        qDebug() << "Ошибка запроса:" << sqlQuery.lastError().text();
    }
    return sqlQuery;
}

///< Авторизация пользователя по логину и паролю
QByteArray DatabaseManager::auth(const QString& log, const QString& pass) {
    QString query = QString("SELECT username FROM users WHERE username = '%1' AND password = '%2';").arg(log, pass);
    QSqlQuery result = getQueryResult(query);

    if (result.next()) {
        return "log+";
    } else {
        return "error";
    }
}

///< Регистрация пользователя
QByteArray DatabaseManager::reg(const QString& log, const QString& pass) {
    QString checkQuery = QString("SELECT username FROM users WHERE username = '%1';").arg(log);
    QSqlQuery result = getQueryResult(checkQuery);

    if (result.next()) {
        return "error"; ///< Пользователь уже существует
    }

    QString insertQuery = QString("INSERT INTO users (username, password) VALUES ('%1', '%2');").arg(log, pass);
    return executeQuery(insertQuery) ? "reg+" : "error";
}

///< Получение постов пользователя по логину
QByteArray DatabaseManager::getPostsByUser(const QString& log) {
    QList<QString> posts;
    QString query = QString("SELECT post FROM posts WHERE username = '%1';").arg(log);
    QSqlQuery result = getQueryResult(query);

    while (result.next()) {
        posts.append(result.value(0).toString());
    }

    return "POSTS: " + posts.join(" | ").toUtf8();
}

///< Поиск постов по ключевому слову
QByteArray DatabaseManager::getPostsByText(const QString& text) {
    QList<QString> board;
    QString searchQuery = QString("SELECT post FROM posts WHERE post LIKE '%%1%';").arg(text);
    QSqlQuery searchResult = getQueryResult(searchQuery);

    while (searchResult.next()) {
        board.append(searchResult.value(0).toString());
    }

    return "POSTS: " + board.join(" | ").toUtf8();
}

///< Создание нового поста
QByteArray DatabaseManager::createPost(const QString& log, const QString& post) {
    QString insertQuery = QString("INSERT INTO posts (username, post) VALUES ('%1', '%2');").arg(log, post);
    return executeQuery(insertQuery) ? "succes" : "error";
}

///< Получение всех постов
QByteArray DatabaseManager::getAllPosts() {
    QList<QString> board;
    QString allPostsQuery = "SELECT post FROM posts;";
    QSqlQuery postsResult = getQueryResult(allPostsQuery);

    while (postsResult.next()) {
        board.append(postsResult.value(0).toString());
    }

    return "POSTS: " + board.join(" | ").toUtf8();
}

///< Удаление поста по ID и пользователю
QByteArray DatabaseManager::delPost(const QString& username, int postId) {
    QString checkQuery = QString("SELECT id FROM posts WHERE id = %1 AND username = '%2';").arg(postId).arg(username);
    QSqlQuery result = getQueryResult(checkQuery);

    if (!result.next()) {
        return "error"; ///< Пост не найден или не принадлежит пользователю
    }

    QString deleteQuery = QString("DELETE FROM posts WHERE id = %1;").arg(postId);
    return executeQuery(deleteQuery) ? "success" : "error";
}
