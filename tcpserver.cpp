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
    qDebug() << "Server stopped.";
}

void TcpServer::handleNewConnection() {
    mTcpSocket = mTcpServer->nextPendingConnection(); // Получаем следующий подключившийся сокет

    // Подключаем сигналы для чтения данных и отключения клиента
    connect(mTcpSocket, &QTcpSocket::readyRead, this, &TcpServer::slotServerRead);
    connect(mTcpSocket, &QTcpSocket::disconnected, this, &TcpServer::slotClientDisconnected);

    qDebug() << "New client connected:" << mTcpSocket->peerAddress().toString();
}

void TcpServer::slotClientDisconnected() {
    qDebug() << "Client disconnected:" << mTcpSocket->peerAddress().toString();
    mTcpSocket->deleteLater(); // Удаляем сокет
}

void TcpServer::slotServerRead() {
    QByteArray data = mTcpSocket->readAll(); // Читаем все доступные данные
    QString command = QString::fromUtf8(data).trimmed(); // Преобразуем в строку и убираем лишние пробелы

    qDebug() << "Received from client:" << command;

    // Парсим команду и получаем ответ
    QString response = parse(command);

    // Отправляем ответ клиенту
    mTcpSocket->write(response.toUtf8());
}

