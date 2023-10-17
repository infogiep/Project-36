#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(std::vector<std::string> list, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->tcpServer = new QTcpServer(); // Инициализируем сервер

    if(this->tcpServer->listen(QHostAddress::LocalHost, 2323)) // сервер прослушивает порт 2323
    {
        connect(this->tcpServer, &QTcpServer::newConnection, this, &MainWindow::newConnection);
        ui->statusbar->showMessage("Server Start");
        ui->terminalChatBrowser->append("Server Start");

        /*---Подключение к БД---*/
       std::string host = list[0]; //std::string host = "localhost";
        std::string user = list[1];//std::string user = "root";
        std::string password = list[2]; //std::string password = "admin_password23";
        std::string dbName = list[3]; //std::string dbName = "db_chat_dz25";

        this->dbConnect = mySQL_db(host, user, password, dbName);

        dbConnect.connect_DB(dbConnect.get_dbName());

        if (dbConnect.getConnectStatus() != true)
        {
            dbConnect.create_DB(dbConnect.get_dbName());
        }

        if (dbConnect.getConnectStatus())
        {
            tables[0] = new db_Table_Client(dbConnect.getConnectStatus(), dbConnect); // Создание таблицы clients исли она не существуют

            tables[1] = new db_Table_hPassword(dbConnect.getConnectStatus(), dbConnect); // Создание таблицы password_hash исли она не существуют

            tables[2] = new db_Table_Messages(dbConnect.getConnectStatus(), dbConnect); // Создать таблицу messages если она не существуют


            if (tables[0]->getStatusCreateTable() && tables[1]->getStatusCreateTable() && tables[2]->getStatusCreateTable())
            {
                this->_chatStart = true;

            }
            else
                this->_chatStart = false;



            if(this->_chatStart == true)
            {
                ui->terminalChatBrowser->append("Start DB");
            }
            else
                ui->terminalChatBrowser->append("Stop DB");
        }

        /*this->dbChat = Chat(this->dbConnect);

        this->dbChat.startChat();

        if(this->dbChat.get_ChatStart())
            QMessageBox::information(this, tr("DB Status"), "Connect");*/
    }else
    {
        QMessageBox::warning(this, tr("TCP Server Error"), this->tcpServer->errorString());
    }
}

MainWindow::~MainWindow()
{
    delete tables[0];
    delete tables[1];
    delete tables[2];

    delete ui;
}

void MainWindow::readSocket()
{
    QTcpSocket *socket = reinterpret_cast<QTcpSocket*>(sender());

    QDataStream in(socket); // с помощью объекта in работаем с данными находящимеся в socket
    in.setVersion(QDataStream::Qt_6_5);

    //Проверяем состояние объекта in если все в порядке продолжаем дальше
    if(in.status() == QDataStream::Ok)
    {
        QString str;
        in >> str;
        ui->terminalChatBrowser->append(QString::number(socket->socketDescriptor()) + " ---> " + str);
        //this->sendMessage(socket, QString::number(socket->socketDescriptor()) + " : " + str);
        this->databaseQuery(str + "/" + QString::number(socket->socketDescriptor()), socket);
    }else
    {
        QMessageBox::warning(this, tr("DataStream"), tr("Error"));
    }
}

void MainWindow::discardSocket()
{
    //удалить клиента из списка при отключении клиента
    QTcpSocket *socket = reinterpret_cast<QTcpSocket*>(sender());
    int idx = this->listClient.indexOf(socket);

    if(idx > -1)
    {
        this->listClient.removeAt(idx);
    }

    //Обновление списка подключенных клиентво в combobox
    /*ui->comboBox->clear();

    for(QTcpSocket *socket_temp : this->listClient)
    {
        ui->comboBox->addItem(QString::number(socket_temp->socketDescriptor()));
    }*/

    socket->deleteLater();
}

