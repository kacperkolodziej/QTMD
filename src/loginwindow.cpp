#include "headers/loginwindow.hpp"
#include "ui_loginwindow.h"
#include "tamandua/message_composer.hpp"
#include "tamandua/message_buffer.hpp"

#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::setSocket(QSslSocket *socket_)
{
    socket = socket_;
}

void LoginWindow::setCommand(QString command_)
{
    command = command_;
}

void LoginWindow::on_btnLogin_clicked()
{
    if (socket && socket->state() == QAbstractSocket::ConnectedState)
    {
        tamandua::message_composer msgc(tamandua::standard_message);
        msgc << command.arg(ui->txtLogin->text(), ui->txtPassword->text()).toStdString();
        tamandua::message msg = msgc();
        tamandua::message_buffer msg_buf(msg.header, msg.body);
        if (socket->write(msg_buf.get_buffer().get(), msg_buf.get_buffer_size()) == static_cast<qint64>(msg_buf.get_buffer_size()))
        {
            close();
        } else
        {
            QMessageBox::information(this, QString("Error"), QString("An error occurred while sending message! PLease try again!"));
        }
    } else
    {
        QMessageBox::information(this, QString("Error"), QString("You must be connected to server!"));
    }
}

void LoginWindow::on_btnCancel_clicked()
{
    close();
}
