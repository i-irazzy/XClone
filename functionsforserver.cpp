#include "functionsforserver.h"

FunctionsForServer::FunctionsForServer(QObject *parent) : QObject(parent) {
    //инизиализация заглушек
    users.append(qMakePair("user1", "password1")); //добавляем в список users с помощью функции qmakepair пару логин/пароль. Впоследствии это будет
    //информация о реальном пользователе

    tweets.append(qMakePair("user1", "Hello, world!")); //добавляем в список tweets пару логин/пост.Список будет содержать посты пользователей.

    follows.append(qMakePair("user1", "user2")); //добавляем в список пару подписчик/на кого подписался.
}

//регистрация пользователя
bool FunctionsForServer::regUser(const QString& username, const QString& password) {
    // Проверяем, существует ли уже пользователь с таким логином
    for (auto it = users.begin(); it != users.end(); ++it) { //auto it создает итератор на начало списка users. идем в цикле до конца списка.
        const auto& user = *it;  //получаем элемент через итератор
        if (user.first == username) {
            qDebug() << "User" << username << "already exists!";
            return false;
        }
    }

    //добавляем нового пользователя в заглушку
    users.append(qMakePair(username, password));
    qDebug() << "User" << username << "registered successfully!";
    return true;
}

//авторизация пользователя
bool FunctionsForServer::logUser(const QString& username, const QString& password) {
    //проверяем, есть ли пользователь с таким логином и паролем
    for (auto it = users.begin(); it != users.end(); ++it) {
        const auto& user = *it;  //получаем элемент через итератор
        if (user.first == username && user.second == password) {
            qDebug() << "User" << username << "logged in successfully!";
            return true;
        }
    }

    qDebug() << "Invalid username or password!";
    return false;
}

//создание твита
bool FunctionsForServer::createTweet(const QString& username, const QString& tweet) {
    //проверяем, существует ли пользователь
    bool userExists = false;
    for (auto it = users.begin(); it != users.end(); ++it) {
        const auto& user = *it;  // Получаем элемент через итератор
        if (user.first == username) {
            userExists = true;
            break;
        }
    }

    if (!userExists) {
        qDebug() << "User" << username << "doesn't exist!";
        return false;
    }

    //добавляем твит в заглушку
    tweets.append(qMakePair(username, tweet));
    qDebug() << "Tweet by" << username << "created:" << tweet;
    return true;
}

//просмотр твитов пользователя
QList<QString> FunctionsForServer::getTweets(const QString& username) {
    QList<QString> userTweets; //создание списка для хранения постов пользователя

    //ищем все твиты пользователя
    for (auto it = tweets.begin(); it != tweets.end(); ++it) {
        const auto& tweet = *it;  //получаем элемент через итератор
        if (tweet.first == username) { //если в tweet поле first совпадает с введенным ником, мы добавляем пост в список userTweets
            userTweets.append(tweet.second);
        }
    }

    qDebug() << "Tweets by" << username << ":" << userTweets;
    return userTweets;
}

//удаление твита
bool FunctionsForServer::delTweet(const QString& username, int tweetId) {
    //проверяем, существует ли твит с таким ID
    // tweetId должен быть в допустимом диапазоне: от 0 до размера списка tweets
    if (tweetId >= 0 && tweetId < tweets.size()) {
        //проверяем, принадлежит ли твит указанному пользователю
         // tweets[tweetId].first — это логин пользователя, который создал твит
        if (tweets[tweetId].first == username) {
            qDebug() << "Tweet" << tweetId << "by" << username << "deleted:" << tweets[tweetId].second;
            tweets.removeAt(tweetId);
            return true;
        }
    }

    qDebug() << "Failed to delete tweet" << tweetId << "by" << username;
    return false;
}

