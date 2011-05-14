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


class IOVideoSource::Impl {
public:
	IODevice *ctrlDev, *dataDev;
	Commander cmdParser;

	VideoSource::Information info;
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

IOVideoSource::Information IOVideoSource::
doGetInformation(int ms) const
{
	if (d->needFetchInfo) {
		Information info;
		CommandBuilder builder;
		builder.appendArgument("QUERY_INFO");

		Command cmd;
		builder.build(cmd);
		if (!d->cmdParser.writeCommand(cmd)) {
			Log::log(Log::LOG_ERROR, d->cmdParser.getLastError());
			goto write_error;
		}
		if (!d->cmdParser.readCommand(cmd)) {
			Log::log(Log::LOG_ERROR, d->cmdParser.getLastError());
			goto read_error;
		}

		if (cmd.getName() == "OK" ) {

		}

		write_error:
		read_error:
		return Information ();
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


