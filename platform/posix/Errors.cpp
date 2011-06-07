/*
 * Errors.cpp
 *
 *  Created on: 2011-6-8
 *      Author: simophin
 */


#include "Error.h"
#include "utils/Misc.hpp"
#include <errno.h>
#include <vector>
#include <string.h>

std::vector<Error::SystemMap> ERROR_MAPS;
syserrno_t SYS_ERR_SUCCESS = 0;
syserrno_t SYS_ERR_UNKNOWN = -1;

struct _map {
	Error::Type type;
	syserrno_t system;
	const char *msg;
};

static struct _map MAPS[] = {
		{ Error::ERR_SUCCESS, 0, "Success"},
		{ Error::ERR_UNKNOWN, -1, "Unknown error"},
		{ Error::ERR_SYS_UNKNOWN, -1, "Unknown error"},
		{ Error::ERR_STATE, ERESTART, 0},
		{ Error::ERR_TIMEOUT, ETIMEDOUT,0},
		{ Error::ERR_INVALID, EINVAL, 0}
};

class PosixErrorInitializer {
public:
	PosixErrorInitializer() {
		init();
	}

	void init () {
		Error::SystemMap map;
		for (int i=0; i<ARRAY_SIZE(MAPS); i++) {
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
};

#ifdef STATIC_INITIALIZER
static PosixErrorInitializer initializer();
#endif

