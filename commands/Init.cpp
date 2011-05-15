/*
 * Init.cpp
 *
 *  Created on: 2011-5-4
 *      Author: simophin
 */

#include "QueryInfoCommandHandler.h"
#include "CommandMgr.h"

void initCommands() {
	CommandMgr::getInstance()->registerCommandHandler( new QueryInfoCommandHandler);
}
