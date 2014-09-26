#ifndef SSLCERTIFICATE_HPP
#define SSLCERTIFICATE_HPP

#include <QDialog>
#include <QList>
#include <QSslCertificate>

namespace Ui {
class SSLCertificate;
}

class SSLCertificate : public QDialog
{
    Q_OBJECT

public:
    explicit SSLCertificate(QWidget *parent = 0);
    ~SSLCertificate();

    void setCertificateChain(QList<QSslCertificate>);

private slots:
    void on_btnClose_clicked();
    void updateCertificateInfo(int);

private:
    Ui::SSLCertificate *ui;
    QList<QSslCertificate> chain;
};

#endif // SSLCERTIFICATE_HPP
