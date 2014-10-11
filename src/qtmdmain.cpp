#include "qtmdmain.hpp"
#include "sslcertificate.hpp"
#include "sslerrors.hpp"
#include "ui_qtmdmain.h"
#include "MessageEdit.hpp"
#include "GroupWidget.hpp"
#include "tamandua/message_composer.hpp"
#include "tamandua/message_buffer.hpp"
#include <string>
#include <memory>
#include <functional>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QScrollBar>
#define DMsg(x) QMessageBox::information(this, QString(), x)

QtmdMain::QtmdMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtmdMain),
    socket(nullptr)
{
    ui->setupUi(this);

    btnCert = new QToolButton(this);
    layCert = new QHBoxLayout(ui->txtAddress);

    msgEdit = new MessageEdit;
    msgEdit->setEnterProcess(true);
    msgEdit->setEnterCallback(std::bind(&QtmdMain::send_message, this));
    ui->gridInput->addWidget(msgEdit, 0, 0);
    laySend = new QGridLayout(msgEdit);
    btnSend = new QToolButton(msgEdit);
    btnSend->setIcon(QIcon(":/imgs/send.png"));
    btnSend->resize(17,17);
    laySend->setSpacing(0);
    laySend->setMargin(msgEdit->style()->pixelMetric(QStyle::PM_DefaultFrameWidth) + 3);
    laySend->addWidget(btnSend, 0, 0, Qt::AlignRight | Qt::AlignBottom);

    connect(btnSend, SIGNAL(clicked()), this, SLOT(btnSendClicked()));

    btnCert->setIcon(QIcon(":/imgs/lock.png"));
    btnCert->setCursor(Qt::ArrowCursor);
    int size = ui->txtAddress->height() - 2;
    btnCert->resize(size, size);
    btnCert->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored);
    btnCert->hide();
    layCert->setMargin(ui->txtAddress->style()->pixelMetric(QStyle::PM_DefaultFrameWidth));
    layCert->setSpacing(0);
    layCert->addStretch();
    layCert->addWidget(btnCert);
    ui->txtAddress->setLayout(layCert);

    connect(btnCert, SIGNAL(clicked()), this, SLOT(btnCertClicked()));
    setupColors();
    updateUi();
}

QtmdMain::~QtmdMain()
{
    delete ui;
}

void QtmdMain::updateUi()
{
    if (socket && socket->state() == QAbstractSocket::ConnectedState)
    {
        ui->btnConnect->setEnabled(false);
        ui->btnDisconnect->setEnabled(true);
        ui->txtAddress->setReadOnly(true);
        ui->spinPort->setReadOnly(true);
        msgEdit->setDisabled(false);
        btnSend->setDisabled(false);
        btnCert->show();
    } else
    {
        ui->btnConnect->setEnabled(true);
        ui->btnDisconnect->setEnabled(false);
        ui->txtAddress->setReadOnly(false);
        ui->spinPort->setReadOnly(false);
        msgEdit->setDisabled(true);
        btnSend->setDisabled(true);
        btnCert->hide();
    }
}

void QtmdMain::setupColors()
{
    nickColor[0] = QString("#f2aca0");
    nickColor[1] = QString("#f20a0a");
    nickColor[2] = QString("#1e8509");
    nickColor[3] = QString("#04d4bb");
    nickColor[4] = QString("#0468d4");
    nickColor[5] = QString("#170875");
    nickColor[6] = QString("#8605e8");
    nickColor[7] = QString("#e414f7");
    nickColor[8] = QString("#3b3d05");
    nickColor[9] = QString("#0a2b29");

    infoColor = QString("#25b507");
    warningColor = QString("#e87800");
    errorColor = QString("#ad0202");

    messageHtml = QString("<time>%1</time> <span class=\"nick\" style=\"color: %2;\">%3</span>: %4<br />");
    infoHtml = QString("<time>%1</time> <span class=\"info\" style=\"color: %2;\">%3</span><br />");
    warningHtml = QString("<time>%1</time> <span class=\"warning\" style=\"color: %2;\">%3</span><br />");
    errorHtml = QString("<time>%1</time> <span class=\"error\" style=\"color: %2;\">%3</span><br />");

    cssStyle = QString("body { margin: 15px 10px; background: #fff; font-family: 'Cantarell', 'Tahoma', 'Verdana'; font-size: 14px; } time { color: #aaa; } .nick { font-weight: bold; } .info { font-weight: 600; } .warning { font-weight: 400 } .error { font-weight: 700; }");
}

