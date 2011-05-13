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
		d->cmdParser.writeCommand(cmd);
		d->cmdParser.readCommand(cmd);

		if (cmd.mName == "OK" ) {

		}
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


