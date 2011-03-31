/*
 * Command.h
 *
 *  Created on: 2011-3-31
 *      Author: simophin
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include <string>
#include <list>

class Command {
public:
	Command(const std::string &name = std::string(), const std::list<std::string> &args =
			std::list<std::string> ());
	virtual ~Command();

	inline std::string getName() const {
		return mName;
	}

	inline std::list<std::string> getArguments() const {
		return mArguments;
	}

	inline void setName(const std::string &n) {
		mName = n;
	}

	inline void setArguments(const std::list<std::string> &arg) {
		mArguments = arg;
	}

private:
	std::string mName;
	std::list<std::string> mArguments;
};

#endif /* COMMAND_H_ */
