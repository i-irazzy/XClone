#include <QCoreApplication>
#include <QDebug>
#include "tcpserver.h"
#include "databasemanager.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv); ///< Создаём консольное Qt-приложение

    DatabaseManager* dbManager = DatabaseManager::getInstance(); ///< Получаем экземпляр менеджера базы данных (Singleton)

    if (!dbManager->initializeDatabase("..\\..\\xclone.db")) {
        qDebug() << "Не получилось подключиться к базе данных.";
        return -1;
    }
    qDebug() << "Подключение к базе данных успешно.";



    TcpServer server; ///< Создаём TCP-сервер
    server.startServer(); ///< Запускаем сервер

    return a.exec(); ///< Запускаем главный цикл приложения
}
