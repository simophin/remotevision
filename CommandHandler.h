/*
 * CommandHandler.h
 *
 *  Created on: 2011-4-28
 *      Author: simophin
 */

#ifndef COMMANDHANDLER_H_
#define COMMANDHANDLER_H_

#include <string>

class Command;
class CommandContext;
class CommandHandler {
public:
	CommandHandler(const std::string &cmdName);
	virtual ~CommandHandler();

	virtual void onHandle(const Command &, const CommandContext *) = 0;

	std::string getCommandName() const { return mKey; }

private:
	std::string mKey;
};

#endif /* COMMANDHANDLER_H_ */
