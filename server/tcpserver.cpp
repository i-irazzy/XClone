#include "tcpserver.h"
#include "functionsforserver.h"

TcpServer::TcpServer(QObject *parent) : QObject(parent) {
    mTcpServer = new QTcpServer(this);
}

TcpServer::~TcpServer() {
    stopServer(); //остановка сервера при уничтожении объекта
}

void TcpServer::startServer() {
    if (!mTcpServer->listen(QHostAddress::Any, 33333)) {
        qDebug() << "Не получается запустить сервер.";
    } else {
        qDebug() << "Сервер запущен на порту 33333...";
        connect(mTcpServer, &QTcpServer::newConnection, this, &TcpServer::handleNewConnection);
    }
}

void TcpServer::stopServer() {
    for (QTcpSocket* client: Clients){
        client -> disconnect();
        client -> deleteLater();
    }
    Clients.clear();

    mTcpServer->close(); // Закрываем сервер
    qDebug() << "Сервер остановлен.";
}

void TcpServer::handleNewConnection() {
    QTcpSocket *clientSocket = mTcpServer -> nextPendingConnection();
    Clients.append(clientSocket);

    // Подключаем сигналы для чтения данных и отключения клиента
    connect(clientSocket, &QTcpSocket::readyRead, this, &TcpServer::slotServerRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &TcpServer::slotClientDisconnected);

    qDebug() << "Новое подключение: " << clientSocket->peerAddress().toString();
}

void TcpServer::slotClientDisconnected() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if(clientSocket){
        qDebug() << "Клиент отключен: " << clientSocket->peerAddress().toString();
        Clients.removeOne(clientSocket);
        clientSocket ->deleteLater();
    }
}

void TcpServer::slotServerRead() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if(clientSocket){
        QByteArray data = clientSocket->readAll();
        QString command = QString::fromUtf8(data).trimmed();

        qDebug() <<"Получено от клиента: " << command;

        QString response = parse(command);

        clientSocket->write(response.toUtf8());
    }


}

