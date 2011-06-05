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

enum State {
	STATE_UNINTIALIZED,
	STATE_READY,
	STATE_INSERVICE,
};

class TCPFFMpegServer::Impl: public Thread {
public:
	CommandMgr *mCmdMgr;
	TCPServerSocket *mServerSocket;
	TCPSocketAddress mBindAddress;
	VideoProvider *mVideoProvider;
	Error mLastError;
	Server *mServer;
	State mState;

	Impl(const std::string &addr, int port)
	:mBindAddress(addr,port),
	 mServerSocket(0),
	 mState(STATE_UNINTIALIZED){

	}

	virtual void entry();
};

TCPFFMpegServer::TCPFFMpegServer(const std::string & addr, int port)
:d(new Impl(addr,port))
{

}

std::string TCPFFMpegServer::getBoundInfo() const
{
	return d->mServerSocket->getAddress()->getReadable();
}

bool TCPFFMpegServer::start()
{
	if (d->mState != STATE_READY) {
		d->mLastError = Error("State error");
		return false;
	}

	d->mState = STATE_INSERVICE;
	d->run();
	return true;
}

void TCPFFMpegServer::stop()
{
	if (d->mState != STATE_INSERVICE) {
		d->mLastError = Error("State error");
		return;
	}

	d->stop();
	d->mState = STATE_READY;
}

bool TCPFFMpegServer::init()
{
	if (d->mState != STATE_UNINTIALIZED) {
		d->mLastError = Error("State error");
		return false;
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
		if (d->mServerSocket->bind(&d->mBindAddress) != 0) {
			d->mLastError = d->mServerSocket->getLastError();
			goto bind_socket_failed;
		}
		if (d->mServerSocket->listen(2) != 0) {
			d->mLastError = d->mServerSocket->getLastError();
			goto listen_failed;
		}
	}

	// Create provider
	{
		d->mVideoProvider = new FFMpegVideoProvider("/dev/video0");
		if (!d->mVideoProvider->initDevice()) {
			d->mLastError = d->mVideoProvider->getLastError();
			goto init_provider_failed;
		}
	}

	// Init command mgr
	{
		d->mCmdMgr = new CommandMgr();
		d->mCmdMgr->registerAllCommands();
	}


	d->mState = STATE_READY;
	return true;

	init_provider_failed:
	delete d->mVideoProvider;
	d->mVideoProvider = 0;
	listen_failed:
	bind_socket_failed:
	delete d->mServerSocket;
	d->mServerSocket = 0;
	close(fd);
	create_fd_failed:

	d->mState = STATE_UNINTIALIZED;
	return false;
}

Error TCPFFMpegServer::getLastError() const
{
	return d->mLastError;
}

TCPFFMpegServer::~TCPFFMpegServer() {
	// TODO Auto-generated destructor stub
}

void TCPFFMpegServer::Impl::entry()
{
	mState = STATE_INSERVICE;

	int rc;
	TCPSocketAddress *addr = NULL;
	TCPSocket *controlSocket, *dataSocket;
	while (!shouldStop()) {
		if ((rc = mServerSocket->poll(IODevice::POLL_READ,2000)) <= 0) {
			if (rc == 0) continue;
			else goto timeout;
		}

		controlSocket = (TCPSocket *)mServerSocket->accept((SocketAddress **)&addr);
		if (controlSocket == 0) {
			goto accept_failed;
		}
		Log::logDebug("Control interface accepted from %s", addr->getReadable().c_str());
		dataSocket = (TCPSocket *)mServerSocket->accept((SocketAddress **)&addr);
		if (controlSocket == 0) {
			goto accept_failed;
		}
		Log::logDebug("Data interface accepted from %s", addr->getReadable().c_str());

		{
			// Now create the server
			mServer = new Server(mCmdMgr,controlSocket,dataSocket,mVideoProvider);
			mServer->start();

			while (!shouldStop()) {
				mServer->wait(1000);
			}

			delete mServer;
			mServer = 0;
		}
	}

	delete controlSocket;
	delete dataSocket;
	accept_failed:
	timeout:
	mState = STATE_READY;
}

