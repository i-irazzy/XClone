#include "functionsforserver.h"
#include "databasemanager.h"

/// Регистрирует нового пользователя.
bool regUser(const QString& username, const QString& password) {
    QByteArray result = DatabaseManager::getInstance()->reg(username, password); ///< Запрос к базе данных на регистрацию
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

/// Интерпретирует строку команды от клиента и вызывает нужную функцию.
QString parse(const QString& command) {
    QStringList parts = command.split(" ", Qt::SkipEmptyParts); ///< Делим команду по пробелам, убирая лишние

    if (parts.isEmpty()) {
        return "error"; ///< Ошибка, если строка пустая
    }

    QString action = parts[0]; ///< Получаем ключевое слово команды

    if (action == "REGISTER" && parts.size() == 3) { ///< Команда регистрации: REGISTER username password
        return regUser(parts[1], parts[2]) ? "reg+" : "error"; ///< Возврат "reg+" при успехе, иначе "error"
    }
    else if (action == "LOGIN" && parts.size() == 3) { ///< Команда входа: LOGIN username password
        return logUser(parts[1], parts[2]) ? "log+" : "error"; ///< Возврат "log+" при успехе
    }
    else if (action == "CREATE_POST" && parts.size() >= 3) { ///< Создание поста: CREATE_POST username data...
        QString data = parts.mid(2).join(" "); ///< Склеиваем всё после username в текст поста
        return createPost(parts[1], data) ? "success" : "error"; ///< "success" при успешном создании
    }
    else if (action == "DELETE_POST" && parts.size() == 3) { ///< Удаление поста: DELETE_POST username postId
        return deletePost(parts[1], parts[2].toInt()) ? "success" : "error"; ///< Удаляем пост по ID
    }
    else if (action == "GET_POSTS_BY_USER" && parts.size() == 2) { ///< Получение постов пользователя: GET_POSTS_BY_USER username
        QList<QString> posts = getPostsByUser(parts[1]); ///< Получаем посты из БД
        return "POSTS: " + posts.join(" | "); ///< Возвращаем как строку
    }
    else if (action == "GET_POSTS_BY_TEXT" && parts.size() == 2) { ///< Поиск постов по тексту: GET_POSTS_BY_TEXT word
        QList<QString> posts = getPostsByText(parts[1]); ///< Ищем совпадения
        return "POSTS: " + posts.join(" | "); ///< Возвращаем результат
    }
    else if (action == "GET_ALL_POSTS" && parts.size() == 2) { ///< Получить все посты: GET_ALL_POSTS any_word
        QList<QString> posts = getAllPosts(); ///< Получаем все посты
        return "POSTS: " + posts.join(" | "); ///< Возврат в виде строки
    }
    else {
        return "error"; ///< Неизвестная команда или неправильное количество аргументов
    }
}

