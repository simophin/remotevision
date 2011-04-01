/*
 * CommandReader.h
 *
 *  Created on: 2011-3-31
 *      Author: simophin
 */

#ifndef COMMANDREADER_H_
#define COMMANDREADER_H_

#include <boost/noncopyable.hpp>
#include <memory>

class IODevice;
class Command;
class CommandReader: public boost::noncopyable {
public:
	CommandReader(IODevice *device = 0);
	virtual ~CommandReader();

	void setDevice(IODevice *);
	IODevice *getDevice() const;

	Command *readCommand();

private:
	class Impl;
	std::auto_ptr<Impl> d;
};

#endif /* COMMANDREADER_H_ */
