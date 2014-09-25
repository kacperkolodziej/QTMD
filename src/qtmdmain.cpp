#include "qtmdmain.hpp"
#include "ui_qtmdmain.h"
#include "tamandua/message_composer.hpp"
#include "tamandua/message_buffer.hpp"
#include <string>
#include <memory>
#include <QMessageBox>
#include <QCryptographicHash>

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

    ui->btnSend->setEnabled(!ui->chbEnter->isChecked() && connected);
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
    if (socket->read(body_buffer.get(), body_size) == body_size)
    {
        tamandua::message_type type = read_message.header.type;
        if (type == tamandua::standard_message)
            add_message();
        else if (type == tamandua::rooms_list)
            set_rooms();
        else if (type == tamandua::participants_list)
            set_users();
        else if (type == tamandua::group_enter_message)
            create_tab();
        else if (type == tamandua::group_leave_message)
            remove_tab();
    }
}

void QtmdMain::add_message()
{
    tamandua::message_buffer buffer(read_message.header, read_message.body);
    QCryptographicHash hash(QCryptographicHash::Sha512);
    hash.addData(buffer.get_buffer().get(), buffer.get_buffer_size());
    QString hash_str(hash.result());
    auto iterator = users.find(read_message.header.author);
    if (iterator == users.end())
        return;

    QString author(iterator->second.data());
    messages.insert(std::make_pair(hash_str, nick_message(author, read_message)));
    messages_hashes.insert(std::make_pair(read_message.header.utc_time, hash_str));
}

void QtmdMain::send_message()
{

}

void QtmdMain::set_users()
{
    QString all_users = QString::fromStdString(read_message.body);
    QStringList users_list = all_users.split(';', QString::SkipEmptyParts);
    foreach (const QString user, users_list)
    {
        QStringList user_data = user.split(':');
        tamandua::id_number_t uid = user_data[0].toLongLong();
        QString uname = user_data[1];
        users.insert(std::make_pair(uid, uname));
    }
}

void QtmdMain::set_rooms()
{
    QString all_rooms = QString::fromStdString(read_message.body);
    QStringList rooms_list = all_rooms.split(';', QString::SkipEmptyParts);
    foreach (const QString room, rooms_list)
    {
        QStringList room_data = room.split(':');
        tamandua::id_number_t gid = room_data[0].toLongLong();
        QString gname = room_data[1];
        rooms.insert(std::make_pair(gid, gname));
    }
}

void QtmdMain::create_tab()
{
    tab_elements tab;
    tab.gid = read_message.header.group;
    tab.name = QString::fromStdString(read_message.body);
    tab.tab = new QWidget(ui->tabs);
    tab.layout = new QVBoxLayout(tab.tab);
    tab.web = new QWebView(tab.tab);
    tab.layout->addWidget(tab.web);
    tab.tab_index = ui->tabs->addTab(tab.tab, tab.name);
    tabs.insert(std::make_pair(tab.gid, tab));
}

void QtmdMain::remove_tab()
{

}
