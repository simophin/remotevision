/*
 * TCPFFMpegRelayServer.cpp
 *
 *  Created on: 2011-6-9
 *      Author: simophin
 */

#include "TCPFFMpegRelayServer.h"
#include "Thread.h"
#include "IODevice.h"
#include "TCPServerSocket.h"
#include "TCPSocket.h"
#include "TCPSocketAddress.h"
#include "Log.h"
#include "platform/posix/PosixCompactHeader.h"
#include "platform/posix/PosixSocketDemuxer.h"

#include <utility>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>

#define MAX_EXCHANGED_DATA 10240

class ExchangeThread : public Thread{
public:
	ExchangeThread (IODevice *d1, IODevice *d2)
	:mD1(d1), mD2(d2) {

	}
	virtual Error entry();

	inline IODevice * theOther(IODevice *p) {
		return (p == mD1) ? mD2 : mD1;
	}

private:
	IODevice  *mD1, *mD2;
};

class TCPFFMpegRelayServer::Impl: public Thread {
public:
	String   mAddr;
	unsigned mProviderPort;
	unsigned mPort;

	TCPServerSocket * mProviderServerSocket, * mServerSocket;
	TCPFFMpegRelayServer::State mState;

	Impl ()
	:mServerSocket(0),
	 mState(STATE_UNINITIALIZED){

	}

	~Impl () {
		if (mProviderServerSocket) {
			mProviderServerSocket->close();
			delete mProviderServerSocket;
		}

		if (mServerSocket) {
			mServerSocket->close();
			delete mServerSocket;
		}
	}

	virtual Error entry ();
};

TCPFFMpegRelayServer::TCPFFMpegRelayServer(const String &addr,unsigned provider_port, unsigned port)
:d(new Impl){
	d->mAddr = addr;
	d->mProviderPort = provider_port;
	d->mPort = port;
}

TCPFFMpegRelayServer::~TCPFFMpegRelayServer() {
	delete d;
}

Error ExchangeThread::entry()
{
	Error ec;
	PosixSocketDemuxer demuxer;
	char *buf = (char *)::malloc(MAX_EXCHANGED_DATA);
	::memset(buf,0,MAX_EXCHANGED_DATA);
	size_t readBytes = 0, writeBytes = 0;
	demuxer.addSocket( (PosixSocket *)mD1, PosixSocketDemuxer::SELECT_READ |
			PosixSocketDemuxer::SELECT_ERROR);
	demuxer.addSocket( (PosixSocket *)mD2,PosixSocketDemuxer::SELECT_READ |
			PosixSocketDemuxer::SELECT_ERROR);

	std::vector<PosixSocket *> reads,writes, errors;

	while (!shouldStop()) {
		reads.clear();
		errors.clear();

		ec = demuxer.waitEvent(reads, writes, errors, 200);
		if (errors.size() > 0) {
			Log::logDebug("One or more sockets are in error");
			ec.setErrorString("One or more sockets are in error");
			break;
		}
		if (ec.isSuccess()) {
			for (int i=0; i<reads.size(); i++) {
				ec = reads[i]->read(buf, MAX_EXCHANGED_DATA, &readBytes);
				if (ec.isError()) goto error;

				if (readBytes == 0) goto error;

				IODevice *other = theOther(reads[i]);
				ec = other->poll(IODevice::POLL_WRITE,500);
				if (ec.isError()) goto error;
				ec = other->write(buf, readBytes,&writeBytes);
				if (ec.isError() || (writeBytes < 1) ) goto error;
			}
		}
	}

	error:
	::free(buf);
	return ec;
}