void QtmdMain::ignoreSslErrors(bool ignore)
{
    ignoreSslErr = ignore;
}

QSslSocket * QtmdMain::getSocket()
{
    return socket;
}

void QtmdMain::on_btnConnect_clicked()
{
    if (socket == nullptr)
    {
        socket = new QSslSocket(this);
        foreach (const QSslCertificate &cert, QSslCertificate::fromPath(":/certs/*.pem", QSsl::Pem, QRegExp::Wildcard)) {
            socket->addCaCertificate(cert);
        }
        connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(socketChangeState(QAbstractSocket::SocketState)));
        connect(socket, SIGNAL(encrypted()), this, SLOT(socketIsEncrypted()));
        connect(socket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(socketSslErrorsOccurred(QList<QSslError>)));
        connect(socket, SIGNAL(readyRead()), this, SLOT(socketReadyToRead()));
    }

    socket->connectToHostEncrypted(ui->txtAddress->text(), ui->spinPort->value());
    updateUi();
}

void QtmdMain::socketChangeState(QAbstractSocket::SocketState state)
{
    updateUi();
    ui->statusBar->clearMessage();
    if (state == QAbstractSocket::UnconnectedState)
    {
        ui->txtAddress->setPalette(QPalette());
        ui->txtAddress->setFocus();
    } else if (state == QAbstractSocket::HostLookupState)
    {
        ui->statusBar->showMessage(QString("Looking for server... please wait..."));
    } else if (state == QAbstractSocket::ConnectingState)
    {
        ui->statusBar->showMessage(QString("Connecting to server... please wait..."));
    } else if (state == QAbstractSocket::ConnectedState)
    {
        QPalette palette;
        palette.setColor(QPalette::Base, QColor(255, 255, 192));
        ui->txtAddress->setPalette(palette);
        btnCert->show();
        ui->statusBar->showMessage(QString("Connected!"));
    } else if (state == QAbstractSocket::UnconnectedState)
    {
        ui->statusBar->showMessage(QString("Disconnected"));
    }
}

void QtmdMain::socketIsEncrypted()
{
    QMessageBox::information(this, QString("Signal"), QString("Secure connection established!"));
    clear_tabs();
}

void QtmdMain::socketReadyToRead()
{
    read_header();
}

void QtmdMain::socketSslErrorsOccurred(QList<QSslError> errors)
{
    SSLErrors *errDialog = new SSLErrors(this);
    errDialog->setErrors(errors);
    if (errDialog->exec() == QDialog::Accepted)
        socket->ignoreSslErrors();
    else
        QMessageBox::information(this, QString("Connection"), QString("Connecting aborted!"));
}

void QtmdMain::btnSendClicked()
{
    send_message();
}

void QtmdMain::btnCertClicked()
{
    SSLCertificate *certinfo = new SSLCertificate(this);
    certinfo->setCertificateChain(socket->peerCertificateChain());
    certinfo->exec();
    certinfo->deleteLater();
}

void QtmdMain::read_header()
{
    int header_size = sizeof(read_message.header);
    if (socket->read(reinterpret_cast<char*>(&read_message.header), header_size) == header_size)
    {
       read_body();
    }
}

