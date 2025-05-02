#ifndef FUNCTIONSFORCLIENT_H
#define FUNCTIONSFORCLIENT_H
#include <QString>
#include "mainwindow.h"  // для структуры Post

struct Post;

bool auth(QString login, QString password);
bool reg(QString login, QString password, QString email);
QList<Post> getPostsByUser(const QString& username);
QList<Post> getPostsByText(const QString& text);
//QList<Post> searchPosts(const QString& query);
bool createNewPost(const QString& username, const QString& header, const QString& text);
bool deletePost(const QString& username, int postId);

#endif