void MainWindow::newConnection()
{
    // hasPendingConnections() Возвращает, true если сервер имеет ожидающее соединение; в противном случае возвращает false.
    while(this->tcpServer->hasPendingConnections())
    {
        // nextPendingConnection() Возвращает следующее ожидающее соединение как подключенный QTcpSocket объект.
        this->addToSocketList(this->tcpServer->nextPendingConnection()); // добавляем сокет нового клиент в список сокетов
    }
}

void MainWindow::addToSocketList(QTcpSocket *socket)
{
    this->listClient.append(socket);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::readSocket);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::discardSocket);
    ui->terminalChatBrowser->append("Socket ID: " + QString::number(socket->socketDescriptor()));
    //ui->comboBox->addItem(QString::number(socket->socketDescriptor()));
}

void MainWindow::sendMessage(QTcpSocket *socket, QString str_msg)
{
    this->_data.clear();
    QDataStream out(&this->_data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);
    out << str_msg;

    socket->write(this->_data);
    /*for(int i = 0; i < this->listClient.size(); ++i)
    {
        this->listClient[i]->write(this->_data);
    }*/
}

void MainWindow::sendMessage_ALL_or_oneClient(QString str_msg, QString descriptor)
{
    //QMessageBox::information(this, "Info", descriptor);

    if(descriptor == "all")
    {
        for(QTcpSocket* socket : this->listClient)
        {
            this->sendMessage(socket, str_msg);
        }
    }
    else
    {
        for(QTcpSocket* socket : this->listClient)
        {
            if(socket->socketDescriptor() == descriptor.toLongLong())
            {
                this->sendMessage(socket, str_msg);
                break;
            }
        }
    }
}

void MainWindow::dysplayList(QString item)
{
    if(item.contains("/"))
    {
        auto list = item.split('/');
        ui->ListClientsChatBrowser->clear();

        for(auto &item : list)
        {
            if(item == "list")
                continue;

            ui->ListClientsChatBrowser->append(item);
        }
    }else
        ui->ListClientsChatBrowser->append(item);
}

