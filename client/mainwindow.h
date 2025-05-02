#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QListView>
#include <QAbstractListModel>
#include <QString>
#include "newpostform.h"
#include "clientapi.h"
#include "functionsforclient.h"

namespace Ui {
class MainWindow;
}

// Структура поста
struct Post {
    QString username;
    QString header;
    QString text;
};



class PostModel : public QAbstractListModel {
    Q_OBJECT
public:
    explicit PostModel(QObject *parent = nullptr);
    void setPosts(const QList<Post> &newPosts);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole ) const override;

private:
    QList<Post> posts;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setCurrentUser(const QString &username);

private:
    Ui::MainWindow *ui;
    PostModel *model;
    QList<Post> allPosts;
    NewPostForm *newPostForm;
    QString currentUser;
    ClientAPI *client;

private slots:
    void SearchPosts();
    void showNewPostForm();
    void onNewPostCreated(QString username, QString header, QString text);
    void on_pushButtonNewPost_clicked();
};

#endif // MAINWINDOW_H
