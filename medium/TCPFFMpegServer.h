/*
 * TCPFFMpegServer.h
 *
 *  Created on: 2011-6-5
 *      Author: simophin
 */

#ifndef TCPFFMPEGSERVER_H_
#define TCPFFMPEGSERVER_H_

#include <string>
#include "Error.h"

class TCPFFMpegServer{
public:
	TCPFFMpegServer(const std::string &addr, int port = 0);

	std::string getBoundInfo () const;
	bool init();
	bool start ();
	void stop();

	Error getLastError() const;

	virtual ~TCPFFMpegServer();

private:
	class Impl;
	Impl *d;
};





#endif /* TCPFFMPEGSERVER_H_ */
