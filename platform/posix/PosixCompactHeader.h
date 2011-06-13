/*
 * PosixCompactHeader.h
 *
 *  Created on: 2011-5-13
 *      Author: simophin
 */

#ifndef POSIXCOMPACTHEADER_H_
#define POSIXCOMPACTHEADER_H_


#ifdef OS_WIN32
#include <winsock.h>
typedef int socklen_t;
typedef unsigned long in_addr_t;
#endif

#ifdef OS_UNIX
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <fcntl.h>
#endif


#endif /* POSIXCOMPACTHEADER_H_ */
