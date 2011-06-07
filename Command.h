/*
 * Command.h
 *
 *  Created on: 2011-3-31
 *      Author: simophin
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include <vector>
#include "String.h"

class Command {
public:
	Command(const String &name = String(), const std::vector<String> &args =
			std::vector<String> ());
	virtual ~Command();

	inline String getName() const {
		return mName;
	}

	inline std::vector<String> getArguments() const {
		return mArguments;
	}

	inline String getArgument(int i) const {
		return mArguments.at(i);
	}

	inline void setName(const String &n) {
		mName = n;
	}

	inline void setArguments(const std::vector<String> &arg) {
		mArguments = arg;
	}

private:
	String mName;
	std::vector<String> mArguments;
};

#endif /* COMMAND_H_ */
