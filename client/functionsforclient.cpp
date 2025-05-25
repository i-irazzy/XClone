#include "functionsforclient.h"
#include "clientapi.h"

QString currentUsername = "";


bool auth(QString login, QString password) {
    QString request = QString("AUTH|%1|%2").arg(login, password);
    QByteArray response = ClientAPI::getInstance()->query_to_server(request);

    if (response.trimmed() == "log+") {
        currentUsername = login;  // Сохраняем имя пользователя
        qDebug() << "Авторизация успешна! currentUsername=" << currentUsername;
        return true;
    }

    qDebug() << "Ошибка авторизации!";
    return false;
}

bool reg(QString login, QString password, QString email) {
    QString request = QString("REG|%1|%2|%3").arg(login, password, email);
    QByteArray response = ClientAPI::getInstance()->query_to_server(request);
ClientAPI::getInstance() ->query_to_server(response);
    if (response.trimmed() == "reg+") {
        currentUsername = login;  // Сохраняем имя нового пользователя
        qDebug() << "Регистрация успешна! currentUsername=" << currentUsername;
        return true;
    }

    qDebug() << "Ошибка регистрации!";
    return false;
}

bool createNewPost(const QString& currentUsername, const QString& header, const QString& text) {
    if (currentUsername.isEmpty()) {

        return false;
    }

    QString request = QString("CREATE_POST|%1|%2|%3").arg(currentUsername, header, text);


    QByteArray response = ClientAPI::getInstance()->query_to_server(request);
    return response.trimmed() == "OK";
}


QList<Post> getPostsByUser(const QString &username)
{
    QList<Post> result;
    QByteArray response = ClientAPI::getInstance()->query_to_server("GET_POSTS_BY_USER|" + username);
    QList<QByteArray> lines = response.split('\n');
    for (const QByteArray& line : lines) {
        QList<QByteArray> parts = line.split('|');
        if (parts.size() == 3) {
            Post post;
            post.username = QString::fromUtf8(parts[0]);
            post.header   = QString::fromUtf8(parts[1]);
            post.text     = QString::fromUtf8(parts[2]);
            result.append(post);
        }
    }
    return result;

}

QList<Post> getPostsByText(const QString &text)
{
    QList<Post> result;
    QByteArray response = ClientAPI::getInstance()->query_to_server("GET_POSTS_BY_TEXT|" + text);
    QList<QByteArray> lines = response.split('\n');
    for (const QByteArray& line : lines) {
        QList<QByteArray> parts = line.split('|');
        if (parts.size() == 3) {
            Post post;
            post.username = QString::fromUtf8(parts[0]);
            post.header   = QString::fromUtf8(parts[1]);
            post.text     = QString::fromUtf8(parts[2]);
            result.append(post);
        }
    }
    return result;
}

bool deletePost(const QString &username, int postId)
{
    QByteArray response = ClientAPI::getInstance()->query_to_server(
        "DELETE_POST|" + username + "|" + QString::number(postId)
    );
    return QString::fromUtf8(response).trimmed() == "OK";
}
