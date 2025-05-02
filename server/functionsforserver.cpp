#include "functionsforserver.h"
#include "databasemanager.h"


//регистрация пользователя
bool regUser(const QString& username, const QString& password) {
    QByteArray result = DatabaseManager::getInstance()->reg(username, password);

    if (result == "reg+") {
        return true;
    } else {
        return false;
    }
}

//авторизация пользователя
bool logUser(const QString& username, const QString& password) {
    QByteArray result = DatabaseManager::getInstance()->auth(username, password);

    if (result == "log+") {
        return true;
    } else {
        return false;
    }
}

//создание твита
bool createPost(const QString& username, const QString& data) {
    QByteArray result = DatabaseManager::getInstance()->createPost(username, data);

    if (result == "success") {
        return true;
    } else {
        return false;
    }
}

//получение постов по пользователю
QList<QString>  getPostsByUser(const QString& username) {
    QByteArray result = DatabaseManager::getInstance()->getPostsByUser(username);
    //преобразуем результат в список
    QList<QString> posts;
    if (result.startsWith("POSTS: ")) {
        QString postData = result.mid(8); //убираем "POSTS: "
        posts = postData.split(" | ");
    }

    qDebug() << "Посты получены: " << posts;
    return posts;
}

//получение постов по тексту
QList<QString> getPostsByText(const QString &data)
{
    QByteArray result = DatabaseManager::getInstance()->getPostsByText(data);
    //преобразуем результат в список
    QList<QString> posts;
    if (result.startsWith("POSTS: ")) {
        QString postData = result.mid(8); //убираем "POSTS: "
        posts = postData.split(" | ");
    }

    qDebug() << "Посты получены: " << posts;
    return posts;

}

//удаление поста
bool deletePost(const QString& username, int postId) {
    QByteArray result = DatabaseManager::getInstance()->delPost(username, postId);

    if (result == "success") {
        return true;
    } else {
        return false;
    }
}

//все посты
QList<QString> getAllPosts() {
    QByteArray result = DatabaseManager::getInstance()->getAllPosts();

    //преобразуем результат в список
    QList<QString> feed;
    if (result.startsWith("POSTS: ")) {
        QString feedData = result.mid(7); //убираем "POSTS: "
        feed = feedData.split(" | ");
    }

    qDebug() << "Посты получены: " << feed;
    return feed;
}

//парсинг команд
QString parse(const QString& command) {
    //разбиваем команду на части по пробелам с помощью split. Qt::SkipEmptyParts пропускает пустые части, появившиеся из-за лишних пробелов
    QStringList parts = command.split(" ", Qt::SkipEmptyParts);

    //если команда пустая, возвращаем ошибку
    if (parts.isEmpty()) {
        return "error";
    }

    //Если команда не пустая, пытаемся ее считать. для этого присваиваем первый элемент списка parts объекту action
    QString action = parts[0];

    //обработка команд
    if (action == "REGISTER" && parts.size() == 3) { //КОМАНДА 1. REGISTER username password
        QString username = parts[1];
        QString password = parts[2];
        //вызов функции для регистрации пользователя
        if (regUser(username, password)) {
            return "reg+";
        } else {
            return "error";
        }
    }
    else if (action == "LOGIN" && parts.size() == 3) { //КОМАНДА 2. LOGIN username password
        QString username = parts[1];
        QString password = parts[2];
        //вызов функции для авторизации пользователя
        if (logUser(username, password)) {
            return "log+";
        } else {
            return "error";
        }
    }
    else if (action == "CREATE_POST" && parts.size() >= 3) { //КОМАНДА 3. CREATE_POST username data
        QString username = parts[1];
        QString data = parts.mid(2).join(" ");  //mid(2) возвращает подсписок элементов, начиная с индекса 2. join соединяет все в одну строку
        //вызов функции создания поста
        if (createPost(username, data)) {
            return "success";
        } else {
            return "error";
        }
    }
    else if (action == "DELETE_POST" && parts.size() == 3) {
        QString username = parts[1];
        int postId = parts[2].toInt();
        //вызов функции удаления поста
        if (deletePost(username, postId)) {
            return "success";
        } else {
            return "error";
        }
    }
    else if (action == "GET_POSTS_BY_USER" && parts.size() == 2) { //КОМАНДА 4. GET_POSTS_BY_USER username
        QString username = parts[1];
        QList<QString> posts = getPostsByUser(username); //получаем все твиты по имени пользователя через функцию getPostsByUser
        return "POSTS: " + posts.join(" | ");  //возвращаем посты через разделитель
    }
    else if (action == "GET_POSTS_BY_TEXT" && parts.size() == 2) { //КОМАНДА 5. GET_POSTS_BY_TEXT data
        QString data = parts[1];
        QList<QString> posts = getPostsByText(data); //получаем все посты по найденному тексту через функцию getPostsByText
        return "POSTS: " + posts.join(" | ");  //возвращаем посты через разделитель
    }
    else if (action == "GET_ALL_POSTS" && parts.size() == 2) { //КОМАНДА 6. GET_ALL_POSTS
        QList<QString> posts = getAllPosts(); //создаем список posts и вызываем функцию, которая возвращает все посты в базе
        return "POSTS: " + posts.join(" | ");  //возвращаем посты через разделитель
    }
    else {
        //неизвестная команда или неверное количество аргументов
        return "error";
    }
}



