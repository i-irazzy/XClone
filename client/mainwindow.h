#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QListView>
#include <QAbstractListModel>
#include <QString>
#include <QMessageBox>
#include <QVBoxLayout>    // Добавлен для управления компоновкой элементов
#include <QHBoxLayout>    // Добавлен для горизонтальных элементов
#include "newpostform.h"
#include "clientapi.h"
// #include "functionsforclient.h"

namespace Ui {
class MainWindow;
}

/**
 * @brief Структура, описывающая пост.
 */
struct Post {
    /** @brief Имя пользователя, создавшего пост. */
    QString username;

    /** @brief Заголовок поста. */
    QString header;

    /** @brief Текст поста. */
    QString text;
};

/**
 * @brief Модель данных для отображения списка постов.
 *
 * Реализует интерфейс QAbstractListModel для использования в QListView.
 */
class PostModel : public QAbstractListModel {
    Q_OBJECT
public:
    /**
     * @brief Конструктор модели постов.
     * @param parent Родительский объект.
     */
    explicit PostModel(QObject *parent = nullptr);

    /**
     * @brief Устанавливает новые данные постов.
     * @param newPosts Новый список постов.
     */
    void setPosts(const QList<Post> &newPosts);

    /**
     * @brief Возвращает количество строк в модели.
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    /**
     * @brief Возвращает данные для заданного индекса и роли.
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    /** @brief Список постов. */
    QList<Post> posts;
};

/**
 * @brief Главное окно приложения.
 *
 * Обеспечивает пользовательский интерфейс для поиска и создания постов.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @brief Конструктор главного окна.
     * @param parent Родительский виджет.
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Деструктор главного окна.
     */
    ~MainWindow();

    /**
     * @brief Устанавливает имя текущего пользователя.
     * @param username Имя пользователя.
     */
    void setCurrentUser(const QString &username);

    /**
     * @brief Показывает форму создания нового поста.
     */
    void showNewPostForm();

private:
    /** @brief Интерфейс, сгенерированный Qt Designer. */
    Ui::MainWindow *ui;

    /** @brief Модель данных для списка постов. */
    PostModel *model;

    /** @brief Все полученные посты. */
    QList<Post> allPosts;

    /** @brief Форма для создания новых постов. */
    NewPostForm *newPostForm;

    /** @brief Текущий пользователь. */
    QString currentUser;

    /** @brief Экземпляр API клиента для общения с сервером. */
    ClientAPI *client;

    /** @brief Главный вертикальный компоновщик. */
    QVBoxLayout *mainLayout;

    /** @brief Горизонтальный компоновщик для кнопок. */
    QHBoxLayout *buttonLayout;

private slots:
    /**
     * @brief Выполняет поиск постов по запросу пользователя.
     */
    void SearchPosts();

    /**
     * @brief Обрабатывает создание нового поста.
     * @param username Имя пользователя.
     * @param header Заголовок поста.
     * @param text Текст поста.
     */
    void onNewPostCreated(QString username, QString header, QString text);

    /**
     * @brief Слот для открытия формы создания нового поста по нажатию кнопки.
     */
    void on_pushButtonNewPost_clicked();

};

#endif // MAINWINDOW_H
