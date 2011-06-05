/*
 * CommandMgr.cpp
 *
 *  Created on: 2011-4-1
 *      Author: simophin
 */

#include "CommandMgr.h"
#include "Command.h"
#include "CommandHandler.h"
#include "commands/VideoCommand.h"

#include <list>
#include <utility>
#include <boost/unordered_map.hpp>

typedef std::list<CommandHandler *> CommandHandlerList;
typedef boost::unordered_map<std::string, CommandHandlerList> CommandMap;

class CommandMgr::Impl {
public:
	CommandMap commandMap;
	bool hasRegistered;

	Impl()
	:hasRegistered(false) {

	}
};

CommandMgr::CommandMgr()
:d(new CommandMgr::Impl) {
}

CommandMgr::~CommandMgr() {
	for (CommandMap::const_iterator iter = d->commandMap.begin();
			iter != d->commandMap.end(); ++ iter) {
		for (CommandHandlerList::const_iterator i = iter->second.begin();
				i != iter->second.end(); ++ i) {
			delete *i;
		}
	}
	delete d;
}

void CommandMgr::
handleCommand(const Command &cmd, const CommandContext *ctx)
{
	CommandMap::const_iterator found = d->commandMap.find(cmd.getName());
	if (found == d->commandMap.end()) return;
	CommandHandlerList hlist = found->second;
	for (CommandHandlerList::const_iterator i = hlist.begin();
			i != hlist.end();
			++ i) {
		(*i)->onHandle(cmd,ctx);
	}
}



void CommandMgr::
registerCommandHandler(CommandHandler *handler)
{
	d->commandMap[handler->getCommandName()].push_back(handler);
}

void CommandMgr::registerAllCommands()
{
	if (d->hasRegistered) return;
	registerCommandHandler(new VideoCommand::QueryInfoCommandHandler);
	registerCommandHandler(new VideoCommand::SetParameterCommandHandler);
	registerCommandHandler(new VideoCommand::GetParameterCommandHandler);
	registerCommandHandler(new VideoCommand::StartCaptureCommandHandler);
	registerCommandHandler(new VideoCommand::StopCaptureCommandHandler);
	d->hasRegistered = true;
}


