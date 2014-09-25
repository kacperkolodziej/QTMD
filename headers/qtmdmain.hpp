#ifndef QTMDMAIN_HPP
#define QTMDMAIN_HPP

#include <QMainWindow>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWebView>
#include <QtNetwork/QSslSocket>
#include <map>
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
    void add_message();
    void send_message();
    void set_users();
    void set_rooms();

    void create_tab();
    void remove_tab();

private:
    struct nick_message {
        QString nick;
        tamandua::message msg;
        nick_message(QString p_nick, tamandua::message p_msg) :
                nick(p_nick),
                msg(p_msg)
        {}
    };
    struct tab_elements {
        tamandua::id_number_t gid;
        int tab_index;
        QString name;
        QWidget *tab;
        QVBoxLayout *layout;
        QWebView *web;
    };

    Ui::QtmdMain *ui;
    QSslSocket *socket;
    QToolButton *btnCert;
    QHBoxLayout *layCert;

    tamandua::message read_message;
    std::map<QString, nick_message> messages;
    std::multimap<tamandua::message_time_t, QString> messages_hashes;
    std::map<tamandua::id_number_t, QString> users;
    std::map<tamandua::id_number_t, QString> rooms;

    std::map<tamandua::id_number_t, tab_elements> tabs;
};

#endif // QTMDMAIN_HPP
