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
        qDebug() << "Ошибка подключения к базе данных!";
        return false;
    }

    ///< Создаем новую таблицу пользователей
    const QString createUsersTable = "CREATE TABLE IF NOT EXISTS users ("
                                     "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                     "username TEXT UNIQUE NOT NULL, "
                                     "password TEXT NOT NULL, "
                                     "email TEXT NOT NULL);";
    bool usersTableCreated = executeQuery(createUsersTable);

    ///< Создаем таблицу постов
    const QString createPostsTable = "CREATE TABLE IF NOT EXISTS posts ("
                                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                      "username TEXT NOT NULL, "
                                      "header TEXT NOT NULL, "   // Добавляем заголовок поста
                                      "text TEXT NOT NULL);";
    bool tweetsTableCreated = executeQuery(createPostsTable);

    ///< Проверяем успешность создания обеих таблиц
    if (!usersTableCreated || !tweetsTableCreated) {
        qDebug() << "Ошибка при создании таблиц!";
        return false;
    }

    return true;
}


///< Подключение к SQLite-базе данных
bool DatabaseManager::connect(const QString& dbPath) {
    db.setDatabaseName(dbPath); ///< Указываем путь к базе

    if (!db.open()) {
        qDebug() << "Ошибка подключения:" << db.lastError().text();
        return false;
    }
    return true;
    qDebug() << "Подключаемся к базе данных: " << dbPath;

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

     qDebug() << "Выполняем SQL-запрос: " << query;

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
    qDebug() << "Функция auth() вызвана! Логин=" << log << ", Пароль=" << pass;
    QSqlQuery query;
    query.prepare("SELECT username FROM users WHERE username = ? AND password = ?");
    query.addBindValue(log);
    query.addBindValue(pass);

    qDebug() << "SQL-запрос на авторизацию: логин=" << log << ", пароль=" << pass;

    if (!query.exec()) {
        qDebug() << "Ошибка SQL: " << query.lastError().text();
        return "error";
    }

    if (query.next()) {
        QString foundUsername = query.value(0).toString();
        qDebug() << "Найден username в базе: " << foundUsername;
        return "log+";
    }

    qDebug() << "Авторизация не найдена, возвращаем error.";
    return "error";
}


///< Регистрация пользователя
QByteArray DatabaseManager::reg(const QString& log, const QString& pass, const QString& email) {
    QString checkQuery = QString("SELECT username FROM users WHERE username = '%1';").arg(log);
    QSqlQuery result = getQueryResult(checkQuery);

    if (result.next()) {
        return "error"; ///< Пользователь уже существует
    }

    QString insertQuery = QString("INSERT INTO users (username, password, email) VALUES ('%1', '%2', '%3');").arg(log, pass, email);
    return executeQuery(insertQuery) ? "reg+" : "error";
}


QByteArray DatabaseManager::getPostsByUser(const QString& log) {
    QList<QString> posts;
    QString query = QString("SELECT username, header, text FROM posts WHERE username = '%1';").arg(log);
    QSqlQuery result = getQueryResult(query);

    while (result.next()) {
        QString username = result.value(0).toString();
        QString header   = result.value(1).toString();
        QString text     = result.value(2).toString();

        posts.append(username + "|" + header + "|" + text);  // Формат: username|header|text
    }

    return posts.join("\n").toUtf8();
}

///< Поиск постов по ключевому слову
QByteArray DatabaseManager::getPostsByText(const QString& text) {
    QStringList board;
    QString searchQuery = QString("SELECT username, header, text FROM posts WHERE text LIKE '%%1%';").arg(text);
    QSqlQuery searchResult = getQueryResult(searchQuery);

    while (searchResult.next()) {
        QString postData = QString("%1|%2|%3")
        .arg(searchResult.value(0).toString())  // username
            .arg(searchResult.value(1).toString())  // header
            .arg(searchResult.value(2).toString()); // text
        board.append(postData);
    }

    return board.join("\n").toUtf8();
}



bool DatabaseManager::createPost(const QString& username, const QString& header, const QString& post) {
    qDebug() << "Функция createPost() вызвана! username=" << username << ", header=" << header << ", post=" << post;

    qDebug() << "Создание поста: username=" << username << ", header=" << header << ", post=" << post;

    QString insertQuery = QString("INSERT INTO posts (username, header, post) VALUES ('%1', '%2', '%3');")
                              .arg(username, header, post);

    qDebug() << "SQL-запрос: " << insertQuery;

    bool success = executeQuery(insertQuery);
    qDebug() << "Результат SQL-запроса: " << (success ? "Успех!" : "Ошибка!");

    return success;
}



QByteArray DatabaseManager::getAllPosts() {
    QList<QString> board;
    QString allPostsQuery = "SELECT id, username, header, text FROM posts;";
    QSqlQuery postsResult = getQueryResult(allPostsQuery);

    while (postsResult.next()) {
        int postId = postsResult.value(0).toInt();
        QString username = postsResult.value(1).toString();
        QString header = postsResult.value(2).toString();
        QString text = postsResult.value(3).toString();

        board.append(QString("%1|%2|%3|%4").arg(postId).arg(username).arg(header).arg(text));  // Формат: id|username|header|text
    }

    return board.join("\n").toUtf8();
}


///< Удаление поста по ID и пользователю
QByteArray DatabaseManager::delPost(int postId) {
    QString checkQuery = QString("SELECT id FROM posts WHERE id = %1;").arg(postId);
    QSqlQuery result = getQueryResult(checkQuery);

    if (!result.next()) {
        return "error"; ///< Пост не найден или не принадлежит пользователю
    }

    QString deleteQuery = QString("DELETE FROM posts WHERE id = %1;").arg(postId);
    return executeQuery(deleteQuery) ? "success" : "error";
}
