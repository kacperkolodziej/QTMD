#include "qtmdmain.hpp"
#include "ui_qtmdmain.h"
#include "tamandua/message_composer.hpp"
#include <string>

QtmdMain::QtmdMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtmdMain)
{
    ui->setupUi(this);
}

QtmdMain::~QtmdMain()
{
    delete ui;
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
