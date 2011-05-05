/*
 * CommandReader.cpp
 *
 *  Created on: 2011-3-31
 *      Author: simophin
 */

#include "Commander.h"
#include "Command.h"
#include "IODevice.h"
#include "Protocol.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <sstream>
#include <string>
#include <list>

#include <boost/algorithm/string.hpp>


class Commander::Impl {
public:
	IODevice *device;
};

Commander::Commander(IODevice *device)
:d(new Commander::Impl){
	setDevice(device);
}

Commander::~Commander() {

}

void Commander::setDevice(IODevice *device) {
	d->device = device;
}

IODevice *Commander::
getDevice() const {
	return d->device;
}

Command *Commander::
readCommand() {
	IODevice *dd = d->device;
	Command *ret = NULL;
	assert(dd != 0);

	CommandHeader hdr;
	char *buf = 0;
	// Read header
	{
		if (dd->read((char *)&hdr,sizeof(hdr)) != sizeof(hdr)) {
			return NULL;
		}
	}

	// Read command
	{
		buf = (char *)::malloc(hdr.length);
		int tried_times = 0;
		size_t offset = 0;
		do {
			int rc = dd->read( (char *)(buf + offset), hdr.length-offset );
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

int Commander::
writeCommand (const Command * cmd) {
	IODevice *dd = d->device;
	Command *ret = NULL;
	assert(dd != 0);

	CommandHeader hdr;
	std::stringbuf buf;
	// Write header
	{

	}
}
