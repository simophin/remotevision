#include "connectdialog.h"
#include "ui_connectdialog.h"

#include "ipvalidator.h"
#include "videopreviewer.h"

#include "vsources/IOVideoSource.h"
#include "medium/TCPSocket.h"
#include "medium/TCPSocketAddress.h"
#include "platform/posix/PosixCompactHeader.h"
#include "connectthread.h"


#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpSocket>
#include <QtGui/QMessageBox>
#include <QtGui/QIntValidator>
#include <QtCore/QVector>

#include <errno.h>
#include <string.h>


class ConnectDialog::Impl {
public:
	ConnectThread connThread;

	Impl (QObject *parent)
	:connThread(parent) {

	}
};

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog),
    d(new Impl(parent))
{
    ui->setupUi(this);
    init();
}

ConnectDialog::~ConnectDialog()
{
    delete ui;
    delete d;
}

void ConnectDialog::init()
{
    ui->portEdit->setFocus();

    connect (&d->connThread, SIGNAL(success()),
    		this, SLOT(onRemoteConnectSuccess()));
    connect (&d->connThread, SIGNAL(error(QString)),
    		this, SLOT(onRemoteConnectError(QString)));
}

void ConnectDialog::on_btnConnect_clicked()
{

	if (ui->portEdit->text().trimmed().isEmpty()){
		QMessageBox::critical(this,tr("错误"),tr("输入的端口号不正确，请重新输入"));
		ui->portEdit->setFocus();
		return;
	}

	ui->btnConnect->setText(tr("连接中..."));
	ui->btnConnect->setEnabled(false);

	d->connThread.establishConnection(ui->ipEdit->text(),ui->portEdit->text().toInt());
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

void ConnectDialog::onRemoteConnectSuccess()
{
	ConnectThread *t = qobject_cast<ConnectThread *> (sender());
	Q_ASSERT(t!=NULL);

	VideoPreviewer *previewer = new VideoPreviewer(t->getVideoSource(),NULL);
	previewer->setAttribute(Qt::WA_DeleteOnClose,true);
	previewer->setWindowTitle(QString("%1:%2").arg(ui->ipEdit->text(), ui->portEdit->text()));
	connect (previewer,SIGNAL(destroyed(QObject *)), this,SLOT(onVideoPreviewerDestroyed(QObject *)));

	hide();
	previewer->show();
}



void ConnectDialog::onRemoteConnectError(QString qString)
{
	resetUI();
	QMessageBox::critical(this,tr("连接失败"),qString);
}

