/*
 * Server.h
 *
 *  Created on: 2011-3-31
 *      Author: simophin
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <boost/noncopyable.hpp>
#include <memory>

class IODevice;
class VideoProvider;
class Server: public boost::noncopyable {
public:
	Server(IODevice *ctrl_device = 0, IODevice *data_device = 0, VideoProvider *p = 0);
	virtual ~Server();

	int start();
	void stop(int ms = -1);

	const IODevice * getControlDevice() const;
	const IODevice * getDataDevice() const;
	void setControlDevice(IODevice *);
	void setDataDevice(IODevice *);

private:
	class ServerImpl;
	std::auto_ptr<ServerImpl> d;
};

#endif /* SERVER_H_ */
