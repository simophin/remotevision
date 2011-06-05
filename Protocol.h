/*
 * Protocol.h
 *
 *  Created on: 2011-3-31
 *      Author: simophin
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include <stdint.h>

struct CommandHeader {
	uint32_t length;
	uint8_t  padding[20];

	CommandHeader()
	:length(0) {

	}
};

#endif /* PROTOCOL_H_ */
