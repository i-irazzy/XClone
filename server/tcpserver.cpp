#include "tcpserver.h"
#include"functionsforserver.h"

TcpServer::TcpServer(QObject *parent) : QObject(parent) {
    mTcpServer = new QTcpServer(this);
}

TcpServer::~TcpServer() {
    stopServer(); //остановка сервера при уничтожении объекта
}

void TcpServer::startServer() {
    if (!mTcpServer->listen(QHostAddress::Any, 33333)) {
        qDebug() << "Server could not start!";
    } else {
        qDebug() << "Server started on port 33333...";
        connect(mTcpServer, &QTcpServer::newConnection, this, &TcpServer::handleNewConnection);
    }
}

void TcpServer::stopServer() {
    mTcpServer->close(); // Закрываем сервер
    for (QTcpSocket* client: Clients){
        client -> disconnect();
        client -> deleteLater();
    }
    Clients.clear();

    qDebug() << "Server stopped.";
}

void TcpServer::handleNewConnection() {
    QTcpSocket *clientSocket = mTcpServer -> nextPendingConnection();
    Clients.append(clientSocket);

    // Подключаем сигналы для чтения данных и отключения клиента
    connect(clientSocket, &QTcpSocket::readyRead, this, &TcpServer::slotServerRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &TcpServer::slotClientDisconnected);

    qDebug() << "New client connected:" << clientSocket->peerAddress().toString();
}

void TcpServer::slotClientDisconnected() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if(clientSocket){
        qDebug() << "Client disconected:" << clientSocket -> peerAddress().toString();
        Clients.removeOne(clientSocket);
        clientSocket ->deleteLater();
    }
}

void TcpServer::slotServerRead() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if(clientSocket){
        QByteArray data = clientSocket->readAll();
        QString command = QString::fromUtf8(data).trimmed();

        qDebug() <<"Recived from client:" << command;

        QString response = parse(command);

        clientSocket->write(response.toUtf8());
    }


}