void MainWindow::databaseQuery(QString requests, QTcpSocket *socket)
{
    // 1 - вход; 2 - регистрация; 3 - Входящие и отправленные сообщения;
    // 4 - Отправить сообщение; 5 - список пользователей; 0 - выход
    QList<QString> listCommand;
    listCommand.clear();
    listCommand = requests.split('/');

    QString resultValue = ""; // результат для отправки получателю(получателям)

    // для получения хэш функции пароля
    auto setUserPassword = [](const std::string &password){

        int sum = 0;
        for (int i = 0; i < password.size(); ++i)
        {
            sum += password[i];
        }
        return std::to_string(sum);
    };

    /*---Выход из пользователя----*/
    //Структура команды: = exist[0]/login[1]/desc[2]

    auto closeUser = [&](QList<QString> listCommand){
        return listCommand[1];
    };

    /*---Авторизация---*/
    //Структура команды: = log[0]/login[1]/password[2]/desc[3]
    auto user_Login = [&](QList<QString> listCommand){
        //
        QString currLogin = "null";
        std::string h_p = setUserPassword(listCommand[2].toStdString());

        std::string valueLogin = tables[0]->get_id_user(listCommand[1].toStdString());
        std::string valuePass = tables[1]->get_id_user(h_p);

        if (valueLogin == valuePass && valueLogin != "-1" && valuePass != "-1")
            currLogin = listCommand[1]; //return "log/" + listCommand[1].toStdString(); //listCommand[1].toStdString();

        return "log/" + currLogin;
    }; // запрос на авторизация в чате


    /*-----------------------------------------*/

    /*---Проверка существования логина и пароля---*/
    //Структура команды: = check[0]/login[1]/password[2]/desc[3]
    auto checkingLogin_and_Password = [&](QList<QString> listCommand){

        std::string h_p = setUserPassword(listCommand[2].toStdString());

        std::string value_login = tables[0]->get_id_user(listCommand[1].toStdString()); //логина
        std::string value_h_p = tables[1]->get_id_user(h_p); //пароля

        /* 0 - логина и пароля нет;
           1 - логин и пароль существуют;
           2 - логин есть пароля нет;
           3 - логина нет пароль есть*/

        if (value_login == "-1" && value_h_p == "-1") // логина и пароля нет
            return "check/0";

        if(value_login != "-1" && value_h_p != "-1") //логин и пароль существуют
            return "check/1";

        if(value_login != "-1" && value_h_p == "-1") //логин есть пароля нет
            return "check/2";

        if(value_login == "-1" && value_h_p != "1") //логина нет пароль есть
            return "check/3";
    };

    /*-----------------------------------------*/

    /*---Регистрация пользователя---*/
    //Структура команды: = reg[0]/Фамилия[1]/Имя[2]/Отчество[3]/Email[4]/Login[5]/Password[6]/desc[7]
    auto user_Registration = [&](QList<QString> listCommand){
        std::string list[] = {
            listCommand[1].toStdString(), //Фамилия
            listCommand[2].toStdString(), //Имя
            listCommand[3].toStdString(), //Отчество
            listCommand[4].toStdString(), // Email
            listCommand[5].toStdString() // Login
        };

        bool flag_clients = tables[0]->add_in_table(list); // Добавить значения в таблицу Clients
        QString currentLogin = "null";


        std::string list_2[] = {
            tables[0]->get_id_user(listCommand[5].toStdString()), // Login
            setUserPassword(listCommand[6].toStdString()) // password
        };

        bool flag_h_p = tables[1]->add_in_table(list_2);

        if (flag_clients == true && flag_h_p == true)
            currentLogin = listCommand[5]; //return "reg/1";

        return "reg/" + currentLogin;
    };

    /*-----------------------------------------*/


    /*---Список пользователей чата ---*/
    //Структура команы: = list[0]/desc[2]
    auto list_Users = [&](){

        //std::string query = "select clients.login from clients where clients.login != '" + listCommand[1].toStdString() + "'";
        std::string query = "select clients.login from clients";

        auto list = tables[0]->request_select_from_Table(query);

        QString result = "list/-1";

        if (list.size() > 0)
        {
            result.clear();

            result += "list/";

            for(int i = 0; i < list.size(); ++i)
            {
                result += list[i];

                if(i < list.size() - 1)
                    result += "/";
            }

            return result;
        }
        else
            return result;
    };

    /*-----------------------------------------*/
    /*---Отправить сообщение---*/
    //recipient - получатель; sender - отправитель
    //Структура команы: = send[0]/recipient[1]/sender[2]/text[3]/desc[4]
    auto send_Message = [&](QList<QString> listCommand){
        // проверка существования логина получателя
        std::string rec_login = tables[0]->get_id_user(listCommand[1].toStdString());

        //QMessageBox::information(this, "send", QString::fromStdString(rec_login));

        //QMessageBox::information(this, "recipient", listCommand[1]);
        QString recipientLogin = "";

        if (tables[0]->get_id_user(listCommand[1].toStdString()) == "-1")
        {
            //QMessageBox::information(this, "Send", );
            recipientLogin = "null"; //return "send/-1";
        }else
        {
            std::string list[] = {listCommand[2].toStdString(), //отправитель
                                  listCommand[1].toStdString(), // получатель
                                  listCommand[3].toStdString()}; //текст сообщения

            if (tables[2]->add_in_table(list) == true)
                recipientLogin = listCommand[1];//return "send/1";
        }

        return "send/" + recipientLogin;
    };

    /*-----------------------------------------*/
    /*---Получить список отправленных сообщений---*/
    //Структура команы: = incom[0]/desc[1]
    auto incoming_and_sent_Messages = [&](QList<QString> listCommand){

        /*std::string query = "select sender_login, recipient_login, text_messag, date_send "
                            "from messages where sender_login = '" + listCommand[1].toStdString()
                            + "' OR recipient_login = '" + listCommand[1].toStdString() + "'";*/

        std::string query = "select sender_login, recipient_login, text_messag, date_send from messages";


        auto list = tables[2]->request_select_from_Table(query);

        /*for (auto& item : list)
            ui->terminalChatBrowser->append(QString::fromStdString(item));*/

        QString result = "incom/-1";

        if (list.size() > 0)
        {
            result.clear();

            result += "incom/";

            for(int i = 0; i < list.size(); ++i)
            {
                result += list[i];

                if(i < list.size() - 1)
                    result += "/";
            }

            return result;
        }
        else
            return result;
    };

    /*-----------------------------------------*/

    /*---Индекс команды---*/
    int index = this->get_commandIndex(requests);

    switch (index) {
    case 1:
        //Авторизация в чат(вход)
        /**/
        resultValue = " ";
        resultValue = user_Login(listCommand);
        //QMessageBox::information(this, "+", resultValue);
        //this->sendMessage(socket, requests);
        this->sendMessage_ALL_or_oneClient(resultValue,listCommand[3]);
        //ui->terminalChatBrowser->append(resultValue);
        if(resultValue != "null")
            ui->terminalChatBrowser->append("Connect: " +listCommand[1] + " : " + QString::number(socket->socketDescriptor()));

        //ui->terminalChatBrowser->append("Авторизация id = " + QString::number(socket->socketDescriptor()));
        listCommand.clear();
        break;
    case 2:
        // регистрация
        resultValue = " ";
        resultValue = user_Registration(listCommand);
        //"all"
        //this->sendMessage_ALL_or_oneClient(resultValue,listCommand[7]);
        this->sendMessage_ALL_or_oneClient(resultValue,"all");
        //ui->terminalChatBrowser->append("регистрация " + QString::number(socket->socketDescriptor()));
        if(resultValue != "null")
        {
            ui->terminalChatBrowser->append("Connect: " +listCommand[5] + " : " + QString::number(socket->socketDescriptor()));
            this->dysplayList(listCommand[5]);
        }

        //ui->terminalChatBrowser->append("регистрация  id = " + QString::number(socket->socketDescriptor()));
        listCommand.clear();
        break;
    case 3:
        resultValue = " ";
        resultValue = incoming_and_sent_Messages(listCommand);
        //QMessageBox::information(this, "ListSend", resultValue);
        this->sendMessage_ALL_or_oneClient(resultValue,listCommand[1]);
        ui->terminalChatBrowser->append("Получить список отправленных сообщений id = " + QString::number(socket->socketDescriptor()));
        listCommand.clear();
        break;
    case 4:
        resultValue = " ";
        resultValue = send_Message(listCommand);
        //this->sendMessage_ALL_or_oneClient(resultValue,listCommand[4]);
        this->sendMessage_ALL_or_oneClient(resultValue,"all");
        ui->terminalChatBrowser->append("Отправить сообщение: id = " + QString::number(socket->socketDescriptor()));
        break;
    case 5:
        /*---Список пользователей---*/
        resultValue = " ";
        resultValue = list_Users();
        //QMessageBox::information(this, "List", resultValue);
        //this->sendMessage_ALL_or_oneClient(resultValue,listCommand[1]);
        this->sendMessage_ALL_or_oneClient(resultValue,"all");
        dysplayList(resultValue);
        ui->terminalChatBrowser->append("Получить список пользователей: id = " + QString::number(socket->socketDescriptor()));
        listCommand.clear();
        break;
    case 6:
        //Проверка существования логина и пароля
        resultValue = " ";
        resultValue = checkingLogin_and_Password(listCommand);
        this->sendMessage_ALL_or_oneClient(resultValue,listCommand[3]);
        //ui->terminalChatBrowser->append("Check ---> " + resultValue + "/" + listCommand[3]);
        listCommand.clear();
        break;
    case 0:
        //"exit"
        resultValue = " ";
        resultValue = closeUser(listCommand);
        ui->terminalChatBrowser->append("disconnect: " + resultValue +  " : " + QString::number(socket->socketDescriptor()));
        break;
    default:
        break;
    }
}

int MainWindow::get_commandIndex(QString request)
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

