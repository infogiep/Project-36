#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QtGui>
#include <QtCore>
#include <QtWidgets>
#include <memory>
#include <QTcpSocket>
#include <QTimer>
#include <future>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //MainWindow(std::vector<std::string> list, QWidget *parent = nullptr);
    MainWindow(std::vector<std::string> list, QWidget *parent = nullptr);
    ~MainWindow();

    void setLoginForm();
    void setRegistrationForm();
    void setWindowChatForm();
    QString getResult(QString str, QString mod);

signals:
    //void resultRequestd_Authorization(QString request);
    void accepted();
    void currentLogin(QString Login);
    //void resultListUser(QString request);
    void ListUser(QString mode);
    void ListSendMessag(QString mode);

    //void updateList();

public slots:
    void requestd_Authorization(QString login, QString pass);
    void request_checkExistens(QString request);
    void requestRegistrationUser(QString request);
    void requestListUser(QString mode);
    void requestSendMessage(QString request);
    void requestListSendMessag(QString mode);

    void requestCloseUser(QString request);

    void requestExist();

private slots:
    void slotReadSocket();
    void disardSocket();
    void sendMessageServer(QTcpSocket *socket, QString str_msg);

    void updateListUser();

private:
    Ui::MainWindow *ui;

    QTcpSocket *tcpSocket;
    QByteArray _data;

    int getCommandIndex(QString request);
};
#endif // MAINWINDOW_H
