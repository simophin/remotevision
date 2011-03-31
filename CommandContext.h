/*
 * CommandContext.h
 *
 *  Created on: 2011-3-31
 *      Author: simophin
 */

#ifndef COMMANDCONTEXT_H_
#define COMMANDCONTEXT_H_

class IODevice;
class Server;
class Client;
class CommandContext {
public:
	IODevice *controlDevice, *dataDevice;
	Server *server;
	Client *client;

	CommandContext(IODevice *ctrl = 0,
			IODevice *data = 0,
			Server *server = 0,
			Client *c = 0)
	:controlDevice(ctrl),
	 dataDevice(data),
	 server(server),
	 client(c) {

	}
};

#endif /* COMMANDCONTEXT_H_ */
