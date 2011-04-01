/*
 * Server.cpp
 *
 *  Created on: 2011-3-31
 *      Author: simophin
 */

#include "Server.h"
#include "IODevice.h"
#include "Thread.h"
#include "CommandReader.h"

#include <assert.h>
#include <memory>

class Server::ServerImpl: public Thread {
public:
	IODevice *controlDevice, *dataDevice;
	CommandReader cmdReader;

	void entry();
};

Server::
Server(IODevice *ctrl_device, IODevice *data_device)
:d(new Server::ServerImpl)
{
	setDataDevice(data_device);
	setControlDevice(ctrl_device);
}

Server::~Server() {
}

void Server::
setDataDevice(IODevice *device)
{
	d->dataDevice = device;
}



const IODevice *Server::
getDataDevice() const
{
	return d->dataDevice;
}



const IODevice *Server::
getControlDevice() const
{
	return d->controlDevice;
}



void Server::
stop(int ms)
{
	d->stop(ms);
}



void Server::
setControlDevice(IODevice *device)
{
	d->controlDevice = device;
	d->cmdReader.setDevice(device);
}



int Server::
start()
{
	assert(d->controlDevice != 0 && d->dataDevice != 0);
	d->run();
	return 0;
}


void Server::ServerImpl::entry() {

}
