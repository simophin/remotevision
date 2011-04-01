/*
 * CommandMgr.cpp
 *
 *  Created on: 2011-4-1
 *      Author: simophin
 */

#include "CommandMgr.h"
#include "Command.h"

#include <list>
#include <boost/unordered_map.hpp>

typedef std::list<CommandHandler> CommandHandlerList;
typedef boost::unordered_map<std::string, CommandHandlerList> CommandMap;

static CommandMap commandMap;

void CommandMgr::
handleCommand(Command *cmd)
{
	CommandMap::const_iterator found = commandMap.find(cmd->getName());
	if (found == commandMap.end()) return;
	CommandHandlerList hlist = found.second;
}



void CommandMgr::
registerCommandHandler(const std::string & name, CommandHandler handler)
{
}

