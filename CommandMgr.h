/*
 * CommandMgr.h
 *
 *  Created on: 2011-4-1
 *      Author: simophin
 */

#ifndef COMMANDMGR_H_
#define COMMANDMGR_H_

#include <string>

class Command;
class CommandContext;

typedef void (*CommandHandler) (Command *, const CommandContext *);

class CommandMgr {
public:
	static void handleCommand (Command *);
	static void registerCommandHandler (const std::string &name, CommandHandler);
};

#endif /* COMMANDMGR_H_ */
