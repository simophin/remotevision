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
#include "utils/SharedPtr.hpp"
#include "utils/Misc.hpp"

#include <utility>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>

#define MAX_EXCHANGED_DATA 10240

enum ClientType {
	PROVIDER_CONTROL = 0,
	PROVIDER_DATA,
	CLIENT_CONTROL,
	CLIENT_DATA,
	NUM_CLIENT_TYPE
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

	virtual ~Impl () {
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


void destroySocket (Socket * sock) {
	sock->close();
	delete sock;
}

bool isClientsAllConnected ( Socket * socks[]) {
	bool ret = true;
	for (int i=0 ;i<NUM_CLIENT_TYPE; i++) {
		if (socks[i] == 0) {
			ret = false;
			break;
		}
	}
	return ret;
}

PosixSocket * anotherSocket (Socket *socks[], PosixSocket *s) {
	for (int i=0; i<NUM_CLIENT_TYPE; i++) {
		if (s == socks[i]) {
			return static_cast<PosixSocket *>(socks[(i+2)%NUM_CLIENT_TYPE]);
		}
	}
	return 0;
}

Error TCPFFMpegRelayServer::Impl::entry() {
	Error ec;
	Socket * clients[NUM_CLIENT_TYPE] = { 0, 0, 0, 0 };
	SocketAddress *addr = 0;
	PosixSocketDemuxer serverDemuxer, clientDemuxer;
	size_t readBytes = 0, writtenSize = 0;
	std::vector<PosixSocket *> reads, errors;
	char * buf = (char *)::malloc(MAX_EXCHANGED_DATA);

	serverDemuxer.addSocket( mProviderServerSocket, IODevice::POLL_READ );
	serverDemuxer.addSocket( mServerSocket, IODevice::POLL_READ);

	std::vector<PosixSocket *> listens, dummy;

	while (!shouldStop()) {
		Log::logDebug("Starting listening for clients");
		while (!shouldStop()) {
			listens.clear();
			ec = serverDemuxer.waitEvent(listens,dummy,dummy,200);
			if (ec.isSuccess()) {
				for (int i=0; i<listens.size(); i++) {
					Socket *sock = 0;
					ec = listens.at(i)->accept(&sock, &addr);
					if (ec.isSuccess()) {
						Log::logDebug("New connection from %s", addr->getReadable().c_str());
						delete addr;
						addr = 0;

						if (listens.at(i) == mProviderServerSocket) {
							if ( clients[PROVIDER_CONTROL] && clients[PROVIDER_DATA]) {
								Log::logDebug("Overriding last connected provider sockets");
								destroySocket(clients[PROVIDER_CONTROL]);
								destroySocket(clients[PROVIDER_DATA]);
								clients[PROVIDER_DATA] = clients[PROVIDER_CONTROL] = 0;
							}
							if ( clients[PROVIDER_CONTROL] == 0 ) clients[PROVIDER_CONTROL] = sock;
							else clients[PROVIDER_DATA] = sock;
						}else{
							if ( clients[CLIENT_CONTROL] && clients[CLIENT_DATA]) {
								Log::logDebug("Overriding last connected client sockets");
								destroySocket(clients[CLIENT_CONTROL]);
								destroySocket(clients[CLIENT_DATA]);
								clients[CLIENT_DATA] = clients[CLIENT_CONTROL] = 0;
							}
							if ( clients[CLIENT_CONTROL] == 0 ) clients[CLIENT_CONTROL] = sock;
							else clients[CLIENT_DATA] = sock;
						}

					}else{
						goto clean_up;
					}
				}

				if (!isClientsAllConnected(clients)) {
					continue;
				}else{
					break;
				}
			}else{
				continue;
			}
		}

		// end accepting

		Log::logDebug("Begin transfering data between 4 sockets");

		// Make demuxer
		{
			clientDemuxer.clearSockets();
			for (int i=0; i<NUM_CLIENT_TYPE; i++) {
				clientDemuxer.addSocket( (PosixSocket *)clients[i], IODevice::POLL_READ | IODevice::POLL_ERROR);
			}
		}

		// Wait for the events
		while (!shouldStop()){
			reads.clear();
			errors.clear();
			ec = clientDemuxer.waitEvent(reads,dummy,errors);
			if (ec.isError()) {
				goto clean_up;
			}else if (errors.size() > 0) {
				goto clean_up_cont;
			}

			for (int i=0;i<reads.size();i++) {
				ec = reads[i]->read(buf,MAX_EXCHANGED_DATA, &readBytes);
				if (ec.isError() || readBytes == 0) {
					goto clean_up_cont;
				}

				ec = anotherSocket(clients, reads[i])->write(buf, readBytes,&writtenSize);
				if (ec.isError() || (writtenSize  != readBytes)) {
					goto clean_up_cont;
				}
			}
		}

		continue;

		clean_up_cont:
		for (int i=0; i<NUM_CLIENT_TYPE; i++) {
			destroySocket(clients[i]);
			clients[i] = 0;
		}

		continue;
	}

	return ec;
	clean_up:
	Log::logDebug("Relay thread is shutting down");
	::free(buf);
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



