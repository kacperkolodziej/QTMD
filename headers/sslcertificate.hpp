#ifndef SSLCERTIFICATE_HPP
#define SSLCERTIFICATE_HPP

#include <QDialog>

namespace Ui {
class SSLCertificate;
}

class SSLCertificate : public QDialog
{
    Q_OBJECT

public:
    explicit SSLCertificate(QWidget *parent = 0);
    ~SSLCertificate();

private:
    Ui::SSLCertificate *ui;
};

#endif // SSLCERTIFICATE_HPP
