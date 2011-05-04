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
class CommandHandler;

class CommandMgr {
public:
	CommandMgr();
	~CommandMgr();
	void handleCommand (Command *, const CommandContext *);
	void registerCommandHandler (const std::string &name, CommandHandler *);

	static CommandMgr * getInstance();

private:
	class Impl;
	Impl *d;
};

#endif /* COMMANDMGR_H_ */