/*
 * rv_service.h
 *
 *  Created on: 2011-6-4
 *      Author: simophin
 */

#ifndef RV_SERVICE_H_
#define RV_SERVICE_H_


#include "RemoteVision.h"
#include "Server.h"
#include "Thread.h"
#include "ServerSocket.h"
#include "Socket.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

class RvService: public Thread {
public:
	RemoteVisionApp app;
	ServerSocket * serverSocket;
	Socket *controlSocket, *dataSocket;
	Server *server;

	virtual void entry();
};


#endif /* RV_SERVICE_H_ */
