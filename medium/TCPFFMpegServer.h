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

class TCPFFMpegServer{
public:
	TCPFFMpegServer(const String &addr, int port = 0);

	String getBoundInfo () const;
	bool init(const char *deviceFile = "/dev/video0");
	bool start ();
	void stop();
	bool wait();

	Error getLastError() const;

	virtual ~TCPFFMpegServer();

private:
	class Impl;
	Impl *d;
};





#endif /* TCPFFMPEGSERVER_H_ */
