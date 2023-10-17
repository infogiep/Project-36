#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(std::vector<std::string> list, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Сетевой Чат");

    connect(this, &MainWindow::currentLogin, ui->WindowChatWidget, &windowChatForm::getCurrentLogin);
    connect(this, &MainWindow::ListUser, this, &MainWindow::requestListUser);

    //connect(this, &MainWindow::updateList, ui->WindowChatWidget, &windowChatForm::requestUpdateList);

    connect(this, &MainWindow::ListSendMessag, this, &MainWindow::requestListSendMessag);

    connect(ui->LoginWidget, &LoginForm::chatStartRequested, this, &MainWindow::setWindowChatForm);
    connect(ui->LoginWidget, &LoginForm::registerRequested, this, &MainWindow::setRegistrationForm);

    connect(ui->LoginWidget, &LoginForm::authorization, this, &MainWindow::requestd_Authorization);
    connect(ui->LoginWidget, &LoginForm::exist, this, &MainWindow::requestExist);


    connect(ui->RegistrationWidget, &RegistrationForm::loginRequested, this, &MainWindow::setLoginForm);
    connect(ui->RegistrationWidget, &RegistrationForm::chatStartRequested, this, &MainWindow::setWindowChatForm);

    connect(ui->RegistrationWidget, &RegistrationForm::checkExistens, this, &MainWindow::request_checkExistens);
    connect(ui->RegistrationWidget, &RegistrationForm::registrationUser, this, &MainWindow::requestRegistrationUser);

    connect(ui->RegistrationWidget, &RegistrationForm::exist, this, &MainWindow::requestExist);

    connect(ui->WindowChatWidget, &windowChatForm::loginRequested, this, &MainWindow::setLoginForm);

    connect(ui->WindowChatWidget, &windowChatForm::sendMessage, this, &MainWindow::requestSendMessage);

    connect(ui->WindowChatWidget, &windowChatForm::closeUser, this, &MainWindow::requestCloseUser);
    //connect(ui->WindowChatWidget, &windowChatForm::ListUser, this, &MainWindow::requestListUser);

    //connect(ui->WindowChatWidget, &windowChatForm::updateListUser, this, &MainWindow::requestListUser);


    this->tcpSocket = new QTcpSocket();
    connect(tcpSocket, &QTcpSocket::readyRead, this, &MainWindow::slotReadSocket);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &MainWindow::disardSocket);

    //this->tcpSocket->connectToHost(QHostAddress("127.0.0.1"), 2323);
    this->tcpSocket->connectToHost(QHostAddress(QString::fromStdString(list[0])), 2323);


    if(this->tcpSocket->waitForConnected(30000))
    {
        ui->statusbar->showMessage("Client connected!");
        //emit this->accepted();
        emit this->ListUser("list");
    }else
    {
        ui->statusbar->showMessage("Client not connected: " + this->tcpSocket->errorString());
    }

    /*auto timer = new QTimer(this);
    std::future<void> f = std::async(std::launch::async, [&](){
        connect(timer, &QTimer::timeout, this, &MainWindow::updateListUser);
        timer->start(50);
    });*/
    //connect(timer, &QTimer::timeout, this, &MainWindow::updateListUser);
    //timer->start(1000);
}

MainWindow::~MainWindow()
{
    if(this->tcpSocket->isOpen())
        this->tcpSocket->close();
    delete ui;
}

void MainWindow::setLoginForm()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::setRegistrationForm()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::setWindowChatForm()
{
    ui->stackedWidget->setCurrentIndex(2);
    emit ListSendMessag("incom");
}

QString MainWindow::getResult(QString str, QString mod)
{
    QList<QString> list = str.split('/');
    QString res = "";

    for(int i = 0; i < list.count(); ++i)
    {
        if(list[i] == mod)
            continue;

        res += list[i];

        if(i < list.count() - 1)
            res += "/";
    }

    return res;
}

void MainWindow::requestd_Authorization(QString login, QString pass)
{
    //Авторизация: = log[0]/login[1]/password[2]/desc[3]

    QString request;
    request = " ";
    request = "log/" + login + "/" + pass;

    //this->setWindowTitle("Connect: " + login);
    this->sendMessageServer(this->tcpSocket, request);
}

void MainWindow::request_checkExistens(QString request)
{
    this->sendMessageServer(this->tcpSocket, request);
}

void MainWindow::requestRegistrationUser(QString request)
{
    this->sendMessageServer(this->tcpSocket, request);
}

