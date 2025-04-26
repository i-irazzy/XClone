#include "functionsforserver.h"
#include"databasemanager.h"


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
bool createTweet(const QString& username, const QString& tweet) {
    QByteArray result = DatabaseManager::getInstance()->postTweet(username, tweet);

    if (result == "succes") {
        return true;
    } else {
        return false;
    }
}

//просмотр твитов пользователя
QList<QString> getTweets(const QString& username) {
    QByteArray result = DatabaseManager::getInstance()->getposts(username, ""); //пароль не нужен для получения твитов

    //преобразуем результат в список твитов
    QList<QString> tweets;
    if (result.startsWith("TWEETS: ")) {
        QString tweetData = result.mid(8); //убираем "TWEETS: "
        tweets = tweetData.split(" | ");
    }

    qDebug() << "Tweets retrieved:" << tweets;
    return tweets;
}

//удаление твита
bool delTweet(const QString& username, int tweetId) {
    QByteArray result = DatabaseManager::getInstance()->delTweet(username, tweetId);

    if (result == "success") {
        return true;
    } else {
        return false;
    }
}

//лента твитов (твиты от пользователей, на которых подписан текущий пользователь)
QList<QString> getAllTweets(const QString& username) {
    QByteArray result = DatabaseManager::getInstance()->getFeed(username);

    //преобразуем результат в список твитов
    QList<QString> feed;
    if (result.startsWith("POSTS: ")) {
        QString feedData = result.mid(7); //убираем "POSTS: "
        feed = feedData.split(" | ");
    }

    qDebug() << "Feed retrieved:" << feed;
    return feed;
}

//подписка на пользователя
bool followUser(const QString& follower, const QString& followee) {
    QByteArray result = DatabaseManager::getInstance()->followUser(follower, followee);

    if (result == "success") {
        return true;
    } else {
        return false;
    }
}

//парсинг команд
QString parse(const QString& command) {
    //разбиваем команду на части по пробелам с помощью split. Qt::SkipEmptyParts пропускает пустые части, появившиеся из-за лишних пробелов
    QStringList parts = command.split(" ", Qt::SkipEmptyParts);

    //если команда пустая, возвращаем ошибку
    if (parts.isEmpty()) {
        return "ERROR";
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
    else if (action == "POST_TWEET" && parts.size() >= 3) { //КОМАНДА 3. POST_TWEET username "tweet text"
        QString username = parts[1];
        QString tweet = parts.mid(2).join(" ");  //mid(2) возвращает подсписок элементов, начиная с индекса 2. join соединяет все в одну строку
        //вызов функции создания поста
        if (createTweet(username, tweet)) {
            return "succes";
        } else {
            return "error";
        }
    }
    else if (action == "GET_TWEETS" && parts.size() == 2) { //КОМАНДА 4. GET_TWEETS username
        QString username = parts[1];
        QList<QString> tweets = getTweets(username); //получаем все твиты по имени пользователя через функцию getTweets
        return "TWEETS: " + tweets.join(" | ");  //возвращаем твиты через разделитель
    }

    else if (action == "FOLLOW" && parts.size() == 3) { //КОМАНДА 5. FOLLOW follower followee
        QString follower = parts[1];
        QString followee = parts[2];
        //вызов функции
        if (followUser(follower, followee)) {
            return "success";
        } else {
            return "error";
        }
    }
    else if (action == "GET_POSTS" && parts.size() == 2) { //КОМАНДА 6. GET_POSTS username
        QString username = parts[1];
        QList<QString> post = getAllTweets(username); //создаем список post и вызываем функцию, которая возвращает все посты пользователя.
        return "POSTS: " + post.join(" | ");  // Возвращаем ленту через разделитель
    }
    else {
        //неизвестная команда или неверное количество аргументов
        return "error";
    }
}

