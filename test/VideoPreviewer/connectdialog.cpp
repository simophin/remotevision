#include "connectdialog.h"
#include "ui_connectdialog.h"

#include "ipvalidator.h"
#include "videopreviewer.h"

#include "vsources/IOVideoSource.h"
#include "medium/TCPSocket.h"
#include "medium/TCPSocketAddress.h"


#include <QtNetwork/QHostAddress>
#include <QtGui/QMessageBox>
#include <QtGui/QIntValidator>

#include <errno.h>
#include <string.h>

class ConnectDialog::Impl {
public:
    IPValidator validator;
    QIntValidator portValidator;
};

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog),
    d(new Impl)
{
    ui->setupUi(this);
    init();
}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}

void ConnectDialog::init()
{
	d->portValidator.setRange(0,100000);

    ui->ipEdit->setValidator(&d->validator);
    ui->portEdit->setValidator(&d->portValidator);
}

void ConnectDialog::on_btnConnect_clicked()
{
	QHostAddress qtAddr;
	if (!qtAddr.setAddress(ui->ipEdit->text()) ||
			qtAddr.isNull()) {
		QMessageBox::critical(this,tr("错误"),tr("输入的IP地址不正确，请重新输入"));
		ui->ipEdit->setFocus();
		return;
	}
	if (ui->portEdit->text().trimmed().isEmpty()){
		QMessageBox::critical(this,tr("错误"),tr("输入的端口号不正确，请重新输入"));
		ui->portEdit->setFocus();
		return;
	}

	ui->btnConnect->setText(tr("连接中..."));
	ui->btnConnect->setEnabled(false);


	int csock = ::socket(AF_INET, SOCK_STREAM, 0),
			dsock = ::socket(AF_INET, SOCK_STREAM, 0);
	TCPSocketAddress remoteAddr (ui->ipEdit->text().toStdString(),ui->portEdit->text().toInt());
	TCPSocket *ctrl = new TCPSocket(csock), *data = new TCPSocket(dsock);
	int rc;

	if ( (rc = ctrl->connect((SocketAddress *)&remoteAddr)) != 0) {
		QMessageBox::critical(this,tr("连接失败"),tr("连接控制接口失败，原因为：%1").arg(ctrl->getLastError().getErrorString().c_str()));
		resetUI();
		return;
	}

	if ( (rc = data->connect((SocketAddress *)&remoteAddr)) != 0) {
		QMessageBox::critical(this,tr("连接失败"),tr("连接数据失败，原因为：%1").arg(data->getLastError().getErrorString().c_str()));
		resetUI();
		return;
	}

	IOVideoSource *src = new IOVideoSource(ctrl,data);

	if (!src->init()) {
		QMessageBox::critical(this,tr("连接失败"),tr("初始化视频源失败，原因为：%1").arg(src->getLastError().getErrorString().c_str()));
		delete src;
		resetUI();
		return;
	}

	VideoPreviewer *previewer = new VideoPreviewer(src,NULL);
	previewer->setAttribute(Qt::WA_DeleteOnClose,true);
	this->connect (previewer,SIGNAL(destroyed(QObject *)), this,SLOT(onVideoPreviewerDestroyed(QObject *)));

	accept();
	previewer->show();
}

void ConnectDialog::onVideoPreviewerDestroyed(QObject * obj) {
	VideoPreviewer *previewer = qobject_cast<VideoPreviewer *>(obj);
	if (!previewer) return;

	IOVideoSource *src = static_cast<IOVideoSource *>(previewer->getVideoSource());
	delete src->getControlDevice();
	delete src->getDataDevice();
	delete src;

	resetUI();
	show();
}

void ConnectDialog::resetUI() {
	ui->btnConnect->setText(tr("连接"));
	ui->btnConnect->setEnabled(true);
}
