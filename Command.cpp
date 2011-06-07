/*
 * Command.cpp
 *
 *  Created on: 2011-3-31
 *      Author: simophin
 */

#include "Command.h"

Command::Command(const String &name, const std::vector<String> &args)
:mName(name),mArguments(args){

}

Command::~Command() {

}
