/*
 * Command.h
 *
 *  Created on: 2011-3-31
 *      Author: simophin
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include <string>
#include <vector>

class Command {
public:
	Command(const std::string &name = std::string(), const std::vector<std::string> &args =
			std::vector<std::string> ());
	virtual ~Command();

	inline std::string getName() const {
		return mName;
	}

	inline std::vector<std::string> getArguments() const {
		return mArguments;
	}

	inline void setName(const std::string &n) {
		mName = n;
	}

	inline void setArguments(const std::vector<std::string> &arg) {
		mArguments = arg;
	}

private:
	std::string mName;
	std::vector<std::string> mArguments;
};

#endif /* COMMAND_H_ */
