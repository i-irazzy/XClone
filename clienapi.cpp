#include "clienapi.h"
#include <QDebug>// для вывода в консоль

ClientAPI* ClientAPI::p_instance;
SingletonDestroyer ClientAPI::destroyer;

SingletonDestroyer::~SingletonDestroyer(){
    delete p_instance;
}

void SingletonDestroyer::initialize(ClientAPI *p){
    p_instance = p;
}

ClientAPI* ClientAPI::getInstance(){

    if (!p_instance)
    {
        p_instance = new ClientAPI();
        destroyer.initialize(p_instance);
    }
    return p_instance;
}

ClientAPI:: ClientAPI(QObject *parent){

    mTcpSocket = new QTcpSocket(this);// инициализируется сокет

    connect(this->mTcpSocket, &QTcpSocket::disconnected, this, &ClientAPI::slotServerDisconnection);

    mTcpSocket->connectToHost("127.0.01", 33333);

    mTcpSocket->waitForReadyRead();

    QByteArray array;

    while(mTcpSocket->bytesAvailable()>0){
        array.append(mTcpSocket->readAll());

    }
    qDebug() << array;

}

ClientAPI::~ClientAPI(){
    this->mTcpSocket->close();// закрывает соединение

}

void ClientAPI:: slotServerDisconnection(){

    mTcpSocket->close();
    // emit disconnected();
}


QByteArray ClientAPI::query_to_server(QString msg){

    mTcpSocket->write(msg.toUtf8());
    mTcpSocket->waitForReadyRead();

    QByteArray array;

    while(mTcpSocket->bytesAvailable()>0){

        array.append(mTcpSocket->readAll());

    }
    qDebug() << array;

    return array;
}
