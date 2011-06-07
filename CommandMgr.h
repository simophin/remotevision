/*
 * CommandMgr.h
 *
 *  Created on: 2011-4-1
 *      Author: simophin
 */

#ifndef COMMANDMGR_H_
#define COMMANDMGR_H_

#include "RString.h"

class Command;
class CommandContext;
class CommandHandler;

class CommandMgr {
public:
	CommandMgr();
	~CommandMgr();
	void handleCommand (const Command &, const CommandContext *);
	void registerCommandHandler ( CommandHandler *);

	void registerAllCommands ();

private:
	class Impl;
	Impl *d;
};

#endif /* COMMANDMGR_H_ */
