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
	Error rc;
	rc = d->mVideoSource->startCapture();
	if (rc.isError()) {
		reportError(tr("开始捕捉：%1").arg(rc.getErrorString().c_str()));
		return;
	}
	d->mTimer.start( 10, this );
}

void VideoPreviewer::stop()
{
	d->mVideoSource->stopCapture();
	d->mTimer.stop();
}

void VideoPreviewer::init() {
	Error rc;
	d->mVideoSource->getImageParam(d->mParam);
	if (d->mParam.pixFmt == IF_INVALID) {
		reportError(tr("无法获得远端图像参数"));
		return;
	}

	if (d->mParam.pixFmt != IF_RGB888) {
		d->mParam.pixFmt = IF_RGB888;
		rc = d->mVideoSource->setImageParam(d->mParam);
		if (rc.isError()) {
			reportError(tr("无法获得远端图像参数: %1").arg(rc.getErrorString().c_str()));
			return;
		}
	}
}

void VideoPreviewer::timerEvent(QTimerEvent *evt)
{
	VideoSource::Buffer buf;
	Error rc = d->mVideoSource->getFilledBuffer(buf);
	if (buf.isValid() && !rc.isError()) {
		QImage img (buf.buf, d->mParam.geo.width, d->mParam.geo.height, QImage::Format_RGB888);
		d->mPixmap = QPixmap::fromImage(img);
		ui->labelContent->setPixmap(d->mPixmap);

		d->mVideoSource->putBuffer(buf);
	}else{
		d->mTimer.stop();
	}

	QWidget::timerEvent(evt);
}

void VideoPreviewer::on_btnStart_clicked()
{
	ui->btnStop->setEnabled(true);
	ui->btnStart->setEnabled(false);
	start();
}

void VideoPreviewer::on_btnStop_clicked()
{
	ui->btnStop->setEnabled(false);
	ui->btnStart->setEnabled(true);
	stop();
}

void VideoPreviewer::closeEvent(QCloseEvent *evt)
{
	stop();
	QWidget::closeEvent(evt);
}

void VideoPreviewer::reportError(const QString &str, bool exit) {
	QMessageBox::critical(this,tr("错误"),str);
	if(exit) close();
}

VideoSource * VideoPreviewer::
getVideoSource() const {
	return d->mVideoSource;
}
