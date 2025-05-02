#ifndef NEWPOSTFORM_H
#define NEWPOSTFORM_H

#include <QWidget>

namespace Ui {
class NewPostForm;
}

class NewPostForm : public QWidget
{
    Q_OBJECT

public:
    explicit NewPostForm(QWidget *parent = nullptr);
    ~NewPostForm();

    // Методы для получения данных из формы
    QString getUsername() const;
    QString getHeader() const;
    QString getText() const;


public slots:
    void setCurrentUser(const QString &username);

private slots:
    void on_pushButtonPost_clicked();

    void on_pushButtonCancel_clicked();

private:
    Ui::NewPostForm *ui;
    QString currentUser;
    void clearFields();
signals:
    void newPostCreated(const QString &username, const QString &header, const QString &text);


};

#endif
