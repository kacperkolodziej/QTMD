#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QDialog>
#include <QSslSocket>

namespace Ui {
class RegisterWindow;
}

class RegisterWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterWindow(QWidget *parent = 0);
    ~RegisterWindow();
    void setSocket(QSslSocket*);
    void setCommand(QString);

private slots:
    void on_txtPass_textEdited(const QString &arg1);

    void on_txtRepPass_textEdited(const QString &arg1);

    void on_btnRegister_clicked();

    void on_btnCancel_clicked();

private:
    Ui::RegisterWindow *ui;
    QSslSocket* socket;
    QString command;
    bool valid_password;
};

#endif // REGISTERWINDOW_H
