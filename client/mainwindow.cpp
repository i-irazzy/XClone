#include "functionsforclient.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

PostModel::PostModel(QObject *parent) : QAbstractListModel(parent) {}

void PostModel::setPosts(const QList<Post> &newPosts) {
    beginResetModel();
    posts = newPosts;
    endResetModel();
}

int PostModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return posts.size();
}

QVariant PostModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= posts.size()) return QVariant();

    const Post &post = posts.at(index.row());

    if (role == Qt::DisplayRole) {
        return QString("👤 %1\n📌 %2\n%3").arg(post.username, post.header, post.text);
    }

    return QVariant();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    model(new PostModel(this)),
    newPostForm(new NewPostForm(this)),
    client(ClientAPI::getInstance())
{
    ui->setupUi(this);

    ui->postList->setModel(model);

    // Сигналы и слоты
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::SearchPosts);
    connect(newPostForm, &NewPostForm::newPostCreated, this, &MainWindow::onNewPostCreated);
    connect(ui->pushButtonNewPost, &QPushButton::clicked, this, &MainWindow::on_pushButtonNewPost_clicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setCurrentUser(const QString &username) {
    currentUser = username;
}

void MainWindow::SearchPosts() {
    QString query = ui->searchInput->text().trimmed();

    // Если поиск пуст, запрашиваем все посты текущего пользователя
    if (query.isEmpty() && !currentUsername.isEmpty()) {
        query = QString("GET_POSTS_BY_USER|%1").arg(currentUsername);
    }

    qDebug() << "Запрос на сервер для поиска постов: " << query;

    // Отправляем запрос на сервер через ClientAPI
    QByteArray response = client->query_to_server(query);
    qDebug() << "Ответ сервера: " << response.trimmed();

    QList<Post> filtered;
    QList<QByteArray> lines = response.split('\n');

    for (const QByteArray &line : lines) {
        QList<QByteArray> parts = line.split('|');  // Формат: username|header|text
        if (parts.size() == 3) {
            Post post;
            post.username = QString::fromUtf8(parts[0]);
            post.header   = QString::fromUtf8(parts[1]);
            post.text     = QString::fromUtf8(parts[2]);

            qDebug() << "Добавляем пост: " << post.username << " | " << post.header << " | " << post.text;
            filtered.append(post);
        }
    }

    model->setPosts(filtered);
}

void MainWindow::on_pushButtonNewPost_clicked() {
    newPostForm->setCurrentUser(currentUser);

    // Добавляем корректное отображение формы
    newPostForm->setWindowFlags(Qt::Window);  // Делаем виджет отдельным окном
    newPostForm->show();
    newPostForm->raise();
    newPostForm->activateWindow();
}

void MainWindow::onNewPostCreated(QString header, QString text) {
    if (createNewPost(currentUsername, header, text)) {
        SearchPosts();  // Обновим посты с сервера
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось создать новый пост.");
    }
}

void MainWindow::showNewPostForm() {
    newPostForm->setCurrentUser(currentUser);

    // Корректируем отображение формы
    newPostForm->setWindowFlags(Qt::Window);
    newPostForm->show();
    newPostForm->raise();
    newPostForm->activateWindow();
}
