#include "authregform.h"
#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    AuthRegForm authForm;
    MainWindow *mainWindow = nullptr;

    QObject::connect(&authForm, &AuthRegForm::auth_ok, [&](QString username){
        authForm.close();  // Закрываем окно авторизации
        mainWindow = new MainWindow();
        mainWindow->show();  // Открываем главное окно
        mainWindow->SearchPosts();
    });

    authForm.show();  // Показываем окно авторизации
    return a.exec();
}
