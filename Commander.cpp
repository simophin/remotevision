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
#include "RString.h"
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

Error Commander::
readCommand(Command &cmd) {
	Error rc;
	IODevice *dd = d->device;
	assert(dd != 0);

	CommandHeader hdr;
	char *buf = 0;
	// Read header
	{
		size_t read_size;
		rc = dd->read((char *)&hdr,sizeof(hdr), &read_size);
		if (!rc.isSuccess()) {
			return rc;
		}
		if( read_size != sizeof(hdr)) {
			rc.setErrorType(Error::ERR_INVALID, "header corrupted");
			return rc;
		}
	}

	// Read command
	{
		buf = (char *)::malloc(hdr.length);
		int tried_times = 0;
		size_t offset = 0;
		size_t read_size;
		do {
			rc = dd->read( (char *)(buf + offset), hdr.length-offset, &read_size);
			if (!rc.isSuccess()) {
				goto out;
			}
			offset += read_size;
		}while(tried_times++ < 20 && offset < hdr.length);
	}
	{
		String name;
		std::vector<String> args;
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
	return rc;

	out:
	if (buf) ::free(buf);
	return rc;
}

Error Commander::
writeCommand (const Command & cmd) {
	IODevice *dd = d->device;
	Error rc;
	assert(dd != 0);


	std::stringstream buf;
	buf << cmd.getName() << '\0';

	std::vector<String> args = cmd.getArguments();
	for (unsigned i=0;i<args.size();i++) {
		buf << args[i] << '\0';
	}

	String data = buf.str();
	CommandHeader hdr;
	// Write header
	{
		hdr.length = data.size();
	}

	// Write data
	{
		rc = dd->write((char *)&hdr, sizeof(hdr));
		if ( !rc.isSuccess()) {
			return rc;
		}
		rc = dd->write(data.c_str(), data.size());
		if ( !rc.isSuccess() ) {
			return rc;
		}
	}

	return rc;
}
