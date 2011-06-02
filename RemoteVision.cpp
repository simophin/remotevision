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
#include "3rdparty/ffmpeg/FFMpeg.h"

#include <assert.h>

static RemoteVisionApp * INSTANCE = 0;

class RemoteVisionApp::Impl {
public:
	CommandMgr cmdMgr;
	FFMpegVideoProvider *videoProvider;

	Impl(const std::string &name)
	:videoProvider(0) {
		FFMpeg::init();
		videoProvider = new FFMpegVideoProvider(name);
		if (!videoProvider->initDevice()) {
			assert (false);
		}
	}

	~Impl() {
		if (videoProvider) delete videoProvider;
	}
};

RemoteVisionApp::RemoteVisionApp(const std::string &name)
:d(new RemoteVisionApp::Impl(name)){
	assert (INSTANCE == 0);
	INSTANCE = this;
	d->cmdMgr.registerCommandHandler(new VideoCommand::QueryInfoCommandHandler);
	d->cmdMgr.registerCommandHandler(new VideoCommand::SetParameterCommandHandler);
	d->cmdMgr.registerCommandHandler(new VideoCommand::GetParameterCommandHandler);
}

RemoteVisionApp::~RemoteVisionApp() {
	delete d;
}

VideoProvider *RemoteVisionApp::getProvider() const
{
	return d->videoProvider;
}



CommandMgr *RemoteVisionApp::getCommandMgr() const
{
	return &d->cmdMgr;
}



RemoteVisionApp *RemoteVisionApp::getInstance() {
	return INSTANCE;
}


