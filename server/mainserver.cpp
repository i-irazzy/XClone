#include <QCoreApplication>
#include <QDebug>
#include "tcpserver.h"
#include "databasemanager.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv); ///< Создаём консольное Qt-приложение

    DatabaseManager* dbManager = DatabaseManager::getInstance(); ///< Получаем экземпляр менеджера базы данных (Singleton)

    ///< Соединеняемся с БД
    if (!dbManager->initializeDatabase("..\\..\\xclone.db")) {
        qDebug() << "Не получилось подключится к базе данных."; ///< Ошибка при подключении к БД
        return -1; ///< Завершаем приложение с ошибкой
    }

    qDebug() << "Подключение к базе данных успешно."; ///< Успешное подключение к БД

    TcpServer server; ///< Создаём TCP-сервер
    server.startServer(); ///< Запускаем сервер

    return a.exec(); ///< Запускаем главный цикл приложения
}
