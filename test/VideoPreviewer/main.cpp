#include <QtGui/QApplication>
#include <QtCore/QTextCodec>
#include "connectdialog.h"

#include "3rdparty/ffmpeg/FFMpeg.h"
#include "platform/posix/PosixCompactHeader.h"
#include <assert.h>

int main(int argc, char *argv[])
{
#ifdef OS_WIN32
	WSAData wsaData;
	int nCode;
	assert( WSAStartup(MAKEWORD(2, 2), &wsaData) == 0);
#endif
    QApplication a(argc, argv);

    FFMpeg::init();

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForTr(codec);

    ConnectDialog w;
    w.show();

    return a.exec();
}
