#ifndef TCPSERVER_H
#define TCPSERVER_H

#include<QTcpServer>
#include<QObject>
#include<QTcpSocket>
#include<QByteArray>
#include<QDebug>
#include<QList>


class TcpServer : public QObject
{
    Q_OBJECT

public:
    explicit TcpServer(QObject *parent = nullptr);
    ~TcpServer();

    void startServer(); //запуск сервера
    void stopServer();  //остановка сервера

private slots:
    void handleNewConnection(); //обработка новых подключений
    void slotClientDisconnected(); //обработка отключения клиента
    void slotServerRead(); //обработка входящих данных

private:
    QTcpServer *mTcpServer; //указатель на TCP-сервер
    QList<QTcpSocket*> Clients;


};

#endif // TCPSERVER_H
