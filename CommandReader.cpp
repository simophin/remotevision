/*
 * CommandReader.cpp
 *
 *  Created on: 2011-3-31
 *      Author: simophin
 */

#include "CommandReader.h"
#include "Command.h"
#include "IODevice.h"
#include "Protocol.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <list>

#include <boost/algorithm/string.hpp>

class CommandReader::Impl {
public:
	IODevice *device;
};

CommandReader::CommandReader(IODevice *device)
:d(new CommandReader::Impl){
	setDevice(device);
}

CommandReader::~CommandReader() {

}

void CommandReader::setDevice(IODevice *device) {
	d->device = device;
}

IODevice *CommandReader::
getDevice() const {
	return d->device;
}

Command *CommandReader::
readCommand() {
	IODevice *dd = d->device;
	Command *ret = NULL;
	assert(dd != 0);

	CommandHeader hdr;
	char *buf = 0;
	// Read header
	{
		if (dd->read((unsigned char *)&hdr,sizeof(hdr)) != sizeof(hdr)) {
			return NULL;
		}
	}

	// Read command
	{
		buf = (char *)::malloc(hdr.length);
		int tried_times = 0;
		size_t offset = 0;
		do {
			int rc = dd->read( (unsigned char *)(buf + offset), hdr.length-offset );
			if (rc < 0) {
				ret = NULL;
				goto out;
			}
			offset += rc;
		}while(tried_times++ < 20 && offset < hdr.length);
	}
	{
		std::string name;
		std::list<std::string> args;
		size_t offset = 0;

		name = buf;
		offset += ::strlen(buf)+1;

		while(offset < hdr.length) {
			args.push_back(buf+offset);
			offset += ::strlen(buf+offset);
		}

		ret = new Command(name,args);
	}


	out:
	if (buf) ::free(buf);
	return ret;
}
