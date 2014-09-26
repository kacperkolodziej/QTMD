#include "qtmdmain.hpp"
#include "sslerrors.hpp"
#include "ui_sslerrors.h"
#include "sslcertificate.hpp"

SSLErrors::SSLErrors(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SSLErrors)
{
    ui->setupUi(this);
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui->btnIgnore, SIGNAL(clicked()), this, SLOT(accept()));
}

SSLErrors::~SSLErrors()
{
    delete ui;
}

void SSLErrors::setErrors(const QList<QSslError> errors)
{
    ui->sslErrors->clear();
    foreach (const QSslError &error, errors)
    {
        ui->sslErrors->addItem(error.errorString());
    }
}

void SSLErrors::on_btnCertificate_clicked()
{
    SSLCertificate *certificate = new SSLCertificate(this);
    certificate->setCertificateChain(reinterpret_cast<QtmdMain*>(parentWidget())->getSocket()->peerCertificateChain());
    certificate->exec();
    certificate->deleteLater();
}
