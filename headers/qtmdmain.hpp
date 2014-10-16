#ifndef QTMDMAIN_HPP
#define QTMDMAIN_HPP

#include <QMainWindow>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QTextBrowser>
#include <QtNetwork/QSslSocket>
#include <QListWidget>
#include <QSystemTrayIcon>
#include <map>
#include "MessageEdit.hpp"
#include "GroupWidget.hpp"
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
    void setupColors();

    void ignoreSslErrors(bool);
    void keyPressEvent(QKeyEvent*);
    void closeEvent(QCloseEvent*);
    QSslSocket * getSocket();

private slots:
    void on_btnConnect_clicked();
    void socketChangeState(QAbstractSocket::SocketState);
    void socketIsEncrypted();
    void socketReadyToRead();
    void socketSslErrorsOccurred(QList<QSslError>);
    void btnSendClicked();
    void btnCertClicked();

    void on_btnDisconnect_clicked();
    void on_listRooms_itemDoubleClicked(QListWidgetItem *item);
    void on_tabs_tabBarDoubleClicked(int index);
    void on_listUsers_itemDoubleClicked(QListWidgetItem *item);
    void on_actionAbout_QTMD_triggered();
    void on_actionAbout_Tamandua_triggered();
    void on_actionAbout_Author_triggered();
    void on_actionUpdate_QTMD_triggered();
    void on_actionQuit_triggered();

private:
    void read_header();
    void read_body();
    void add_message();
    void send_message();
    void set_users();
    void set_rooms();
    void disconnect_socket();

    void create_tab();
    void remove_tab();
    void clear_tabs();
    void clear_message_cache();

    QString generate_html(tamandua::id_number_t);

private:
    struct nick_message {
        QString nick;
        tamandua::message msg;
        nick_message(QString p_nick, tamandua::message p_msg) :
                nick(p_nick),
                msg(p_msg)
        {}
    };

    Ui::QtmdMain *ui;
    QSslSocket *socket;
    QToolButton *btnCert;
    QHBoxLayout *layCert;
    QGridLayout *laySend;
    QToolButton *btnSend;
    MessageEdit *msgEdit;
    QSystemTrayIcon *trayIcon;

    QString nickColor[10], infoColor, warningColor, errorColor;
    QString messageHtml, infoHtml, warningHtml, errorHtml;
    QString cssStyle;

    tamandua::message read_message;
    std::map<QString, nick_message> messages;
    std::multimap<tamandua::message_time_t, QString> messages_hashes;
    std::map<tamandua::id_number_t, QString> users;
    std::map<tamandua::id_number_t, QString> rooms;

    std::map<tamandua::id_number_t, GroupWidget*> tabs;
    char command_char;

    bool ignoreSslErr;
};

#endif // QTMDMAIN_HPP
