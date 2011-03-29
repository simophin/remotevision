/*
 * TCPSocket.cpp
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#include "TCPSocket.h"

TCPSocket::TCPSocket(int fd)
:PosixSocket(fd){

}

TCPSocket::~TCPSocket() {
}
