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
    return executeQuery(createUsersTable) && executeQuery(createTweetsTable);
}

//подключение к базе данных по указанному пути
bool DatabaseManager::connect(const QString& dbPath) {
    // Устанавливаем имя базы данных (путь к файлу)
    db.setDatabaseName(dbPath);

    //пытаемся открыть базу данных
    if (!db.open()) {
        qDebug() << "Ошибка." << db.lastError().text();
        return false;
    }
    return true;
}

// Отключение от базы данных
void DatabaseManager::disconnect() {
    // Если база данных открыта, закрываем её
    if (db.isOpen()) {
        db.close();
        qDebug() << "Отключение от базы данных.";
    }
}

//выполнение SQL-запроса
bool DatabaseManager::executeQuery(const QString& query) {
    QSqlQuery sqlQuery;

    //пытаемся выполнить запрос
    if (!sqlQuery.exec(query)) {
        qDebug() << "Ошибка." << sqlQuery.lastError().text();
        return false;
    }

    return true;
}

//получение результата SQL-запроса
QSqlQuery DatabaseManager::getQueryResult(const QString& query) {
    QSqlQuery sqlQuery;

    //пытаемся выполнить запрос
    if (!sqlQuery.exec(query)) {
        qDebug() << "Ошибка." << sqlQuery.lastError().text();
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

//поиск постов по пользователю
QByteArray DatabaseManager::getPostsByUser(const QString& log) {
//    //проверяем авторизацию
//    if (auth(log, pass) == "error") {
//        return "error";
//    }
    //создаем список для хранения постов
    QList<QString> posts;

    QString query = QString("SELECT post FROM posts WHERE username = '%1';").arg(log);
    QSqlQuery result = getQueryResult(query);

    //перебираем результат запроса и добавляем посты в список
    while (result.next()) {
        posts.append(result.value(0).toString());
    }
    //возвращаем посты в формате строки, разделенной " | "
    return "POSTS: " + posts.join(" | ").toUtf8();
}

//поиск постов по словам
QByteArray DatabaseManager::getPostsByText(const QString &text)
{
    QList<QString> board;

    // Запросим все посты, где текст поста содержит нужную строку
    QString searchQuery = QString("SELECT post FROM posts WHERE post LIKE '%%1%';").arg(text);
    QSqlQuery searchResult = getQueryResult(searchQuery);

    while (searchResult.next()) {
        board.append(searchResult.value(0).toString());
    }

    return "POSTS: " + board.join(" | ").toUtf8();

}

//создание поста
QByteArray DatabaseManager::createPost(const QString& log, const QString& post) {
    QString insertQuery = QString("INSERT INTO posts (username, post) VALUES ('%1', '%2');").arg(log, post);
    if (executeQuery(insertQuery)) {
        return "succes";
    } else {
        return "error";
    }
}

// Получение всех постов
QByteArray DatabaseManager::getAllPosts() {
    //создаем список для хранения постов
    QList<QString> board;

    // получить все посты из бд
       QString allPostsQuery = "SELECT post FROM posts;";
       QSqlQuery postsResult = getQueryResult(allPostsQuery);

       while (postsResult.next()) {
           board.append(postsResult.value(0).toString());
       }

    //возвращаем ленту постов в формате строки, разделенной " | "
    return "POSTS: " + board.join(" | ").toUtf8();
}

//удаление поста
QByteArray DatabaseManager::delPost(const QString& username, int postId) {
    //проверяем, принадлежит ли пост указанному пользователю
    QString checkQuery = QString("SELECT id FROM posts WHERE id = %1 AND username = '%2';").arg(postId).arg(username);
    QSqlQuery result = getQueryResult(checkQuery);

    if (!result.next()) {
        return "error";  //пост не принадлежит пользователю или не существует
    }

    //удаляем пост
    QString deleteQuery = QString("DELETE FROM posts WHERE id = %1;").arg(postId);

    if (executeQuery(deleteQuery)) {
        return "success";
    } else {
        return "error";
    }
}
