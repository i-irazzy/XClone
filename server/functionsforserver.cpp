#include "functionsforserver.h"
#include "databasemanager.h"

/// Регистрирует нового пользователя.
bool regUser(const QString& username, const QString& password, const QString& email) {
    QByteArray result = DatabaseManager::getInstance()->reg(username, password, email); ///< Используем public-метод reg()
    return result == "reg+";
}


/// Авторизует пользователя по логину и паролю.
bool logUser(const QString& username, const QString& password) {
    QByteArray result = DatabaseManager::getInstance()->auth(username, password); ///< Запрос на авторизацию
    return result == "log+";
}

/// Создаёт новый пост пользователя.
bool createPost(const QString& username, const QString& data) {
    QByteArray result = DatabaseManager::getInstance()->createPost(username, data); ///< Создаём пост в базе
    return result == "success";
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
    qDebug() << "Получен запрос регистрации: " << command;
    QStringList parts = command.split("|", Qt::SkipEmptyParts); ///< Исправляем разделитель!

    if (parts.isEmpty()) {
        return "error"; ///< Ошибка, если строка пустая
    }

    QString action = parts[0]; ///< Получаем ключевое слово команды

    if (action == "REGISTER" && parts.size() == 4) { ///< Исправляем проверку!
        return regUser(parts[1], parts[2], parts[3]) ? "reg+" : "error"; ///< Теперь передаём email
    }
    else if (action == "LOGIN" && parts.size() == 3) {
        return logUser(parts[1], parts[2]) ? "log+" : "error";
    }
    else if (action == "CREATE_POST" && parts.size() >= 3) {
        QString data = parts.mid(2).join(" ");
        return createPost(parts[1], data) ? "success" : "error";
    }
    else if (action == "DELETE_POST" && parts.size() == 3) {
        return deletePost(parts[1], parts[2].toInt()) ? "success" : "error";
    }
    else if (action == "GET_POSTS_BY_USER" && parts.size() == 2) {
        QList<QString> posts = getPostsByUser(parts[1]);
        return "POSTS: " + posts.join(" | ");
    }
    else if (action == "GET_POSTS_BY_TEXT" && parts.size() == 2) {
        QList<QString> posts = getPostsByText(parts[1]);
        return "POSTS: " + posts.join(" | ");
    }
    else if (action == "GET_ALL_POSTS") {
        QList<QString> posts = getAllPosts();
        return "POSTS: " + posts.join(" | ");
    }
    else {
        return "error";
    }
}
