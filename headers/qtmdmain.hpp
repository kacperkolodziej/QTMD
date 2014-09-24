#ifndef QTMDMAIN_HPP
#define QTMDMAIN_HPP

#include <QMainWindow>
#include <QtNetwork/QSslSocket>

namespace Ui {
class QtmdMain;
}

class QtmdMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit QtmdMain(QWidget *parent = 0);
    ~QtmdMain();

private slots:
    void on_chbEnter_stateChanged(int);

    void on_btnSend_clicked();

private:
    Ui::QtmdMain *ui;
    QSslSocket *socket;
};

#endif // QTMDMAIN_HPP
