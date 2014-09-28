#include "qtmdmain.hpp"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if (!QSslSocket::supportsSsl()) {
        QMessageBox::information(0, "QTMD",
                                 "This system does not support OpenSSL.");
        return -1;
    }
    QtmdMain w;
    w.show();

    return a.exec();
}
