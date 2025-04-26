// #include <QCoreApplication>
// #include<QDebug>
// #include "tcpserver.h"
// #include"databasemanager.h"

// int main(int argc, char *argv[]) {
//     QCoreApplication a(argc, argv);
//     //инициализация базы данных
//     DatabaseManager* dbManager = DatabaseManager::getInstance();  //получаем экземпляр DatabaseManager

//     // Подключаемся к базе данных (укажите путь к файлу базы данных)
//     if (!dbManager->connect("twitter.db")) {
//         qDebug() << "Failed to connect to database!";
//         return -1;  //завершаем программу, если подключение не удалось
//     }

//     //создаем таблицы, если они ещё не существуют
//     QString createUsersTable = "CREATE TABLE IF NOT EXISTS users ("
//                                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
//                                "username TEXT UNIQUE NOT NULL, "
//                                "password TEXT NOT NULL);";

//     QString createTweetsTable = "CREATE TABLE IF NOT EXISTS tweets ("
//                                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
//                                 "username TEXT NOT NULL, "
//                                 "tweet TEXT NOT NULL);";

//     QString createFollowsTable = "CREATE TABLE IF NOT EXISTS follows ("
//                                  "follower TEXT NOT NULL, "
//                                  "followee TEXT NOT NULL);";

//     // Выполняем запросы на создание таблиц
//     if (!dbManager->executeQuery(createUsersTable)) {
//         qDebug() << "Failed to create users table!";
//         return -1;
//     }

//     if (!dbManager->executeQuery(createTweetsTable)) {
//         qDebug() << "Failed to create tweets table!";
//         return -1;
//     }

//     if (!dbManager->executeQuery(createFollowsTable)) {
//         qDebug() << "Failed to create follows table!";
//         return -1;
//     }

//     qDebug() << "Database initialized successfully!";
//     TcpServer server;
//     server.startServer();
//     return a.exec();
// }

