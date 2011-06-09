/*
 * TCPFFMpegRelayClient.h
 *
 *  Created on: 2011-6-9
 *      Author: simophin
 */

#ifndef TCPFFMPEGRELAYCLIENT_H_
#define TCPFFMPEGRELAYCLIENT_H_

#include "RString.h"

class TCPFFMpegRelayClient {
public:
	TCPFFMpegRelayClient(const String & remote, unsigned port);
	virtual ~TCPFFMpegRelayClient();
};

#endif /* TCPFFMPEGRELAYCLIENT_H_ */
