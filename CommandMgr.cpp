/*
 * CommandMgr.cpp
 *
 *  Created on: 2011-4-1
 *      Author: simophin
 */

#include "CommandMgr.h"
#include "Command.h"

#include <list>
#include <utility>
#include <boost/unordered_map.hpp>

typedef std::list<CommandHandler> CommandHandlerList;
typedef boost::unordered_map<std::string, CommandHandlerList> CommandMap;

static CommandMap commandMap;

void CommandMgr::
handleCommand(Command *cmd, const CommandContext *ctx)
{
	CommandMap::const_iterator found = commandMap.find(cmd->getName());
	if (found == commandMap.end()) return;
	CommandHandlerList hlist = found->second;
	for (CommandHandlerList::const_iterator i = hlist.begin();
			i != hlist.end();
			++ i) {
		(*i)(cmd,ctx);
	}
}



void CommandMgr::
registerCommandHandler(const std::string & name, CommandHandler handler)
{
	commandMap[name].push_back(handler);
}

