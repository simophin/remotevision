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
#include "commands/QueryInfoCommandHandler.h"


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

VideoInfo IOVideoSource::
doGetInformation(int ms) const
{
	if (d->needFetchInfo) {
		CommandBuilder builder;
		builder.appendArgument("QUERY_INFO");

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

		if (cmd.getName() == "QUERY_INFO_OK" ) {
			d->info = QueryInfoCommandHandler::parseVideoInformationFromCommand(cmd);
			d->needFetchInfo = false;
		}

		write_error:
		read_error:
		return VideoInfo ();
	}

	return d->info;
}



bool IOVideoSource::
doSetFormat(VideoFormat & fmt, int ms)
{
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


