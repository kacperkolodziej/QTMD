#ifndef QTMDMAIN_HPP
#define QTMDMAIN_HPP

#include <QMainWindow>
#include <QToolButton>
#include <QHBoxLayout>
#include <QtNetwork/QSslSocket>
#include "tamandua/message.hpp"

namespace Ui {
class QtmdMain;
}

class QtmdMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit QtmdMain(QWidget *parent = 0);
    ~QtmdMain();
    void updateUi();

private slots:
    void on_chbEnter_stateChanged(int);
    void on_btnSend_clicked();
    void on_btnConnect_clicked();
    void socketChangeState(QAbstractSocket::SocketState);
    void socketIsEncrypted();
    void socketReadyToRead();
    void socketSslErrorsOccurred(QList<QSslError>);

private:
    void read_header();
    void read_body();
    void show_message();
    //void write_message(tamandua::message &);

private:
    Ui::QtmdMain *ui;
    QSslSocket *socket;
    QToolButton *btnCert;
    QHBoxLayout *layCert;
    tamandua::message read_message;
};

#endif // QTMDMAIN_HPP
