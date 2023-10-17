#include "loginform.h"
#include "ui_loginform.h"

LoginForm::LoginForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);


}

LoginForm::~LoginForm()
{
    delete ui;
}

/*void LoginForm::getResultRequestd_AuthorizationFromServer(QString request)
{
    this->ResultRequestd_Authorization = request;
}*/

/*void LoginForm::connectServer()
{
    ui->label_2->setText("Connect");
}*/

void LoginForm::on_buttonBox_accepted()
{
    emit this->authorization("@" + ui->LoginEdit->text(), ui->PasswordEdit->text());
    //emit this->chatStartRequested();

    ui->LoginEdit->clear();
    ui->PasswordEdit->clear();
}


void LoginForm::on_buttonBox_rejected()
{
    emit exist();
}


void LoginForm::on_registrationPushButton_clicked()
{
    ui->LoginEdit->clear();
    ui->PasswordEdit->clear();
    emit this->registerRequested();
}

