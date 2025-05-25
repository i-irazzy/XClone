#include "tcpserver.h"
#include "functionsforserver.h"

TcpServer::TcpServer(QObject *parent) : QObject(parent) {
    mTcpServer = new QTcpServer(this); ///< Инициализируем TCP-сервер
}

TcpServer::~TcpServer() {
    stopServer(); ///< Останавливаем сервер при уничтожении объекта
}

void TcpServer::startServer() {
    if (!mTcpServer->listen(QHostAddress::Any, 33333)) {
        qDebug() << "Не получается запустить сервер."; ///< Ошибка при запуске сервера
    } else {
        qDebug() << "Сервер запущен на порту 33333..."; ///< Успешный запуск сервера
        connect(mTcpServer, &QTcpServer::newConnection, this, &TcpServer::handleNewConnection); ///< Подключаем слот для обработки новых подключений
    }
}

void TcpServer::stopServer() {
    for (QTcpSocket* client : Clients) {
        client->disconnect(); ///< Отключаем клиента от сигналов
        client->deleteLater(); ///< Помечаем на удаление
    }

    Clients.clear(); ///< Очищаем список клиентов
    mTcpServer->close(); ///< Закрываем сервер
    qDebug() << "Сервер остановлен."; ///< Подтверждение остановки
}

void TcpServer::handleNewConnection() {
    QTcpSocket *clientSocket = mTcpServer->nextPendingConnection(); ///< Получаем сокет нового клиента
    Clients.append(clientSocket); ///< Добавляем клиента в список

    connect(clientSocket, &QTcpSocket::readyRead, this, &TcpServer::slotServerRead); ///< Подключаем слот для чтения данных
    connect(clientSocket, &QTcpSocket::disconnected, this, &TcpServer::slotClientDisconnected); ///< Подключаем слот для обработки отключения

    qDebug() << "Новое подключение: " << clientSocket->peerAddress().toString(); ///< Логируем IP клиента
}

void TcpServer::slotClientDisconnected() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender()); ///< Получаем сокет, вызвавший сигнал
    if (clientSocket) {
        qDebug() << "Клиент отключен: " << clientSocket->peerAddress().toString(); ///< Лог отключения
        Clients.removeOne(clientSocket); ///< Удаляем клиента из списка
        clientSocket->deleteLater(); ///< Освобождаем память
    }
}

void TcpServer::slotServerRead() {
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender()); ///< Получаем сокет клиента
    if (!clientSocket) {
        qDebug() << "Ошибка: клиентский сокет не найден!";
        return;
    }

    QByteArray data = clientSocket->readAll(); ///< Читаем все данные
    QString command = QString::fromUtf8(data).trimmed(); ///< Преобразуем в строку и удаляем пробелы

    if (command.isEmpty()) {
        qDebug() << "Ошибка: получена пустая команда!";
        return;
    }

    qDebug() << "Получено от клиента: " << command; ///< Лог команды

    QString response = parse(command); ///< Обработка команды
    qDebug() << "Отправлено клиенту: " << response;
    if (clientSocket->state() == QAbstractSocket::ConnectedState) {
        clientSocket->write(response.toUtf8()); ///< Отправка ответа клиенту
        clientSocket->flush(); ///< Гарантированная отправка данных
    } else {
        qDebug() << "Ошибка: клиент уже отключился, ответ не отправлен.";
    }
}

