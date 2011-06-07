/*
 * CommandBuilder.cpp
 *
 *  Created on: 2011-5-5
 *      Author: launliu
 */

#include "CommandBuilder.h"
#include "Command.h"

#include <vector>
#include <sstream>

class CommandBuilder::Impl {
public:
	String cmd;
	std::vector<String> argList;
};

CommandBuilder::CommandBuilder()
:d(new CommandBuilder::Impl){

}

CommandBuilder::~CommandBuilder() {
	delete d;
}

void CommandBuilder::
setRequestCommand (const String & request){
	d->cmd = request;
}
void CommandBuilder::
setResponseCommand (const String &cmd, const String &error){
	d->cmd = cmd;
	if (!error.empty()) {
		d->cmd += (":" + error);
	}
}

void CommandBuilder::
appendArgument (const String & arg){
	d->argList.push_back(arg);
}
void CommandBuilder::
clearArguments(){
	d->argList.clear();
}

void CommandBuilder::
build(Command &cmd) {
	cmd.setName(d->cmd);
	cmd.setArguments(d->argList);
}
