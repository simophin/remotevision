#include <QtGui/QApplication>
#include <QtCore/QTextCodec>
#include "connectdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForTr(codec);

    ConnectDialog w;
    w.show();

    return a.exec();
}
