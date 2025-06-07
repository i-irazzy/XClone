#ifndef TCPSERVER_H
#define TCPSERVER_H

#include<QTcpServer>
#include<QObject>
#include<QTcpSocket>
#include<QByteArray>
#include<QDebug>
#include<QList>

/**
 * @brief Класс TcpServer реализует простой TCP-сервер.
 *
 * Сервер принимает входящие подключения, читает данные от клиентов
 * и отправляет им обработанные ответы.
 */
class TcpServer : public QObject
{
    Q_OBJECT

public:
    /**
    * @brief Конструктор класса TcpServer.
    * @param parent Родительский объект (по умолчанию nullptr).
    */
    explicit TcpServer(QObject *parent = nullptr);

    /**
     * @brief Деструктор класса TcpServer.
     */
    ~TcpServer();

    /**
     * @brief Запускает сервер на порту 33333.
     */
    void startServer();

    /**
     * @brief Останавливает сервер и отключает всех клиентов.
     */
    void stopServer();

private slots:
    /**
     * @brief Обрабатывает новое входящее подключение клиента.
     *
     * Добавляет клиента в список, подключает слоты чтения и обработки отключения.
     */
    void handleNewConnection();

    /**
     * @brief Обрабатывает отключение клиента.
     *
     * Удаляет клиента из списка и освобождает ресурсы.
     */
    void slotClientDisconnected();

    /**
     * @brief Обрабатывает входящие данные от клиента.
     *
     * Читает команду, передаёт её в парсер и отправляет ответ клиенту.
     */
    void slotServerRead();

private:
    /**
     * @brief Указатель на объект TCP-сервера.
     *
     * Отвечает за прослушивание входящих соединений.
     */
    QTcpServer *mTcpServer;

    /**
     * @brief Список подключённых клиентов.
     *
     * Хранит все активные подключения для отправки ответов и управления сессиями.
     */
    QList<QTcpSocket*> Clients;

};

#endif // TCPSERVER_H