void MainWindow::requestListUser(QString mode)
{
    this->sendMessageServer(this->tcpSocket, mode);
}

void MainWindow::requestSendMessage(QString request)
{
    this->sendMessageServer(this->tcpSocket, request);
}

void MainWindow::requestListSendMessag(QString mode)
{
    this->sendMessageServer(this->tcpSocket, mode);
}

void MainWindow::requestCloseUser(QString request)
{
    this->sendMessageServer(this->tcpSocket, request);
}

void MainWindow::requestExist()
{
    this->close();
}

void MainWindow::slotReadSocket()
{
    QTcpSocket *socket = reinterpret_cast<QTcpSocket*>(sender());

    QDataStream in(socket); // с помощью объекта in работаем с данными находящимеся в socket
    in.setVersion(QDataStream::Qt_6_5);

    //Проверяем состояние объекта in если все в порядке продолжаем дальше
    if(in.status() == QDataStream::Ok)
    {
        QString str;
        str.clear();
        in >> str;

        int index = this->getCommandIndex(str);
        QString value = "";


        switch (index) {
        case 1:
            //Авторизация(Вход)
            //QMessageBox::information(this, "авторизация", str);
            value = this->getResult(str, "log");
            if(value != "null")
            {
                //QMessageBox::information(this, "Авторизация", value);
                emit this->currentLogin(value);
                ui->LoginWidget->chatStartRequested();
            }
            else {
                QMessageBox::warning(this, "Авторизация", "Не верный логин или пароль!!!");
            }
            //emit this->resultRequestd_Authorization(str);
            break;
        case 2:
            //регистрация
            value = this->getResult(str, "reg");
            if(value != "null")
            {
                ui->WindowChatWidget->addNewUser(value);
                //this->updateListUser();
                //emit this->currentLogin(value);
                //ui->RegistrationWidget->chatStartRequested();
                //emit this->ListUser("list");
                QMessageBox::information(this, tr("Registration"), "Пользователь " + value + " зарегестрирован!!!");
                //ui->WindowChatWidget->requestUpdateList(value);
            }
            break;
        case 3:
            //Список сообщений
            //"incom"
            value = this->getResult(str, "incom");
            ui->WindowChatWidget->setListMessages(value);
            break;
        case 4:
            //Отправить сообщение
            value = this->getResult(str, "send");
            if(value != "null")
            {
                //QMessageBox::information(this, "Отправление сообщений", "Сообщение пользователю: " + value + " отправлено!");
                emit ListSendMessag("incom");
            }
            break;
        case 5:
            //Список пользователей
            value = this->getResult(str, "list");
            if(value != "-1")
                ui->WindowChatWidget->setListUser(value); //emit this->resultListUser(value);
            //QMessageBox::information(this, "List", value);
            break;
        case 6:
            /**/
            value = this->getResult(str, "check");
            if(value == "0")
                ui->RegistrationWidget;//QMessageBox::information(this, "Info", "Логина и пароля нет");

            if(value == "1")
                QMessageBox::information(this, "check", "логин и пароль существуют");

            if(value == "2")
                QMessageBox::information(this, "check", "логин есть пароля нет");

            if(value == "3")
                QMessageBox::information(this, "check", "логина нет пароль есть");
            /**/
            break;
        default:
            break;
        }
        //ui->privateChatBrowser->append(QString::number(socket->socketDescriptor()) + " ---> " + str);
        //ui->privateChatBrowser->append(str);
    }else
    {
        QMessageBox::warning(this, tr("DataStream"), tr("Error"));
    }
}

void MainWindow::disardSocket()
{
    this->tcpSocket->deleteLater();
}

void MainWindow::sendMessageServer(QTcpSocket *socket, QString str_msg)
{
    this->_data.clear();
    QDataStream out(&this->_data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);
    out << str_msg;
    socket->write(this->_data);
}

void MainWindow::updateListUser()
{
    emit this->ListUser("list");
}

int MainWindow::getCommandIndex(QString request)
{
    if(request.contains("log")) // вход
        return 1;
    if(request.contains("reg")) // регистрация
        return 2;
    if(request.contains("incom")) //получить сообщения
        return 3;
    if(request.contains("send"))//отправить сообщение
        return 4;
    if(request.contains("list"))// список пользователей
        return 5;
    if(request.contains("check")) // проверка существования логина и пароля
        return 6;
    if(request.contains("exit")) // выход
        return 0;
}

