#ifndef LOGINWINDOW_HPP
#define LOGINWINDOW_HPP

#include <QDialog>
#include <QSslSocket>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();
    void setSocket(QSslSocket*);
    void setCommand(QString);

private slots:
    void on_btnLogin_clicked();

    void on_btnCancel_clicked();

private:
    Ui::LoginWindow *ui;
    QSslSocket *socket;
    QString command;
};

#endif // LOGINWINDOW_HPP
