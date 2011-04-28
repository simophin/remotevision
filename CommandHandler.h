/*
 * CommandHandler.h
 *
 *  Created on: 2011-4-28
 *      Author: simophin
 */

#ifndef COMMANDHANDLER_H_
#define COMMANDHANDLER_H_

class Command;
class CommandContext;
class CommandHandler {
public:
	CommandHandler();
	virtual ~CommandHandler();

	virtual void onHandle(Command *, const CommandContext *) = 0;
};

#endif /* COMMANDHANDLER_H_ */
