/*
 * QueryInfoCommandHandler.h
 *
 *  Created on: 2011-5-4
 *      Author: simophin
 */

#ifndef QUERYINFOCOMMANDHANDLER_H_
#define QUERYINFOCOMMANDHANDLER_H_

#include "CommandHandler.h"

class VideoInfo;
class Command;
class QueryInfoCommandHandler: public CommandHandler {
public:
	QueryInfoCommandHandler();
	virtual ~QueryInfoCommandHandler();
	virtual void onHandle(const Command &, const CommandContext *);

	static VideoInfo parseVideoInformationFromCommand(const Command &);
};

#endif /* QUERYINFOCOMMANDHANDLER_H_ */
