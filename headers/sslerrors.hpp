#ifndef SSLERRORS_HPP
#define SSLERRORS_HPP

#include <QDialog>
#include <QSslError>

namespace Ui {
class SSLErrors;
}

class SSLErrors : public QDialog
{
    Q_OBJECT

public:
    explicit SSLErrors(QWidget *parent = 0);
    ~SSLErrors();
    void setErrors(QList<QSslError>);

private slots:
    void on_btnCertificate_clicked();

private:
    Ui::SSLErrors *ui;
};

#endif // SSLERRORS_HPP
