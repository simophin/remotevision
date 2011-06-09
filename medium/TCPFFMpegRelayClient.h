/*
 * TCPFFMpegRelayClient.h
 *
 *  Created on: 2011-6-9
 *      Author: simophin
 */

#ifndef TCPFFMPEGRELAYCLIENT_H_
#define TCPFFMPEGRELAYCLIENT_H_

#include "TCPFFMpegRelayServer.h"

#include "RString.h"

class TCPFFMpegRelayClient {
public:
	TCPFFMpegRelayClient(const String & remote = "localhost",
			unsigned port = RELAY_PROVIDER_PORT);
	virtual ~TCPFFMpegRelayClient();

private:
	class Impl;
	Impl *d;
};

#endif /* TCPFFMPEGRELAYCLIENT_H_ */
