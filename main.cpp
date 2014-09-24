#include "qtmdmain.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtmdMain w;
    w.show();

    return a.exec();
}
