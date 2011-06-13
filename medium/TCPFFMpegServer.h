/*
 * TCPFFMpegServer.h
 *
 *  Created on: 2011-6-5
 *      Author: simophin
 */

#ifndef TCPFFMPEGSERVER_H_
#define TCPFFMPEGSERVER_H_

#include "RString.h"
#include "Error.h"
#include "providers/FFMpegVideoProvider.h"

class TCPFFMpegServer{
public:
	enum  ConnType {
		SERVER = 0,
		RELAY
	};


	TCPFFMpegServer(ConnType connType, const String &addr, int port = 0);

	ConnType getConnType () const;
	String getBoundInfo () const;
	Error init(const char *deviceFile = "/dev/video0");
	Error start ();
	Error stop();
	Error wait();

	virtual ~TCPFFMpegServer();

private:
	class Impl;
	Impl *d;
};





#endif /* TCPFFMPEGSERVER_H_ */
