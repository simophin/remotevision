/*
 * CommandBuilder.h
 *
 *  Created on: 2011-5-5
 *      Author: launliu
 */

#ifndef COMMANDBUILDER_H_
#define COMMANDBUILDER_H_

#include <string>

class CommandBuilder {
public:
	CommandBuilder();
	virtual ~CommandBuilder();

	void setRequestCommand (const std::string &);
	void setResponseCommand (const std::string &cmd, const std::string &error = std::string());

	void appendArgument (const std::string &);
	void clearArguments();

	std::string build();

	class Impl;
private:
	Impl *d;
};

#endif /* COMMANDBUILDER_H_ */
