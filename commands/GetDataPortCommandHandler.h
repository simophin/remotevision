/*
 * GetDataPortCommandHandler.h
 *
 *  Created on: 2011-6-9
 *      Author: simophin
 */

#ifndef GETDATAPORTCOMMANDHANDLER_H_
#define GETDATAPORTCOMMANDHANDLER_H_

#include "CommandHandler.h"

class GetDataPortCommandHandler: public CommandHandler {
public:
	class Info {
	public:
		unsigned port;
	};


	GetDataPortCommandHandler();
	virtual ~GetDataPortCommandHandler();
};

#endif /* GETDATAPORTCOMMANDHANDLER_H_ */
