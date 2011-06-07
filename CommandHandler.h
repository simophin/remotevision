/*
 * CommandHandler.h
 *
 *  Created on: 2011-4-28
 *      Author: simophin
 */

#ifndef COMMANDHANDLER_H_
#define COMMANDHANDLER_H_

#include "RString.h"

class Command;
class CommandContext;
class CommandHandler {
public:
	CommandHandler(const String &cmdName);
	virtual ~CommandHandler();

	virtual void onHandle(const Command &, const CommandContext *) = 0;

	String getCommandName() const { return mKey; }

private:
	String mKey;
};

#endif /* COMMANDHANDLER_H_ */
