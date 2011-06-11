/*
 * TCPFFMpegRelayServer.h
 *
 *  Created on: 2011-6-9
 *      Author: simophin
 */

#define RELAY_PROVIDER_PORT 15000
#define RELAY_PORT 15001

#ifndef TCPFFMPEGRELAYSERVER_H_
#define TCPFFMPEGRELAYSERVER_H_

#include "utils/NonCopyable.hpp"
#include "Error.h"
#include "RString.h"

class TCPFFMpegRelayServer: public Utils::NonCopyable {
public:
	TCPFFMpegRelayServer(const String & addr,
							unsigned  provider_port = RELAY_PROVIDER_PORT,
							unsigned  port = RELAY_PORT);
	virtual ~TCPFFMpegRelayServer();

	enum State {
		STATE_UNINITIALIZED,
		STATE_READY,
		STATE_INSERVICE
	};

	Error init () ;
	Error start ();
	Error stop ();
	Error wait();
	State getState() const;

private:
	class Impl;
	Impl *d;
};




#endif /* TCPFFMPEGRELAYSERVER_H_ */
