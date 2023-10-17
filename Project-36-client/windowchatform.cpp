#include "windowchatform.h"
#include "ui_windowchatform.h"

windowChatForm::windowChatForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::windowChatForm)
{
    ui->setupUi(this);
    //ui->updateListUserButton->setEnabled(false);
}

windowChatForm::~windowChatForm()
{
    delete ui;
}

void windowChatForm::getCurrentLogin(QString Login)
{
    ui->currentLoginLable->setText(Login);
    this->_Login = Login;
}

QString windowChatForm::getLogin()
{
    return this->_Login;
}

QString windowChatForm::getListUser()
{
    return this->listUser;
}

void windowChatForm::setListUser(QString request)
{
    this->listUser = request;
    //ui->privateChatBrowser->append(this->listUser);
    //QMessageBox::information(this, "-", "Список получен");
}

void windowChatForm::setListMessages(QString request)
{
    this->listMessages = request;
    //QMessageBox::information(this, "-", "Список сообщений получен");

    auto messagesList = this->listMessages.split('/');

    ui->privateChatBrowser->clear();

    for(auto messag : messagesList)
    {
        if(messag.contains(this->getLogin()))
            ui->privateChatBrowser->append(messag);
    }
}

void windowChatForm::addNewUser(QString nLogin)
{
    this->listUser += "/" + nLogin;


    //QMessageBox::information(this, "-", "добавлен");
}

/*void windowChatForm::requestUpdateList(QString nLogin)
{
    ui->privateChatBrowser->append("Пользователь " + nLogin + " зарегестрирован!");
}*/


void windowChatForm::on_closeClientButton_clicked()
{
    ui->messagesLineEdit->clear();
    ui->currentLoginLable->clear();
    emit this->loginRequested();
    //this->setWindowTitle("Сетевой Чат");

    emit this->closeUser("exit/" + this->getLogin());
}


void windowChatForm::on_messagesLineEdit_returnPressed()
{
    this->on_senMessagesButton_clicked();
}


void windowChatForm::on_senMessagesButton_clicked()
{
    QDialog dial(this);
    dial.setModal(true);
    auto l = new QVBoxLayout();
    dial.setLayout(l);

    auto userListWidget = new QListWidget(&dial);
    l->addWidget(userListWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dial);
    l->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, &dial, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dial, &QDialog::reject);

    auto userList = this->getListUser().split('/');

    for(auto user : userList)
    {
        if(user == this->getLogin())
            continue;
        userListWidget->addItem(user);
    }

    userListWidget->setCurrentRow(0);

    auto result = dial.exec();

    if(result == QDialog::Accepted && userListWidget->currentItem())
    {
        //Отправить сообщение
        /*Отправить сообщение recipient - получатель; sender - отправитель
          Структура команы: = send[0]/recipient[1]/sender[2]/text[3]/desc[4]*/
        QString request = "send/" + userListWidget->currentItem()->text() + "/" + this->getLogin() + "/" + ui->messagesLineEdit->text();
        emit this->sendMessage(request);
        //ui->privateChatBrowser->append("Сообщение отправлено: " + userListWidget->currentItem()->text() + " от " + this->getLogin());
    }

    ui->messagesLineEdit->clear();
}

