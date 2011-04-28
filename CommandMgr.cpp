/*
 * CommandMgr.cpp
 *
 *  Created on: 2011-4-1
 *      Author: simophin
 */

#include "CommandMgr.h"
#include "Command.h"
#include "CommandHandler.h"

#include <list>
#include <utility>
#include <boost/unordered_map.hpp>

typedef std::list<CommandHandler *> CommandHandlerList;
typedef boost::unordered_map<std::string, CommandHandlerList> CommandMap;

static CommandMgr * INSTANCE = NULL;

class CommandMgr::Impl {
public:
	CommandMap commandMap;
};

CommandMgr::CommandMgr()
:d(new CommandMgr::Impl) {
	INSTANCE = this;
}

CommandMgr::~CommandMgr() {
	delete d;
}

void CommandMgr::
handleCommand(Command *cmd, const CommandContext *ctx)
{
	CommandMap::const_iterator found = d->commandMap.find(cmd->getName());
	if (found == d->commandMap.end()) return;
	CommandHandlerList hlist = found->second;
	for (CommandHandlerList::const_iterator i = hlist.begin();
			i != hlist.end();
			++ i) {
		(*i)->onHandle(cmd,ctx);
	}
}



void CommandMgr::
registerCommandHandler(const std::string & name, CommandHandler *handler)
{
	d->commandMap[name].push_back(handler);
}

CommandMgr * CommandMgr::
getInstance() {
	return INSTANCE;
}
