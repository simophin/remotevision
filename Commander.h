/*
 * CommandReader.h
 *
 *  Created on: 2011-3-31
 *      Author: simophin
 */

#ifndef COMMANDER_H_
#define COMMANDER_H_

#include <boost/noncopyable.hpp>
#include <memory>
#include <string>

class IODevice;
class Command;
class Commander: public boost::noncopyable {
public:
	Commander(IODevice *device = 0);
	virtual ~Commander();

	void setDevice(IODevice *);
	IODevice *getDevice() const;

	bool readCommand(Command &);
	bool writeCommand (const Command &);

	std::string getLastError() const;

private:
	class Impl;
	std::auto_ptr<Impl> d;
};

#endif /* COMMANDER_H_ */
