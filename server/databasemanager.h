#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QByteArray>


/// Класс-уничтожитель для управления временем жизни единственного экземпляра DatabaseManager
class DatabaseManagerDestroyer;

/**
 * @brief Класс DatabaseManager реализует шаблон Singleton
 * для управления подключением и работой с базой данных.
 */
class DatabaseManager
{
private:
    /**
     * @brief Указатель на единственный экземпляр класса.
     */
    static DatabaseManager* p_instance;

    /**
     * @brief Объект-уничтожитель для корректного удаления экземпляра.
     */
    static DatabaseManagerDestroyer destroyer;

    /**
     * @brief Объект базы данных.
     */
    QSqlDatabase db;

    /**
     * @brief Приватный конструктор.
     */
    DatabaseManager();

    /**
     * @brief Удаляем копирующий конструктор.
     */
    DatabaseManager(const DatabaseManager&) = delete;

    /**
     * @brief Удаляем оператор присваивания.
     */
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    /**
     * @brief Деструктор.
     */
    ~DatabaseManager();

    friend class DatabaseManagerDestroyer;

    /**
     * @brief Подключение к базе данных.
     * @param dbPath Путь к файлу базы данных.
     * @return true при успешном подключении.
     */
    bool connect(const QString& dbPath);

    /**
     * @brief Отключение от базы данных.
     */
    void disconnect();

    /**
     * @brief Выполнение SQL-запроса.
     * @param query Строка SQL-запроса.
     * @return true при успешном выполнении.
     */
    bool executeQuery(const QString& query);

    /**
     * @brief Получение результата SQL-запроса.
     * @param query Строка SQL-запроса.
     * @return Объект QSqlQuery с результатами.
     */
    QSqlQuery getQueryResult(const QString& query);

public:
    /**
     * @brief Получение единственного экземпляра DatabaseManager.
     * @return Указатель на экземпляр DatabaseManager
     */
    static DatabaseManager* getInstance();

    /**
     * @brief Инициализация базы данных и создание таблиц.
     * @param dbPath Путь к файлу базы данных
     * @return true при успешной инициализации
     */
    bool initializeDatabase(const QString& dbPath);

    /**
     * @brief Авторизация пользователя.
     * @param log Логин.
     * @param pass Пароль.
     * @return Результат авторизации в виде QByteArray.
     */
    QByteArray auth(const QString& log, const QString& pass);

    /**
     * @brief Регистрация нового пользователя.
     * @param log Логин.
     * @param pass Пароль.
     * @return Результат регистрации в виде QByteArray.
     */
    QByteArray reg(const QString& log, const QString& pass);

    /**
     * @brief Получение постов пользователя.
     * @param log Логин пользователя.
     * @return Список постов в виде QByteArray.
     */
    QByteArray getPostsByUser(const QString& log);

    /**
     * @brief Поиск постов по ключевому слову.
     * @param text Ключевое слово.
     * @return Найденные посты в виде QByteArray.
     */
    QByteArray getPostsByText(const QString& text);

    /**
     * @brief Создание нового поста.
     * @param log Логин пользователя.
     * @param post Текст поста.
     * @return Результат создания поста.
     */
    QByteArray createPost(const QString& log, const QString& post);

    /**
    * @brief Получение всех постов.
    * @return Все посты в виде QByteArray.
    */
   QByteArray getAllPosts();

   /**
    * @brief Удаление поста.
    * @param username Логин пользователя.
    * @param postId ID поста.
    * @return Результат удаления.
    */
   QByteArray delPost(const QString& username, int postId);

};

/**
 * @brief Класс-уничтожитель для DatabaseManager.
 * Используется для безопасного удаления Singleton-объекта.
 */
class DatabaseManagerDestroyer
{
private:
    /**
     * @brief Указатель на экземпляр DatabaseManager.
     */
    DatabaseManager* p_instance;

public:
    /**
     * @brief Деструктор, который удаляет экземпляр.
     */
    ~DatabaseManagerDestroyer() { delete p_instance; }

    /**
     * @brief Инициализация экземпляра DatabaseManager.
     * @param p Указатель на DatabaseManager.
     */
    void initialize(DatabaseManager* p) { p_instance = p; }

};

#endif // DATABASEMANAGER_H
