#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>
#include <QMessageBox>
#include <chrono>
#include <thread>

namespace Ui {
class LoginForm;
}

class LoginForm : public QWidget
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget *parent = nullptr);
    ~LoginForm();

signals:
    void registerRequested();
    void chatStartRequested();

    void authorization(QString login, QString pass); // сигнал для отправки данных на сервер для авторизации
    void exist();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_registrationPushButton_clicked();

    //QString getResultRequestd_Authorization();

private:
    Ui::LoginForm *ui;
};

#endif // LOGINFORM_H
