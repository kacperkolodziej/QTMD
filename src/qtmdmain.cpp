#include "qtmdmain.hpp"
#include "ui_qtmdmain.h"
#include "tamandua/message_composer.hpp"
#include <string>
#include <memory>
#include <QMessageBox>

QtmdMain::QtmdMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtmdMain),
    socket(nullptr),
    btnCert(new QToolButton),
    layCert(new QHBoxLayout(ui->txtAddress))
{
    ui->setupUi(this);

    btnCert->setIcon(QIcon(":/imgs/lock.png"));
    int size = ui->txtAddress->height() - 2;
    btnCert->resize(size, size);
    btnCert->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored);
    btnCert->hide();
    layCert->setMargin(ui->txtAddress->style()->pixelMetric(QStyle::PM_DefaultFrameWidth));
    layCert->setSpacing(0);
    layCert->addStretch();
    layCert->addWidget(btnCert);
    ui->txtAddress->setLayout(layCert);
}

QtmdMain::~QtmdMain()
{
    delete ui;
}

void QtmdMain::updateUi()
{
    bool connected = socket && socket->state() == QAbstractSocket::ConnectedState;
    bool unconnected = !connected;

    ui->btnConnect->setEnabled(unconnected && !ui->txtAddress->text().isEmpty());
    ui->txtAddress->setReadOnly(unconnected);
    ui->txtAddress->setFocusPolicy(unconnected ? Qt::StrongFocus : Qt::NoFocus);
    ui->spinPort->setEnabled(unconnected);

    ui->btnSend->setEnabled(connected);
    ui->chbEnter->setEnabled(connected);
}

void QtmdMain::on_chbEnter_stateChanged(int state)
{
    ui->btnSend->setDisabled(state);
}

void QtmdMain::on_btnSend_clicked()
{
    QString msg = ui->txtMessage->toPlainText();
    std::string msg_str = msg.toStdString();
    tamandua::message_composer msgc(tamandua::standard_message, msg_str);
}

void QtmdMain::on_btnConnect_clicked()
{
    if (socket == nullptr)
    {
        socket = new QSslSocket(this);
        connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(socketChangeState(QAbstractSocket::SocketState)));
        connect(socket, SIGNAL(encrypted()), this, SLOT(socketIsEncrypted()));
        connect(socket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(socketSslErrorsOccurred(QList<QSslError>)));
        connect(socket, SIGNAL(readyRead()), this, SLOT(socketReadyToRead()));
    }

    socket->connectToHostEncrypted(ui->txtAddress->text(), ui->spinPort->value());
}

void QtmdMain::socketChangeState(QAbstractSocket::SocketState state)
{
    updateUi();
    if (state == QAbstractSocket::UnconnectedState)
    {
        QMessageBox::information(this, QString("Connection"), QString("Unconnected"));
        ui->txtAddress->setPalette(QPalette());
        ui->txtAddress->setFocus();
    }
}

void QtmdMain::socketIsEncrypted()
{
    // wyczysc karty
}

void QtmdMain::socketReadyToRead()
{
    read_header();
}

void QtmdMain::socketSslErrorsOccurred(QList<QSslError> errors)
{
    /*foreach (const QSslError error, errors) {
        QMessageBox::information(this, QString("SSL ERROR"), error.errorString());
    }*/
    socket->ignoreSslErrors();
}

void QtmdMain::read_header()
{
    int header_size = sizeof(read_message.header);
    if (socket->read(reinterpret_cast<char*>(&read_message.header), header_size) == header_size)
    {
       read_body();
    } else
        QMessageBox::information(this, QString("title"), QString("Error while reading message header"));
}

void QtmdMain::read_body()
{
    int body_size = read_message.header.size;
    std::shared_ptr<char> body_buffer(new char[body_size]);
    if (socket->read(body_buffer.get(), body_size) == 0)
    {
        show_message();
    }
}

void QtmdMain::show_message()
{
    QString msg(read_message.body.data());
    QMessageBox::information(this, QString("Msg"), msg);
}
