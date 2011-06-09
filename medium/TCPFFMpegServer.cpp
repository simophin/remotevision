/*
 * TCPFFMpegServer.cpp
 *
 *  Created on: 2011-6-5
 *      Author: simophin
 */

#include "TCPFFMpegServer.h"
#include "Server.h"
#include "providers/FFMpegVideoProvider.h"
#include "CommandMgr.h"
#include "Thread.h"
#include "TCPServerSocket.h"
#include "TCPSocket.h"
#include "TCPSocketAddress.h"
#include "Log.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

enum State {
	STATE_UNINTIALIZED,
	STATE_READY,
	STATE_INSERVICE
};

class TCPFFMpegServer::Impl: public Thread {
public:
	CommandMgr *mCmdMgr;
	TCPSocketAddress mBindAddress;
	TCPServerSocket *mServerSocket;
	VideoProvider *mVideoProvider;
	Server *mServer;
	State mState;

	Impl(const String &addr, int port)
	:mBindAddress(addr,port),
	 mServerSocket(0),
	 mState(STATE_UNINTIALIZED){

	}

	virtual Error entry();
};

TCPFFMpegServer::TCPFFMpegServer(const String & addr, int port)
:d(new Impl(addr,port))
{

}

String TCPFFMpegServer::getBoundInfo() const
{
	return d->mServerSocket->getAddress()->getReadable();
}

Error TCPFFMpegServer::start()
{
	Error ret;
	if (d->mState != STATE_READY) {
		ret.setErrorType(Error::ERR_STATE);
		return ret;
	}

	d->mState = STATE_INSERVICE;
	d->run();
	return ret;
}

Error TCPFFMpegServer::stop()
{
	Error ret;
	if (d->mState != STATE_INSERVICE) {
		ret.setErrorType(Error::ERR_STATE);
		return ret;
	}

	d->stop();
	d->mState = STATE_READY;
	return ret;
}

Error TCPFFMpegServer::init(const char *filename, FFMpegVideoProvider::DeviceType t)
{
	Error ret;
	if (d->mState != STATE_UNINTIALIZED) {
		ret.setErrorType(Error::ERR_STATE);
		return ret;
	}

	int fd;

	// Create fd
	{
		fd = socket(AF_INET, SOCK_STREAM, 0);
		if (fd < 0) goto create_fd_failed;
	}

	// Create server socket and init
	{
		d->mServerSocket = new TCPServerSocket(fd);
		ret = d->mServerSocket->bind(&d->mBindAddress);
		if ( !ret.isSuccess()) {
			goto bind_socket_failed;
		}
		ret = d->mServerSocket->listen(2);
		if ( !ret.isSuccess()) {
			goto listen_failed;
		}
	}

	// Create provider
	{
		d->mVideoProvider = new FFMpegVideoProvider(filename,t);
		ret = d->mVideoProvider->initDevice();
		if (!ret.isSuccess()) {
			goto init_provider_failed;
		}
	}

	// Init command mgr
	{
		d->mCmdMgr = new CommandMgr();
		d->mCmdMgr->registerAllCommands();
	}


	d->mState = STATE_READY;
	return ret;

	init_provider_failed:
	delete d->mVideoProvider;
	d->mVideoProvider = 0;
	listen_failed:
	bind_socket_failed:
	delete d->mServerSocket;
	d->mServerSocket = 0;
	::close(fd);
	create_fd_failed:

	d->mState = STATE_UNINTIALIZED;
	return ret;
}


Error TCPFFMpegServer::wait()
{
	return d->wait();
}

TCPFFMpegServer::~TCPFFMpegServer() {
	delete d;
}

Error TCPFFMpegServer::Impl::entry()
{
	mState = STATE_INSERVICE;

	Error rc;
	TCPSocket *controlSocket = 0, *dataSocket = 0;


	while (!shouldStop()) {
		rc = mServerSocket->poll(IODevice::POLL_READ,2000);
		if ( !rc.isSuccess() ) {
			if (rc.getErrorType() == Error::ERR_TIMEOUT) continue;
			else goto error_polling;
		}

		rc = mServerSocket->accept( (Socket **)&controlSocket,NULL);
		if (!rc.isSuccess()) {
			break;
		}

		rc = mServerSocket->accept( (Socket **)&dataSocket,NULL);
		if (!rc.isSuccess()) {
			break;
		}

		{
			// Now create the server
			mServer = new Server(mCmdMgr,controlSocket,dataSocket,mVideoProvider);
			mServer->start();

			while (!shouldStop() && (mServer->wait(500).getErrorType() == Error::ERR_TIMEOUT)) {}

			delete mServer;
			mServer = 0;
		}

		controlSocket->close();
		dataSocket->close();

		delete controlSocket;
		delete dataSocket;
	}


	error_polling:
	mState = STATE_READY;

	return rc;
}

