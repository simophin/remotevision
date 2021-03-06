/*
 * Errors.cpp
 *
 *  Created on: 2011-6-8
 *      Author: simophin
 */

#include "Error.h"

#include <errno.h>
#include <windows.h>
#include <vector>

#include "utils/Misc.hpp"

std::vector<Error::SystemMap> ERROR_MAPS;
syserrno_t SYS_ERR_SUCCESS = 0;
syserrno_t SYS_ERR_UNKNOWN = 15;

struct _map {
	Error::Type type;
	syserrno_t system;
	const char *msg;
};

static struct _map MAPS[] = {
		{ Error::ERR_SUCCESS, 0, "Success"},
		{ Error::ERR_UNKNOWN, SYS_ERR_UNKNOWN, "Unknown error"},
		{ Error::ERR_SYS_UNKNOWN, SYS_ERR_UNKNOWN, "Unknown error"},
		{ Error::ERR_STATE, WSAEINVAL , 0},
		{ Error::ERR_TIMEOUT, WSAETIMEDOUT, 0},
		{ Error::ERR_INVALID, WSAEINVAL, 0},
		{ Error::ERR_ADDRINUSE,WSAEADDRINUSE, 0},
};


void initErrors () {
	Error::SystemMap map;
	for (unsigned i=0; i<ARRAY_SIZE(MAPS); i++) {
		map.system = MAPS[i].system;
		map.type = MAPS[i].type;
		if (MAPS[i].msg == 0) {
			map.message = ::strerror(map.system);
		}else{
			map.message = MAPS[i].msg;
		}
		ERROR_MAPS.push_back(map);
	}
}
