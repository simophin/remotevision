/*
 * Server.cpp
 *
 *  Created on: 2011-3-31
 *      Author: simophin
 */

#include "Server.h"
#include "IODevice.h"
#include "Thread.h"
#include "Commander.h"
#include "CommandMgr.h"
#include "CommandContext.h"
#include "Command.h"
#include "VideoProvider.h"
#include "Log.h"
#include "Error.h"

#include <assert.h>
#include <memory>
#include <stdlib.h>


class Server::ServerImpl: public Thread {
public:
	IODevice *controlDevice, *dataDevice;
	Commander cmdExe;
	CommandContext cmdCtx;
	CommandMgr *cmdMgr;

	virtual Error entry();
};

Server::
Server(CommandMgr *cmdMgr,IODevice *ctrl_device, IODevice *data_device, VideoProvider *p)
:d(new Server::ServerImpl)
{
	setDataDevice(data_device);
	setControlDevice(ctrl_device);
	d->cmdCtx.server = this;
	d->cmdCtx.client = 0;
	d->cmdCtx.videoProvider = p;
	d->cmdMgr = cmdMgr;
}

Server::~Server() {
}

VideoProvider *Server::getProvider() const
{
	return d->cmdCtx.videoProvider;
}

Error Server::wait(int ms)
{
	return d->wait(NULL,ms);

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



Error Server::
stop(int ms)
{
	return d->stop(ms);
}



void Server::
setControlDevice(IODevice *device)
{
	d->controlDevice = device;
	d->cmdExe.setDevice(device);
	d->cmdCtx.controlDevice = device;
}



Error Server::
start()
{
	assert(d->controlDevice != 0 && d->dataDevice != 0);
	return d->run();
}


Error Server::ServerImpl::entry() {
	Error rc;
	Command raw_cmd;
	while(!shouldStop()) {
		rc = controlDevice->poll(IODevice::POLL_READ, 500);
		if (rc.getErrorType() ==  Error::ERR_TIMEOUT) continue;
		else if (!rc.isSuccess()) break;

		rc = cmdExe.readCommand(raw_cmd);
		if (!rc.isSuccess()) {
			break;
		}

		cmdMgr->handleCommand(raw_cmd,&cmdCtx);
	}

	return rc;
}

