#include "functionsforclient.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clientapi.h"
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

    if (role == Qt::UserRole) {  // ✔️ Теперь возвращаем `int`
        return post.postId.toInt();
    }


    if (role == Qt::DisplayRole && post.username == currentUsername) {
        return QString("👤 %1\n📌 %2\n%3\n🗑 %4").arg(post.username, post.header, post.text, "Удалить пост");
    }

    return QVariant();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    model(new PostModel(this)),  // Модель списка постов
    newPostForm(new NewPostForm(this)),  // Форма создания поста
    client(ClientAPI::getInstance())  // API клиента
{
    ui->setupUi(this);

    // Настраиваем список постов
    setupPostList();

    // Подключаем сигналы и слоты
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::SearchPostsByText);
    connect(ui->searchButton_2, &QPushButton::clicked, this, &MainWindow::SearchPostsByUser);
    connect(newPostForm, &NewPostForm::newPostCreated, this, &MainWindow::onNewPostCreated);
    connect(ui->pushButtonNewPost, &QPushButton::clicked, this, &MainWindow::on_pushButtonNewPost_clicked);
}


MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setCurrentUser(const QString &username) {
    currentUser = username;
}

void MainWindow::setupPostList() {
    ui->postList->setModel(model);
    // ui->postList->setItemDelegate(new PostDelegate(this));  // ✔️ Добавляем кастомный делегат

    // Обрабатываем клики по постам
    connect(ui->postList, &QListView::pressed, this, [&](const QModelIndex &index) {
        if (!index.isValid()) return;

        if (QGuiApplication::mouseButtons() == Qt::RightButton) {  // Проверяем ПКМ
            Post post = model->posts.at(index.row());

            if (post.username == currentUsername) {  // Показываем меню только для своих постов
                QMenu contextMenu;
                QAction *deleteAction = contextMenu.addAction("Удалить пост");

                connect(deleteAction, &QAction::triggered, this, [=]() {
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::question(this, "Удаление поста",
                                                  "Вы хотите удалить этот пост?",
                                                  QMessageBox::Yes | QMessageBox::No);
                    if (reply == QMessageBox::Yes) {
                        if (deletePost(post.postId.toInt())) {
                            QMessageBox::information(this, "Готово!", "Пост успешно удалён.");
                            SearchPosts();  // Обновляем список
                        } else {
                            QMessageBox::warning(this, "Ошибка", "Не удалось удалить пост.");
                        }
                    }
                });

                contextMenu.exec(QCursor::pos());
            }
        }
    });
}



void MainWindow::SearchPosts() {
    QString query = ui->searchInput->text().trimmed();

    if (query.isEmpty()) {
        query = "GET_ALL_POSTS";
    }

    qDebug() << "Отправляем запрос на сервер: " << query;
    QByteArray response = client->query_to_server(query);
    qDebug() << "Ответ сервера: " << response.trimmed();

    QList<Post> filtered;
    QList<QByteArray> lines = response.split('\n');

    for (const QByteArray &line : lines) {
        QList<QByteArray> parts = line.split('|');  // Формат: id|username|header|text
        if (parts.size() == 4) {
            Post post;
            post.postId   = parts[0];  // ID поста (не показываем)
            post.username = QString::fromUtf8(parts[1]);
            post.header   = QString::fromUtf8(parts[2]);
            post.text     = QString::fromUtf8(parts[3]);

            filtered.append(post);
        }
    }

    qDebug() << "Обновлено постов: " << filtered.size();
    model->setPosts(filtered);
}



void MainWindow::SearchPostsByText() {
    QString searchQuery = ui->searchInput->text().trimmed();

    if (searchQuery.isEmpty()) {
        qDebug() << "Поле поиска пусто, отменяем поиск.";
        return;
    }

    qDebug() << "Поиск постов по тексту: " << searchQuery;
    QList<Post> filteredPosts = getPostsByText(searchQuery);

    qDebug() << "Найдено постов: " << filteredPosts.size();
    model->setPosts(filteredPosts);
}

void MainWindow::SearchPostsByUser() {
    QString usernameQuery = ui->searchInput->text().trimmed();

    if (usernameQuery.isEmpty()) {
        qDebug() << "Поле поиска пусто, отменяем поиск.";
        return;
    }

    qDebug() << "Поиск постов по пользователю: " << usernameQuery;
    QList<Post> filteredPosts = getPostsByUser(usernameQuery);

    qDebug() << "Найдено постов: " << filteredPosts.size();
    model->setPosts(filteredPosts);
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
    if (createNewPost(header, text)) {
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
