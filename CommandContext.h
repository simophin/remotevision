/*
 * CommandContext.h
 *
 *  Created on: 2011-3-31
 *      Author: simophin
 */

#ifndef COMMANDCONTEXT_H_
#define COMMANDCONTEXT_H_

#include <vector>

class IODevice;
class Server;
class Client;
class VideoProvider;
class Thread;
class CommandContext {
public:
	IODevice *controlDevice;
	IODevice *dataDevice;
	Server *server;
	Client *client;
	VideoProvider *videoProvider;
	Thread *dataThread;

	CommandContext(IODevice *ctrl = 0,
			IODevice *data = 0,
			Server *server = 0,
			Client *c = 0)
	:controlDevice(ctrl),
	 dataDevice(data),
	 server(server),
	 client(c),
	 dataThread(0){

	}
};

#endif /* COMMANDCONTEXT_H_ */
