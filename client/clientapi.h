#ifndef CLIENTAPI_H
#define CLIENTAPI_H

#include <QObject>
#include <QTcpSocket>// нужен для установки соединения
#include <QByteArray>// для работы со строкими и с байтами
#include <QDebug>// для вывода в консоль
#include <QtNetwork>// для указации IP - адресации
#include <QString>

class ClientAPI;

class SingletonDestroyer{
private:
    ClientAPI* p_instance;
public:
    ~SingletonDestroyer();
    void initialize(ClientAPI * p);
    //private:


};

class ClientAPI: public QObject{

    Q_OBJECT// набор констант

private:
    static ClientAPI * p_instance;
    static SingletonDestroyer destroyer;
    QTcpSocket *mTcpSocket;
protected:
    ClientAPI(QObject *parent = nullptr);
    ~ClientAPI();

    ClientAPI(const ClientAPI& ) = delete;
    ClientAPI& operator = (ClientAPI &) = delete;

    friend class SingletonDestroyer;

     static ClientAPI* instance;
public:
    static ClientAPI* getInstance();
     QByteArray query_to_server(QString);
    static void setInstance(ClientAPI* instance);
signals:
    void disconnected();
public slots:
    void slotServerDisconnection();

};


#endif // CLIENTAPI_H