void QtmdMain::read_body()
{
    int body_size = read_message.header.size;
    std::shared_ptr<char> body_buffer(new char[body_size]);
    if (socket->read(body_buffer.get(), body_size) == body_size)
    {
        tamandua::message_type type = read_message.header.type;
        read_message.body = std::string(body_buffer.get(), body_size);
        if (type == tamandua::standard_message || type == tamandua::info_message || type == tamandua::error_message || type == tamandua::warning_message)
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
    QString author;
    if (iterator == users.end())
        author = "[server]";
    else
        author = iterator->second;
    auto insp = messages.insert(std::make_pair(hash_str, nick_message(author, read_message)));
    if (insp.second == true)
    {
        messages_hashes.insert(std::make_pair(read_message.header.utc_time, hash_str));
        auto tab = tabs.find(read_message.header.group);
        tab->second->getTextBrowser()->setHtml(generate_html(read_message.header.group));
        tab->second->getTextBrowser()->verticalScrollBar()->setValue(tab->second->getTextBrowser()->verticalScrollBar()->maximum());
    }
}

void QtmdMain::send_message()
{
    std::string msg_body = msgEdit->toPlainText().toStdString();
    int tab_index = ui->tabs->currentIndex();
    tamandua::id_number_t gid = reinterpret_cast<GroupWidget*>(ui->tabs->currentWidget())->getGid();
    tamandua::message_composer msgc(tamandua::standard_message, msg_body, gid);
    tamandua::message msg = msgc();
    tamandua::message_buffer msg_buf(msg.header, msg.body);
    if (socket->write(msg_buf.get_buffer().get(), msg_buf.get_buffer_size()) == static_cast<qint64>(msg_buf.get_buffer_size()))
    {
        msgEdit->clear();
    } else
    {
        DMsg(QString("Error while sending message!"));
    }

}

void QtmdMain::set_users()
{
    QString all_users = QString::fromStdString(read_message.body);
    QStringList users_list = all_users.split(';', QString::SkipEmptyParts);
    ui->listUsers->clear();
    users.clear();
    foreach (const QString user, users_list)
    {
        QStringList user_data = user.split(':');
        tamandua::id_number_t uid = user_data[0].toLongLong();
        QString uname = user_data[1];
        users.insert(std::make_pair(uid, uname));
        ui->listUsers->addItem(uname);
    }
}

void QtmdMain::set_rooms()
{
    QString all_rooms = QString::fromStdString(read_message.body);
    QStringList rooms_list = all_rooms.split(';', QString::SkipEmptyParts);
    ui->listRooms->clear();
    rooms.clear();
    foreach (const QString room, rooms_list)
    {
        QStringList room_data = room.split(':');
        tamandua::id_number_t gid = room_data[0].toLongLong();
        QString gname = room_data[1];
        rooms.insert(std::make_pair(gid, gname));
        ui->listRooms->addItem(gname);
    }
}

void QtmdMain::create_tab()
{
    GroupWidget *tab = new GroupWidget(ui->tabs, read_message.header.group, QString::fromStdString(read_message.body));
    int index = ui->tabs->addTab(tab, tab->getName());
    tabs.insert(std::make_pair(tab->getGid(), tab));
    ui->tabs->setCurrentIndex(index);
}

void QtmdMain::remove_tab()
{
    auto tab_it = tabs.find(read_message.header.group);
    if (tab_it == tabs.end())
    {
        DMsg("Tab not found!");
        return;
    }
    int index = ui->tabs->indexOf(tab_it->second);
    ui->tabs->removeTab(index);
    tabs.erase(tab_it);
}

void QtmdMain::clear_tabs()
{
    for (auto tab_it = tabs.begin(); tab_it != tabs.end(); ++tab_it)
    {
        int index = ui->tabs->indexOf(tab_it->second);
        ui->tabs->removeTab(index);
        tabs.erase(tab_it);
    }
}

QString QtmdMain::generate_html(tamandua::id_number_t gid)
{
    auto tab = tabs.find(gid);
    if (tab == tabs.end())
        return QString("<h1>Not found!</h1>");

    QString html;
    html += QString("<style type=\"text/css\">%1</style>").arg(cssStyle);
    for (auto msgh = messages_hashes.begin(); msgh != messages_hashes.end(); ++msgh)
    {
        auto msgp = messages.find(msgh->second);
        tamandua::message msg = msgp->second.msg;
        if (msg.header.group != gid)
            continue;

        QString author = msgp->second.nick;
        tamandua::message_type type = msg.header.type;

        QDateTime datetime = QDateTime::fromTime_t(msg.header.utc_time);
        datetime = datetime.toLocalTime();
        QDate date = datetime.date(), today_date = QDate::currentDate();
        QString timeStr;
        bool today = (today_date.year() == date.year() && today_date.month() == date.month() && today_date.day() == date.day());
        if (today)
            timeStr = datetime.time().toString("hh:mm:ss");
        else
            timeStr = datetime.date().toString("dd/MM/yyyy ") + datetime.time().toString("hh:mm:ss");

        if (type == tamandua::standard_message)
        {
            html += messageHtml.arg(timeStr).arg(nickColor[msg.header.author % 10]).arg(author).arg(QString::fromStdString(msg.body));
        } else if (type == tamandua::info_message)
        {
            html += infoHtml.arg(timeStr).arg(infoColor).arg(QString::fromStdString(msg.body));
        } else if (type == tamandua::warning_message)
        {
            html += warningHtml.arg(timeStr).arg(warningColor).arg(QString::fromStdString(msg.body));
        } else if (type == tamandua::error_message)
        {
            html += errorHtml.arg(timeStr).arg(errorColor).arg(QString::fromStdString(msg.body));
        }
    }
    return html;
}

void QtmdMain::on_btnDisconnect_clicked()
{
    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        socket->disconnectFromHost();
        updateUi();
    }
}
