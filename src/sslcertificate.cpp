#include "headers/sslcertificate.hpp"
#include "ui_sslcertificate.h"

SSLCertificate::SSLCertificate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SSLCertificate)
{
    ui->setupUi(this);
}

SSLCertificate::~SSLCertificate()
{
    delete ui;
}
