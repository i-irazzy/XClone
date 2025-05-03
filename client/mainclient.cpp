#include "authregform.h"
#include "mainwindow.h"
#include <QApplication>

int mainClient(int argc, char *argv[])
{
    QApplication a(argc, argv);

    AuthRegForm authForm;
    MainWindow *mainWindow = nullptr;

    QObject::connect(&authForm, &AuthRegForm::auth_ok, [&](QString username){
        authForm.close();  // Закрываем окно авторизации
        mainWindow = new MainWindow();
        mainWindow->show();  // Открываем главное окно
    });

    authForm.show();  // Показываем окно авторизации
    return a.exec();
}
