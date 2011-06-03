#include "videopreviewer.h"
#include "ui_videopreviewer.h"

#include "VideoSource.h"

#include <QtGui/QPixmap>
#include <QtGui/QImage>
#include <QtCore/QBasicTimer>
#include <QtGui/QMessageBox>

class VideoPreviewer::Impl {
public:
	VideoSource *mVideoSource;
	QBasicTimer mTimer;
	VideoSource::ImageParam mParam;
	QPixmap mPixmap;

	Impl(VideoSource *src)
	:mVideoSource(src){

	}
};

VideoPreviewer::VideoPreviewer(VideoSource *src,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoPreviewer),
    d(new Impl(src))
{
    ui->setupUi(this);
    init();
}

VideoPreviewer::~VideoPreviewer()
{
    delete ui;
    delete d;
}
void VideoPreviewer::start()
{
	d->mTimer.start( d->mParam.fps.den*1000/d->mParam.fps.num, this );
}

void VideoPreviewer::stop()
{
	d->mTimer.stop();
}

void VideoPreviewer::init() {
	d->mParam = d->mVideoSource->getImageParam();
	if (d->mParam.pixFmt == IF_INVALID) {
		reportError(tr("无法获得远端图像参数: %1").arg(d->mVideoSource->getLastError().getErrorString().c_str()));
		return;
	}

	if (d->mParam.pixFmt != IF_RGB565) {
		d->mParam.pixFmt = IF_RGB565;
		if (!d->mVideoSource->setImageParam(d->mParam)) {
			reportError(tr("无法获得远端图像参数: %1").arg(d->mVideoSource->getLastError().getErrorString().c_str()));
			return;
		}
	}
}

void VideoPreviewer::timerEvent(QTimerEvent *evt)
{
	VideoSource::Buffer buf = d->mVideoSource->getFilledBuffer();
	if (buf.isValid()) {
		QImage img (buf.buf, d->mParam.geo.width, d->mParam.geo.height, QImage::Format_RGB555);
		d->mPixmap = QPixmap::fromImage(img);
		ui->labelContent->setPixmap(d->mPixmap);
	}

	VideoPreviewer::timerEvent(evt);
}

void VideoPreviewer::reportError(const QString &str, bool exit) {
	QMessageBox::critical(this,tr("错误"),str);
	if(exit) close();
}