//лента твитов (твиты от пользователей, на которых подписан текущий пользователь)
QList<QString> FunctionsForServer::getAllTweets(const QString& username) {
    QList<QString> posts;  //список для хранения ленты твитов

    //ищем всех пользователей, на которых подписан текущий пользователь
    for (auto followIt = follows.begin(); followIt != follows.end(); ++followIt) {
        const auto& follow = *followIt;  //получаем текущую подписку через итератор

        //проверяем, совпадает ли подписчик с указанным пользователем
        if (follow.first == username) {
            QString follower = follow.second;  //получаем логин пользователя, на которого подписан текущий пользователь

            // Ищем все твиты этого пользователя follower
            for (auto tweetIt = tweets.begin(); tweetIt != tweets.end(); ++tweetIt) {
                const auto& tweet = *tweetIt;  //получаем текущий твит через итератор

                // Проверяем, принадлежит ли твит пользователю follower
                if (tweet.first == follower) {
                    posts.append(tweet.second);  // Добавляем текст твита в ленту
                }
            }
        }
    }

    // Выводим ленту твитов
    qDebug() << "Posts for " << username << ":" << posts;

    // Возвращаем ленту твитов
    return posts;
}

// Подписка на пользователя
bool FunctionsForServer::followUser(const QString& follower, const QString& followee) {
    // Проверяем, существует ли пользователь, на которого подписываются
    bool followeeExists = false; //заводим для этого переменную которая по умолчанию false

    //обходим список пользователей с помощью итератора, начиная с начала и двигая его до конца
    for (auto userIt = users.begin(); userIt != users.end(); ++userIt) {
        const auto& user = *userIt;  //получаем текущего пользователя через итератор

        // Проверяем, совпадает ли логин пользователя с followee
        if (user.first == followee) {
            followeeExists = true;  // Пользователь найден
            break;  // Выходим из цикла
        }
    }

    // Если пользователь не найден, выводим сообщение и возвращаем false
    if (!followeeExists) {
        qDebug() << "User" << followee << "does not exist!";
        return false;
    }

    // Добавляем подписку в список follows
    follows.append(qMakePair(follower, followee));

    // Выводим сообщение об успешной подписке
    qDebug() << "User" << follower << "followed" << followee;

    // Возвращаем true, чтобы указать, что подписка прошла успешно
    return true;
}

QString FunctionsForServer::parse(const QString& command) {
    //разбиваем команду на части по пробелам с помощью split. Qt::SkipEmptyParts пропускает пустые части, появившиеся из-за лишних пробелов
    QStringList parts = command.split(" ", Qt::SkipEmptyParts);

    //если команда пустая, возвращаем ошибку
    if (parts.isEmpty()) {
        return "ERROR: Empty command";
    }

    //Если команда не пустая, пытаемся ее считать. для этого присваиваем первый элемент списка parts объекту action
    QString action = parts[0];

    //обработка команд
    if (action == "REGISTER" && parts.size() == 3) { //КОМАНДА 1. REGISTER username password
        QString username = parts[1];
        QString password = parts[2];
        //вызов функции для регистрации пользователя
        if (regUser(username, password)) {
            return "SUCCESS: User registered";
        } else {
            return "ERROR: User already exists"; //
        }
    }
    else if (action == "LOGIN" && parts.size() == 3) { //КОМАНДА 2. LOGIN username password
        QString username = parts[1];
        QString password = parts[2];
        //вызов функции для авторизации пользователя
        if (logUser(username, password)) {
            return "SUCCESS: User logged in";
        } else {
            return "ERROR: Invalid username or password";
        }
    }
    else if (action == "POST_TWEET" && parts.size() >= 3) { //КОМАНДА 3. POST_TWEET username "tweet text"
        QString username = parts[1];
        QString tweet = parts.mid(2).join(" ");  //mid(2) возвращает подсписок элементов, начиная с индекса 2. join соединяет все в одну строку
        //вызов функции создания поста
        if (createTweet(username, tweet)) {
            return "SUCCESS: Tweet posted";
        } else {
            return "ERROR: User does not exist";
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
            return "SUCCESS: Followed user";
        } else {
            return "ERROR: User to follow does not exist";
        }
    }
    else if (action == "GET_POSTS" && parts.size() == 2) { //КОМАНДА 6. GET_POSTS username
        QString username = parts[1];
        QList<QString> post = getAllTweets(username); //создаем список post и вызываем функцию, которая возвращает все посты пользователя.
        return "POSTS: " + post.join(" | ");  // Возвращаем ленту через разделитель
    }
    else {
        //неизвестная команда или неверное количество аргументов
        return "ERROR: Invalid command";
    }
}

