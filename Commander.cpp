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



class Commander::Impl {
public:
	IODevice *device;
	Error lastError;
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

bool Commander::
readCommand(Command &cmd) {
	IODevice *dd = d->device;
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
				goto out;
			}
			offset += rc;
		}while(tried_times++ < 20 && offset < hdr.length);
	}
	{
		std::string name;
		std::vector<std::string> args;
		size_t offset = 0;

		name = buf;
		offset += ::strlen(buf)+1;


		while( (unsigned int)offset < (unsigned )hdr.length) {
			args.push_back(buf+offset);
			offset += ::strlen(buf+offset)+1;
		}

		cmd.setName(name);
		cmd.setArguments(args);
	}

	if (buf) ::free(buf);
	return true;

	out:
	if (buf) ::free(buf);
	return false;
}

bool Commander::
writeCommand (const Command & cmd) {
	IODevice *dd = d->device;
	assert(dd != 0);


	std::stringstream buf;
	buf << cmd.getName() << '\0';

	std::vector<std::string> args = cmd.getArguments();
	for (int i=0;i<args.size();i++) {
		buf << args[i] << '\0';
	}

	std::string data = buf.str();
	CommandHeader hdr;
	// Write header
	{
		hdr.length = data.size();
	}

	// Write data
	{
		if (dd->write((char *)&hdr, sizeof(hdr)) < 0 ) {
			d->lastError = dd->getLastError();
			return false;
		}
		if (dd->write(data.c_str(), data.size()) < 0 ) {
			d->lastError = dd->getLastError();
			return false;
		}
	}

	return true;
}

Error Commander::
getLastError() const {
	return d->lastError;
}
