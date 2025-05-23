#include "functionsforclient.h"
#include "clientapi.h"

bool auth(QString login, QString password) {
   // if(login == "user" && password == "123"){
   //     return true;
   // }
   // return false;
    QString request = QString("AUTH|%1|%2").arg(login, password);
    QByteArray response = ClientAPI::getInstance()->query_to_server(request);
    return response.trimmed() == "OK";
}

bool reg(QString login, QString password, QString email) {
   // if(login != "user" && password != "123"){
   //     return true;
   // }
   // return false;
    QString request = QString("REG|%1|%2|%3").arg(login, password, email);
        QByteArray response = ClientAPI::getInstance()->query_to_server(request);
        return response.trimmed() == "OK";
}

bool createNewPost(const QString& username, const QString& header, const QString& text) {
    QString request = QString("CREATE|%1|%2|%3").arg(username, header, text);
    QByteArray response = ClientAPI::getInstance()->query_to_server(request);
    return response.trimmed() == "OK";  // Сервер должен вернуть "OK"
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
