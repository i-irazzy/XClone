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

    /** @brief ID поста. */
    QString postId;
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
     * @param parent Родительский индекс (по умолчанию пуст).
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    /**
     * @brief Возвращает данные для заданного индекса и роли.
     * @param index Индекс поста.
     * @param role Роль отображения (Qt::DisplayRole).
     * @return Данные поста в виде `QString`.
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /** @brief Список постов. */
    QList<Post> posts;

signals:
    /**
     * @brief Сигнал при удалении поста.
     * @param postId ID удалённого поста.
     */
    void postDeleted(int postId);

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

    /**
     * @brief Выполняет общий поиск постов (по умолчанию загружает все посты).
     */
    void SearchPosts();

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
     * @brief Настраивает список постов и обработку кликов.
     */
    void setupPostList();

    /**
     * @brief Выполняет поиск постов по тексту.
     */
    void SearchPostsByText();

    /**
     * @brief Выполняет поиск постов по имени пользователя.
     */
    void SearchPostsByUser();

    /**
     * @brief Обрабатывает создание нового поста.
     * @param header Заголовок поста.
     * @param text Текст поста.
     */
    void onNewPostCreated(QString header, QString text);

    /**
     * @brief Слот для открытия формы создания нового поста по нажатию кнопки.
     */
    void on_pushButtonNewPost_clicked();
};

#endif // MAINWINDOW_H
