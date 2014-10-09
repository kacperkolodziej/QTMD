#include "headers/sslcertificate.hpp"
#include "ui_sslcertificate.h"
#include <QSslCertificate>
#include <QMessageBox>

SSLCertificate::SSLCertificate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SSLCertificate)
{
    ui->setupUi(this);

    connect(ui->certificates, SIGNAL(currentIndexChanged(int)), this, SLOT(updateCertificateInfo(int)));
}

SSLCertificate::~SSLCertificate()
{
    delete ui;
}

void SSLCertificate::setCertificateChain(QList<QSslCertificate> p_chain)
{
    chain = p_chain;
    ui->certificates->clear();
    for (int i = 0; i < chain.size(); ++i)
    {
        const QSslCertificate &cert = chain.at(i);
        ui->certificates->addItem(QString("%1%2 (%3)")
                                   .arg(!i ? QString() : QString("Issuer: "))
                                   .arg(cert.subjectInfo(QSslCertificate::Organization).join(' '))
                                   .arg(cert.subjectInfo(QSslCertificate::CommonName).join(' '))
                                  );
    }
    ui->certificates->setCurrentIndex(0);
}

void SSLCertificate::on_btnClose_clicked()
{
    close();
}

void SSLCertificate::updateCertificateInfo(int index)
{
    if (index >= 0 && index < chain.size())
    {
        ui->certificateInfo->clear();
        const QSslCertificate &cert = chain.at(index);
        QStringList list;
        list << QString("Organisation: %1").arg(cert.subjectInfo(QSslCertificate::Organization).join(' '))
            << QString("Unit name: %1").arg(cert.subjectInfo(QSslCertificate::OrganizationalUnitName).join(' '))
            << QString("Common name: %1").arg(cert.subjectInfo(QSslCertificate::CommonName).join(' '))
            << QString("Locality: %1").arg(cert.subjectInfo(QSslCertificate::LocalityName).join(' '))
            << QString("Country: %1").arg(cert.subjectInfo(QSslCertificate::CountryName).join(' '))
            << QString("State or province: %1").arg(cert.subjectInfo(QSslCertificate::StateOrProvinceName).join(' '))
            << QString()
            << QString("Issued by:")
            << QString("Organisation: %1").arg(cert.issuerInfo(QSslCertificate::Organization).join(' '))
            << QString("Unit name: %1").arg(cert.issuerInfo(QSslCertificate::OrganizationalUnitName).join(' '))
            << QString("Common name: %1").arg(cert.issuerInfo(QSslCertificate::CommonName).join(' '))
            << QString("Locality: %1").arg(cert.issuerInfo(QSslCertificate::LocalityName).join(' '))
            << QString("Country: %1").arg(cert.issuerInfo(QSslCertificate::CountryName).join(' '))
            << QString("State or province: %1").arg(cert.issuerInfo(QSslCertificate::StateOrProvinceName).join(' '));
        foreach (QString line, list)
            ui->certificateInfo->addItem(line);
    } else
    {
        QMessageBox::information(this, QString("Error"), QString("Bad index!"));
    }
}
