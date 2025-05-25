#include "functionsforserver.h"
#include "databasemanager.h"

/// Регистрирует нового пользователя.
bool regUser(const QString& username, const QString& password, const QString& email) {
    QByteArray result = DatabaseManager::getInstance()->reg(username, password, email); ///< Используем public-метод reg()
    return result == "reg+";
}


/// Авторизует пользователя по логину и паролю.
bool logUser(const QString& username, const QString& password) {
    QByteArray result = DatabaseManager::getInstance()->auth(username, password);
    qDebug() << "Ответ от auth(): " << result;

    return result.trimmed() == "log+";
}


bool createPost(const QString& username, const QString& header, const QString& text) {
    if (username.isEmpty() || header.isEmpty() || text.isEmpty()) {
        qDebug() << "Ошибка: один из параметров createPost() пуст!";
        return false;
    }

    QString query = QString("INSERT INTO posts (username, header, post) VALUES ('%1', '%2', '%3');")
                        .arg(username, header, text);

    qDebug() << "Выполняем SQL-запрос на создание поста: " << query;
    return DatabaseManager::getInstance()->executeQuery(query);
}

/// Возвращает список всех постов указанного пользователя.
QList<QString> getPostsByUser(const QString& username) {
    QByteArray result = DatabaseManager::getInstance()->getPostsByUser(username); ///< Получаем посты из базы

    QList<QString> posts;
    if (result.startsWith("POSTS: ")) {
        QString postData = result.mid(8); ///< Убираем префикс "POSTS: "
        posts = postData.split(" | "); ///< Разделяем строку на отдельные посты
    }

    qDebug() << "Посты получены:" << posts;
    return posts;
}

/// Возвращает посты, содержащие указанный текст.
QList<QString> getPostsByText(const QString& data) {
    QByteArray result = DatabaseManager::getInstance()->getPostsByText(data); ///< Поиск по тексту

    QList<QString> posts;
    if (result.startsWith("POSTS: ")) {
        QString postData = result.mid(8); ///< Убираем префикс
        posts = postData.split(" | "); ///< Делим на посты
    }

    qDebug() << "Посты получены:" << posts;
    return posts;
}

/// Удаляет пост пользователя по его идентификатору.
bool deletePost(const QString& username, int postId) {
    QByteArray result = DatabaseManager::getInstance()->delPost(username, postId); ///< Запрос на удаление
    return result == "success";
}

/// Возвращает список всех постов из базы данных.
QList<QString> getAllPosts() {
    QByteArray result = DatabaseManager::getInstance()->getAllPosts(); ///< Запрос на получение всех постов

    QList<QString> feed;
    if (result.startsWith("POSTS: ")) {
        QString feedData = result.mid(7); ///< Убираем префикс
        feed = feedData.split(" | "); ///< Делим строку на посты
    }

    qDebug() << "Посты получены:" << feed;
    return feed;
}

QString parse(const QString& command) {
    qDebug() << "Получен запрос: " << command;

    // Проверяем, не пустой ли запрос
    if (command.trimmed().isEmpty()) {
        qDebug() << "Ошибка: пустой запрос!";
        return "error";
    }

    // Разбиваем команду по `|`
    QStringList parts = command.split('|');
    if (parts.isEmpty()) {
        qDebug() << "Ошибка: команда не распознана!";
        return "error";
    }

    QString action = parts.first();
    qDebug() << "Действие: " << action;

    if (action == "REG" && parts.size() == 4) {
        QString username = parts[1];
        QString password = parts[2];
        QString email = parts[3];

        qDebug() << "Попытка регистрации: username=" << username << ", email=" << email;
        return regUser(username, password, email) ? "reg+" : "error";
    }

    else if (action == "AUTH" && parts.size() == 3) {
        QString username = parts[1];
        QString password = parts[2];

        qDebug() << "Попытка входа: username=" << username;
        return logUser(username, password) ? "log+" : "error";
    }

    else if (action == "CREATE_POST" && parts.size() >= 3) {
        QString username = parts[1];  // НЕ currentUsername!
        QString header = parts[2];
        QString text = parts[3];

        qDebug() << "Создание поста: username=" << username << ", header=" << header;
        bool result = createPost(username, header, text);

        qDebug() << "Результат создания поста: " << (result ? "success" : "error");
        return result ? "success" : "error";
    }

    else if (action == "DELETE_POST" && parts.size() == 3) {
        QString username = parts[1];  // НЕ currentUsername!
        int postId = parts[2].toInt();

        qDebug() << "Удаление поста: username=" << username << ", postId=" << postId;
        return deletePost(username, postId) ? "success" : "error";
    }

    else if (action == "GET_POSTS_BY_USER" && parts.size() == 2) {
        QString username = parts[1];  // НЕ currentUsername!

        qDebug() << "Запрос постов пользователя: username=" << username;
        QList<QString> posts = getPostsByUser(username);
        return posts.isEmpty() ? "error" : "POSTS: " + posts.join(" | ");
    }

    else if (action == "GET_POSTS_BY_TEXT" && parts.size() == 2) {
        QString searchText = parts[1];

        qDebug() << "Поиск постов по тексту: " << searchText;
        QList<QString> posts = getPostsByText(searchText);
        return posts.isEmpty() ? "error" : "POSTS: " + posts.join(" | ");
    }

    else if (action == "GET_ALL_POSTS") {
        qDebug() << "Запрос всех постов";
        QList<QString> posts = getAllPosts();
        return posts.isEmpty() ? "error" : "POSTS: " + posts.join(" | ");
    }

    qDebug() << "Ошибка: неизвестная команда -> " << action;
    return "unknown_command";
}
