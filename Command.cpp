/*
 * Command.cpp
 *
 *  Created on: 2011-3-31
 *      Author: simophin
 */

#include "Command.h"

Command::Command(const std::string &name, const std::list<std::string> &args)
:mName(name),mArguments(args){

}

Command::~Command() {

}
