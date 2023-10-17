#include "registrationform.h"
#include "ui_registrationform.h"

RegistrationForm::RegistrationForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegistrationForm)
{
    ui->setupUi(this);
}

RegistrationForm::~RegistrationForm()
{
    delete ui;
}

void RegistrationForm::on_buttonBox_accepted()
{
    //Регистрация пользователя: = reg[0]/Фамилия[1]/Имя[2]/Отчество[3]/Email[4]/Login[5]/Password[6]/desc[7]

    QString request = "reg/" + ui->SurnameEdit->text() + "/" + ui->NameEdit->text() + "/" + ui->PatronymicEdit->text() + "/"
        + ui->EmailEdit->text() + "/@" + ui->LoginEdit->text() + "/" + ui->PasswordEdit->text();

    emit this->registrationUser(request);
    //emit this->checkExistens(request);
    //emit this->chatStartRequested();
    ui->SurnameEdit->clear();
    ui->NameEdit->clear();
    ui->PatronymicEdit->clear();
    ui->EmailEdit->clear();
    ui->LoginEdit->clear();
    ui->PasswordEdit->clear();
}


void RegistrationForm::on_buttonBox_rejected()
{
    emit this->exist();
}

void RegistrationForm::on_LoginButton_clicked()
{
    emit this->loginRequested();
}

