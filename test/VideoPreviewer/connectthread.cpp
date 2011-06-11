/*
 * connectthread.cpp
 *
 *  Created on: 2011-6-11
 *      Author: simophin
 */

#include "connectthread.h"
#include "medium/TCPSocket.h"
#include "medium/TCPSocketAddress.h"
#include "vsources/IOVideoSource.h"

#include <QtNetwork/QHostAddress>
#include <QtNetwork/QHostInfo>

#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>

ConnectThread::ConnectThread(QObject *parent)
:QThread(parent),
 mVideoSource(0),
 mShouldStop(false){

}

ConnectThread::~ConnectThread() {
}

void ConnectThread::run()
{
	QHostInfo addr = QHostInfo::fromName(mAddress);
	if (addr.error() != QHostInfo::NoError) {
		Q_EMIT error(addr.errorString());
		return;
	}

	QHostAddress actualAddr;
	QList<QHostAddress> addresses = addr.addresses();
	for (QList<QHostAddress>::const_iterator iter = addresses.begin();
			iter != addresses.end(); ++ iter) {
		if (iter->protocol() == QAbstractSocket::IPv4Protocol) {
			actualAddr = *iter;
			break;
		}
	}

	if (actualAddr.isNull()) {
		Q_EMIT error(tr("无法解析域名: 找不到'%1'的IPv4地址").arg(mAddress));
		return;
	}

	// Begin connection
	{
		std::string addr = actualAddr.toString().toStdString();
		TCPSocketAddress remote (addr,mPort);
		IOVideoSource *src = 0;

		Error rc;
		TCPSocket *ctrl = new TCPSocket (::socket(AF_INET, SOCK_STREAM, 0));
		TCPSocket *data = new TCPSocket (::socket(AF_INET, SOCK_STREAM, 0));

		int flags = fcntl(ctrl->getFileDescriptor(),F_GETFL, 0);
		fcntl(ctrl->getFileDescriptor(),F_SETFL, flags | O_NONBLOCK);
		fcntl(data->getFileDescriptor(),F_SETFL, flags | O_NONBLOCK);

		ctrl->connect(&remote);
		do {
			rc = ctrl->poll(IODevice::POLL_WRITE,200);
			if (rc.isSuccess()) break;
			else if (rc.getErrorType() == Error::ERR_TIMEOUT) continue;
			else {
				rc.setErrorString("无法连接到远端服务器");
				goto connect_ctrl_failed;
			}
		}while(!mShouldStop);

		data->connect(&remote);
		do {
			rc = data->poll(IODevice::POLL_WRITE,200);
			if (rc.isSuccess()) break;
			else if (rc.getErrorType() == Error::ERR_TIMEOUT) continue;
			else {
				rc.setErrorString("无法连接到远端服务器");
				goto connect_data_failed;
			}
		}while(!mShouldStop);

		fcntl(ctrl->getFileDescriptor(),F_SETFL, flags & ~O_NONBLOCK);
		fcntl(data->getFileDescriptor(),F_SETFL, flags & ~O_NONBLOCK);


		src = new IOVideoSource(ctrl,data);
		rc = src->init();
		if (rc.isError()) {
			rc.setErrorString("初始化视频源失败，原因为：" + rc.getErrorString());
			goto init_video_source_failed;
		}


		mVideoSource = src;
		Q_EMIT success();
		return;

		init_video_source_failed:
		delete src;
		connect_data_failed:
		connect_ctrl_failed:
		ctrl->close();
		data->close();
		delete ctrl;
		delete data;

		Q_EMIT error(QString::fromStdString(rc.getErrorString()));

	}
}


void ConnectThread::establishConnection(const QString & addr, int port)
{
	mAddress = addr;
	mPort = port;

	if (isRunning()) wait();
	start();
}

VideoSource *ConnectThread::getVideoSource() const
{
	return mVideoSource;
}





