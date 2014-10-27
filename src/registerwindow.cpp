#include "headers/registerwindow.hpp"
#include "ui_registerwindow.h"
#include "tamandua/message_composer.hpp"
#include "tamandua/message_buffer.hpp"

#include <QMessageBox>

RegisterWindow::RegisterWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterWindow),
    valid_password(false)
{
    ui->setupUi(this);
}

RegisterWindow::~RegisterWindow()
{
    delete ui;
}

void RegisterWindow::setSocket(QSslSocket *socket_)
{
    socket = socket_;
}

void RegisterWindow::setCommand(QString command_)
{
    command = command_;
}

void RegisterWindow::on_txtPass_textEdited(const QString &arg1)
{
    if (arg1 == ui->txtRepPass->text())
    {
        valid_password = true;
    } else
    {
        valid_password = false;
    }
    ui->btnRegister->setEnabled(valid_password && !ui->txtPass->text().isEmpty());
}

void RegisterWindow::on_txtRepPass_textEdited(const QString &arg1)
{
    if (arg1 == ui->txtPass->text())
    {
        valid_password = true;
    } else
    {
        valid_password = false;
    }
    ui->btnRegister->setEnabled(valid_password && !ui->txtPass->text().isEmpty());
}

void RegisterWindow::on_btnRegister_clicked()
{
    if (socket && socket->state() == QAbstractSocket::ConnectedState)
    {
        tamandua::message_composer msgc(tamandua::standard_message);
        msgc << command.arg(ui->txtPass->text()).toStdString();
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

void RegisterWindow::on_btnCancel_clicked()
{
    close();
}
