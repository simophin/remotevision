/*
 * CommandReader.h
 *
 *  Created on: 2011-3-31
 *      Author: simophin
 */

#ifndef COMMANDER_H_
#define COMMANDER_H_

#include <memory>
#include "RString.h"

#include "Error.h"
#include "utils/NonCopyable.hpp"

class IODevice;
class Command;
class Commander: public Utils::NonCopyable {
public:
	Commander(IODevice *device = 0);
	virtual ~Commander();

	void setDevice(IODevice *);
	IODevice *getDevice() const;

	Error readCommand(Command &);
	Error writeCommand (const Command &);

private:
	class Impl;
	std::auto_ptr<Impl> d;
};

#endif /* COMMANDER_H_ */
