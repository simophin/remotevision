/*
 * CommandBuilder.h
 *
 *  Created on: 2011-5-5
 *      Author: launliu
 */

#ifndef COMMANDBUILDER_H_
#define COMMANDBUILDER_H_

#include "RString.h"

class Command;
class CommandBuilder {
public:
	CommandBuilder();
	virtual ~CommandBuilder();

	void setRequestCommand (const String &);
	void setResponseCommand (const String &cmd, const String &error = String());

	void appendArgument (const String &);
	void clearArguments();

	void build(Command &);

	class Impl;
private:
	Impl *d;
};

#endif /* COMMANDBUILDER_H_ */
