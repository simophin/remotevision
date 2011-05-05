/*
 * CommandBuilder.cpp
 *
 *  Created on: 2011-5-5
 *      Author: launliu
 */

#include "CommandBuilder.h"

#include <vector>
#include <sstream>

class CommandBuilder::Impl {
public:
	std::string cmd;
	std::vector<std::string> argList;
};

CommandBuilder::CommandBuilder()
:d(new CommandBuilder::Impl){

}

CommandBuilder::~CommandBuilder() {
	delete d;
}

void CommandBuilder::
setRequestCommand (const std::string & request){
	d->cmd = request;
}
void CommandBuilder::
setResponseCommand (const std::string &cmd, const std::string &error){
	d->cmd = cmd;
	if (!error.empty()) {
		d->cmd += (":" + error);
	}
}

void CommandBuilder::
appendArgument (const std::string & arg){
	d->argList.push_back(arg);
}
void CommandBuilder::
clearArguments(){
	d->argList.clear();
}

std::string CommandBuilder::
build() {
	std::stringstream ret;
	ret << d->cmd << '\0';
	for (int i=0;i<d->argList.size();i++){
		ret << d->argList[i] << '\0';
	}
	return ret.str();
}
