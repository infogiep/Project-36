#ifndef REGISTRATIONFORM_H
#define REGISTRATIONFORM_H

#include <QWidget>

namespace Ui {
class RegistrationForm;
}

class RegistrationForm : public QWidget
{
    Q_OBJECT

public:
    explicit RegistrationForm(QWidget *parent = nullptr);
    ~RegistrationForm();

signals:
    void loginRequested();
    void chatStartRequested();
    void exist();

    void checkExistens(QString request);
    void registrationUser(QString request);

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_LoginButton_clicked();

private:
    Ui::RegistrationForm *ui;
};

#endif // REGISTRATIONFORM_H
