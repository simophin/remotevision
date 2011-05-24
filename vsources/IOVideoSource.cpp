/*
 * IOVideoSource.cpp
 *
 *  Created on: 2011-5-13
 *      Author: simophin
 */

#include "IOVideoSource.h"
#include "IODevice.h"
#include "VideoFormat.h"
#include "ImageBuffer.h"
#include "Command.h"
#include "Log.h"

#include "Commander.h"
#include "commands/CommandBuilder.h"
#include "commands/VideoCommand.h"


class IOVideoSource::Impl {
public:
	IODevice *ctrlDev, *dataDev;
	Commander cmdParser;

	VideoInfo info;
	bool needFetchInfo;

	Impl(IODevice *ctrl, IODevice *data)
	:ctrlDev(ctrl),
	 dataDev(data),
	 cmdParser(ctrl),
	 needFetchInfo(true){
	}
};

IOVideoSource::IOVideoSource(IODevice *ctrl, IODevice *data)
:d(new IOVideoSource::Impl(ctrl,data)){
}

IOVideoSource::~IOVideoSource() {
	delete d;
}

IOVideoSource::Info IOVideoSource::
doGetInformation(int ms) const
{
	/*
	if (d->needFetchInfo) {
		CommandBuilder builder;
		builder.setRequestCommand(VideoCommand::QueryInfoCommandHandler::REQUEST_STRING);

		Command cmd;
		builder.build(cmd);
		if (!d->cmdParser.writeCommand(cmd)) {
			const_cast<IOVideoSource *>(this)->setLastError(d->cmdParser.getLastError());
			goto write_error;
		}
		if (!d->cmdParser.readCommand(cmd)) {
			const_cast<IOVideoSource *>(this)->setLastError(d->cmdParser.getLastError());
			goto read_error;
		}

		if (cmd.getName() == VideoCommand::QueryInfoCommandHandler::SUCCESS_STRING ) {
			d->info = VideoCommand::QueryInfoCommandHandler::parseVideoInformationFromCommand(cmd);
			d->needFetchInfo = false;
		}
	}

	return d->info;

	write_error:
	read_error:
	return VideoInfo ();
	*/
}



bool IOVideoSource::
doSetFormat(VideoFormat & fmt, int ms)
{
	Command request,response;
	VideoCommand::SetParameterCommandHandler::buildRequestCommand(request,fmt.getGeometry(),fmt.getVideoCodec());

	// Do request and get response
	{
		if (!d->cmdParser.writeCommand(request)) {
			setLastError(d->cmdParser.getLastError());
			return false;
		}

		if (!d->cmdParser.readCommand(response)) {
			setLastError(d->cmdParser.getLastError());
			return false;
		}
	}

	// Parse response
	{
		if (response.getName() == VideoCommand::SetParameterCommandHandler::SUCCESS_STRING) {
			return true;
		}else{
			if (response.getName() == VideoCommand::SetParameterCommandHandler::ERROR_STRING )
			setLastError(Error(response.getArgument(1).c_str(),response.getArgument(1).size()));
			else
			setLastError(Error("Unknown error"));
			return false;
		}
	}
}



ImageBuffer IOVideoSource::
doGetFilledBuffer(int ms)
{
}



bool IOVideoSource::
doStopCapture(int ms)
{
}



void IOVideoSource::
doPutBuffer(const ImageBuffer & buf, int ms)
{
}



VideoFormat IOVideoSource::
doGetFormat(int ms) const
{
}



bool IOVideoSource::
doStartCapture(int ms)
{
}



bool IOVideoSource::
doInit(const IOVideoSource::Option & options, int ms)
{
}


