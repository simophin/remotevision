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
#include "CommandMgr.h"
#include "CommandContext.h"
#include "Command.h"

#include <assert.h>
#include <memory>

class Server::ServerImpl: public Thread {
public:
	IODevice *controlDevice, *dataDevice;
	CommandReader cmdReader;
	CommandContext cmdCtx;

	void entry();
};

Server::
Server(IODevice *ctrl_device, IODevice *data_device)
:d(new Server::ServerImpl)
{
	setDataDevice(data_device);
	setControlDevice(ctrl_device);
	d->cmdCtx.server = this;
	d->cmdCtx.client = 0;
}

Server::~Server() {
}

void Server::
setDataDevice(IODevice *device)
{
	d->dataDevice = device;
	d->cmdCtx.dataDevice = device;
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
	d->cmdCtx.controlDevice = device;
}



int Server::
start()
{
	assert(d->controlDevice != 0 && d->dataDevice != 0);
	d->run();
	return 0;
}


void Server::ServerImpl::entry() {
	int rc;
	while(!shouldStop()) {
		rc = controlDevice->poll(IODevice::POLL_READ, 500);
		if (rc < 0) break;
		else if (rc == 0) continue;
		Command *raw_cmd = cmdReader.readCommand();
		CommandMgr::handleCommand(raw_cmd,&cmdCtx);
		delete raw_cmd;
	}
}
