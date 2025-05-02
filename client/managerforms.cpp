#include "managerforms.h"
#include "ui_managerforms.h"  // Подключаем автоматически сгенерированный файл интерфейса

ManagerForms::ManagerForms(QWidget *parent) : QMainWindow(parent), ui(new Ui::ManagerForms)
{
    ui->setupUi(this);  // Инициализация интерфейса с помощью Qt Designer

    // Создаем форму авторизации/регистрации
    curr_auth = new AuthRegForm(this);

    // Подключаем сигнал успешной авторизации к слоту для отображения основного окна
    connect(curr_auth, &AuthRegForm::auth_ok, this, &ManagerForms::onAuthSuccess);


    // Отображаем форму авторизации/регистрации
    setCentralWidget(curr_auth);
}

ManagerForms::~ManagerForms() {
    delete ui;  // Удаляем интерфейсный объект
}

void ManagerForms::onAuthSuccess(QString username) {
    main_form = new MainWindow(this);
    main_form->setCurrentUser(username); // Передача имени пользователя

    setCentralWidget(main_form);
    main_form->show();
}
