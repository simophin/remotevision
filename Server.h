/*
 * Server.h
 *
 *  Created on: 2011-3-31
 *      Author: simophin
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <memory>

#include "utils/NonCopyable.hpp"

class IODevice;
class VideoProvider;
class CommandMgr;
class Error;
class Server: public Utils::NonCopyable {
public:
	Server(CommandMgr *,IODevice *ctrl_device = 0, IODevice *data_device = 0, VideoProvider *p = 0);
	virtual ~Server();

	Error start();
	Error stop(int ms = -1);
	Error wait(int ms = -1);

	const IODevice * getControlDevice() const;
	const IODevice * getDataDevice() const;
	VideoProvider  * getProvider() const;
	void setControlDevice(IODevice *);
	void setDataDevice(IODevice *);

	class ServerImpl;
private:
	std::auto_ptr<ServerImpl> d;
};





#endif /* SERVER_H_ */
