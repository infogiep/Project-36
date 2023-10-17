#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QString>
#include <QList>
//#include <QtGui>
//#include <QtCore>
#include <QtWidgets>
#include <QMessageBox>
#include <QDebug>
#include "myClassChat/mySQL_db.h"
#include "myClassChat/iTables.h"
#include "myClassChat/db_Table.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(std::vector<std::string> list, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void readSocket();
    void discardSocket();

    void newConnection();

    void addToSocketList(QTcpSocket *socket);
    void sendMessage(QTcpSocket* socket, QString str_msg);

    void sendMessage_ALL_or_oneClient(QString str_msg, QString descriptor);


    void dysplayList(QString item); // Получение списка зарегестрированных пользователей

private:
    Ui::MainWindow *ui;

    QTcpServer *tcpServer;
    QList<QTcpSocket*> listClient;

    QByteArray _data; // для передачи данных между клиентом и сервером

    /*---Работа с БД---*/
    mySQL_db dbConnect;
    iTables* tables[3];
    bool _chatStart;
    //Chat dbChat;

    void databaseQuery(QString requests, QTcpSocket *socket); // метод для отправления запросов в бд
    int get_commandIndex(QString request);

signals:
    void signalStartDB();
};
#endif // MAINWINDOW_H
