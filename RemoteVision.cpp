/*
 * RemoteVision.cpp
 *
 *  Created on: 2011-5-5
 *      Author: launliu
 */

#include "RemoteVision.h"
#include "CommandMgr.h"
#include "providers/FFMpegVideoProvider.h"
#include "commands/VideoCommand.h"


extern void initCommands();

class RemoteVision::Impl {
public:
	CommandMgr cmdMgr;
	FFMpegVideoProvider videoProvider;

	Impl(const std::string &name)
	:videoProvider(name) {

	}
};

RemoteVision::RemoteVision(const std::string &name)
:d(new RemoteVision::Impl(name)){
	d->cmdMgr.registerCommandHandler(new VideoCommand::QueryInfoCommandHandler);
	d->cmdMgr.registerCommandHandler(new VideoCommand::SetParameterCommandHandler);
}

RemoteVision::~RemoteVision() {
	delete d;
}
