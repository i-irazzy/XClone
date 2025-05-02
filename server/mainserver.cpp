#include <QCoreApplication>
#include <QDebug>
#include "tcpserver.h"
#include "databasemanager.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    DatabaseManager* dbManager = DatabaseManager::getInstance();
    if (!dbManager->initializeDatabase("..\\..\\xclone.db")) {
        qDebug() << "Не получилось подключится к базе данных.";
        return -1;
    }

    qDebug() << "Подключение к базе данных успешно.";
    TcpServer server;
    server.startServer();
    return a.exec();
}
