/*
 * Win32SocketAddress.h
 *
 *  Created on: 2011-4-29
 *      Author: launliu
 */

#ifndef WIN32SOCKETADDRESS_H_
#define WIN32SOCKETADDRESS_H_

#include "SocketAddress.h"

class Win32SocketAddress: public SocketAddress {
public:
	Win32SocketAddress();
	virtual ~Win32SocketAddress();
};

#endif /* WIN32SOCKETADDRESS_H_ */
