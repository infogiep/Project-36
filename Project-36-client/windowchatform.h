#ifndef WINDOWCHATFORM_H
#define WINDOWCHATFORM_H

#include <QMessageBox>
#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QListWidget>

namespace Ui {
class windowChatForm;
}

class windowChatForm : public QWidget
{
    Q_OBJECT

public:
    explicit windowChatForm(QWidget *parent = nullptr);
    ~windowChatForm();

signals:
    void loginRequested(); // Из окна чата в окно логин
    //void ListUser(QString mode);
    void sendMessage(QString request);
    void closeUser(QString request);

    //void updateListUser(QString mode);

public slots:
    void getCurrentLogin(QString Login);
    //void getCurrentList(QString request);
    QString getLogin();
    QString getListUser();
    void setListUser(QString request);
    void setListMessages(QString request);

    //void requestUpdateList(QString nLogin);

    void addNewUser(QString nLogin);

private slots:
    void on_closeClientButton_clicked();
    void on_messagesLineEdit_returnPressed();
    void on_senMessagesButton_clicked();

private:
    Ui::windowChatForm *ui;
    QString _Login;
    QString listUser;
    QString listMessages;
};

#endif // WINDOWCHATFORM_H
