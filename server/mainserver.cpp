#include <QCoreApplication>
#include<QDebug>
#include "tcpserver.h"
#include"databasemanager.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    DatabaseManager* dbManager = DatabaseManager::getInstance();
    if (!dbManager->initializeDatabase("..\\..\\twitter.db")) {
        qDebug() << "Failed to initialize database!";
        return -1;
    }

    qDebug() << "Database initialized successfully!";
    TcpServer server;
    server.startServer();
    return a.exec();
}
