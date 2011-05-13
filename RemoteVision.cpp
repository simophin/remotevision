/*
 * RemoteVision.cpp
 *
 *  Created on: 2011-5-5
 *      Author: launliu
 */

#include "RemoteVision.h"
#include "CommandMgr.h"
#include "providers/FFMpegVideoProvider.h"

extern void initCommands();

class RemoteVision::Impl {
public:
	CommandMgr cmdMgr;
	FFMpegVideoProvider videoProvider;
};

RemoteVision::RemoteVision()
:d(new RemoteVision::Impl){

}

RemoteVision::~RemoteVision() {
	delete d;
}