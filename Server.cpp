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

#include <assert.h>
#include <memory>
#include <stdlib.h>


class Server::ServerImpl: public Thread {
public:
	IODevice *controlDevice, *dataDevice;
	Commander cmdExe;
	CommandContext cmdCtx;
	CommandMgr *cmdMgr;

	void entry();
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

bool Server::wait(int ms)
{
	if (d->isRunning())
		return d->wait(ms);
	else return true;

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
	d->cmdExe.setDevice(device);
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
	Command raw_cmd;
	while(!shouldStop()) {
		rc = controlDevice->poll(IODevice::POLL_READ, 500);
		if (rc < 0) break;
		else if (rc == 0) continue;
		if (!cmdExe.readCommand(raw_cmd)) {
			break;
		}

		cmdMgr->handleCommand(raw_cmd,&cmdCtx);
	}
	rc = 0;
}

