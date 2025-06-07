#include "clientapi.h"
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


void ClientAPI::setInstance(ClientAPI* newInstance) {
    if (p_instance) {
        delete p_instance;
    }
    p_instance = newInstance;
    destroyer.initialize(p_instance);
}


ClientAPI:: ClientAPI(QObject *parent){

    mTcpSocket = new QTcpSocket(this);// инициализируется сокет

    connect(this->mTcpSocket, &QTcpSocket::disconnected, this, &ClientAPI::slotServerDisconnection);

    mTcpSocket->connectToHost("127.0.0.1", 33333);

    mTcpSocket->waitForReadyRead();

    QByteArray array;

    while(mTcpSocket->bytesAvailable()>0){
        array.append(mTcpSocket->readAll());

    }
    qDebug() << array;

}

ClientAPI::~ClientAPI(){
    if (mTcpSocket->isOpen()) {
        mTcpSocket->close();
    }
}

void ClientAPI:: slotServerDisconnection(){

    mTcpSocket->close();
    emit disconnected();
    qDebug() << "Соединение с сервером разорвано.";
}


QByteArray ClientAPI::query_to_server(QString query){

    mTcpSocket->write(query.toUtf8());
    mTcpSocket->waitForReadyRead();

    QByteArray array = "";

    while(mTcpSocket->bytesAvailable()>0){

        array.append(mTcpSocket->readAll());

    }
    qDebug() << array;

    return array;
}
