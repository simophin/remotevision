/*
 * QueryInfoCommandHandler.h
 *
 *  Created on: 2011-5-4
 *      Author: simophin
 */

#ifndef QUERYINFOCOMMANDHANDLER_H_
#define QUERYINFOCOMMANDHANDLER_H_

#include "CommandHandler.h"

class QueryInfoCommandHandler: public CommandHandler {
public:
	QueryInfoCommandHandler();
	virtual ~QueryInfoCommandHandler();
	virtual void onHandle(Command *, const CommandContext *);
};

#endif /* QUERYINFOCOMMANDHANDLER_H_ */
