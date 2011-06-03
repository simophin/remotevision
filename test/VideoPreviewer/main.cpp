#include <QtGui/QApplication>
#include "connectdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ConnectDialog w;
    w.show();

    return a.exec();
}