Error TCPFFMpegRelayServer::Impl::entry() {
	Error ec;
	Socket * providerControl = 0, * providerData = 0;
	Socket * clientControl = 0, * clientData = 0;

	while (!shouldStop()) {

		// Get all clients connected
		while (!shouldStop() && (!providerData || !clientData) ) {
			if (providerData == 0){
				ec = mProviderServerSocket->poll(IODevice::POLL_READ,500);
				if (ec.isSuccess()) {
					if ( providerControl == 0 ) {
						ec = mProviderServerSocket->accept(&providerControl, NULL);
						if (ec.isError()) goto fatal;
					}else{
						ec = mProviderServerSocket->accept(&providerData, NULL);
						if (ec.isError()) goto fatal;
					}
				}else{
					if (ec.getErrorType() != Error::ERR_TIMEOUT) goto fatal;
				}
			}

			if (clientData == 0) {
				ec = mServerSocket->poll(IODevice::POLL_READ, 500);
				if (ec.isSuccess()) {
					if ( clientControl == 0 ) {
						ec = mServerSocket->accept(&clientControl, NULL);
						if (ec.isError()) goto fatal;
					}else{
						ec = mServerSocket->accept(&clientData, NULL);
						if (ec.isError()) goto fatal;
					}
				}
			}
		}

		if (!providerData || !clientData) break;

		// Do exchanged thread
		ExchangeThread thread1 (providerControl, clientControl);
		ExchangeThread thread2 (clientData, providerData);
		thread1.run();
		thread2.run();

		while (!shouldStop()) {

			if (!thread1.isRunning() || !thread2.isRunning()) {
				providerControl->close();
				providerData->close();
				clientControl->close();
				clientData->close();

				thread1.wait();
				thread2.wait();
				break;
			}


			thread1.wait(NULL,500);
			thread2.wait(NULL,500);
		}


		delete providerControl;
		delete providerData;
		delete clientControl;
		delete clientData;
		mProviderServerSocket->close();
		mServerSocket->close();
		delete mProviderServerSocket;
		delete mServerSocket;
		mProviderServerSocket = 0;
		mServerSocket = 0;
		mState = STATE_UNINITIALIZED;
		providerControl = providerData = clientControl = clientData = 0;
		break;
	}



	fatal:
	if (providerControl) delete providerControl;
	if (providerData) delete providerData;
	if (clientControl) delete clientControl;
	if (clientData) delete clientData;
	return ec;
}

Error TCPFFMpegRelayServer::init()
{
	Error ret;
	int on = 1;

	if (d->mState != STATE_UNINITIALIZED) {
		ret.setErrorType(Error::ERR_STATE);
		return ret;
	}

	TCPSocketAddress providerBindAddr (d->mAddr, d->mProviderPort);
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if ( fd < 0 ) {
		ret.setErrorString("Can't create socket");
		goto create_provider_fd_failed;
	}

	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on)) < 0) {
		ret.setSystemError(errno);
		goto create_provider_fd_failed;
	}


	// Create provider server socket instance
	d->mProviderServerSocket = new TCPServerSocket(fd);


	ret = d->mProviderServerSocket->bind(&providerBindAddr);
	if (ret.isError()) {
		goto bind_provider_server_failed;
	}
	ret = d->mProviderServerSocket->listen(2);
	if (ret.isError()) {
		goto listen_provider_server_failed;
	}

	// Create relay server socket instance
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if ( fd < 0 ) {
		ret.setErrorString("Can't create socket");
		goto create_server_fd_failed;
	}
	on = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on)) < 0) {
		ret.setSystemError(errno);
		goto create_server_fd_failed;
	}
	d->mServerSocket = new TCPServerSocket (fd);
	providerBindAddr.setPort(d->mPort);
	ret = d->mServerSocket->bind(&providerBindAddr);
	if (ret.isError()) {
		goto bind_server_failed;
	}
	ret = d->mServerSocket->listen(2);
	if (ret.isError()) {
		goto listen_provider_failed;
	}

	d->mState = STATE_READY;
	return ret;

	listen_provider_failed:
	bind_server_failed:
	d->mServerSocket->close();
	delete d->mServerSocket;
	d->mServerSocket = 0;
	create_server_fd_failed:
	listen_provider_server_failed:
	bind_provider_server_failed:
	d->mProviderServerSocket->close();
	delete d->mProviderServerSocket;
	d->mProviderServerSocket =0;
	create_provider_fd_failed:

	return ret;
}



Error TCPFFMpegRelayServer::start()
{
	Error ec;
	if (d->mState != STATE_READY) {
		ec.setErrorType(Error::ERR_STATE);
		return ec;
	}

	ec = d->run();
	if (ec.isSuccess()) {
		d->mState = STATE_INSERVICE;
	}
	return ec;
}



Error TCPFFMpegRelayServer::stop()
{
	Error ec;
	ec = d->stop();
	d->mState = STATE_READY;
	return ec;
}



Error TCPFFMpegRelayServer::wait()
{
	return d->wait();
}

TCPFFMpegRelayServer::State TCPFFMpegRelayServer::getState() const
{
	return d->mState;
}



