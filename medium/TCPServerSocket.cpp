/*
 * TCPServerSocket.cpp
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#include "TCPServerSocket.h"

TCPServerSocket::TCPServerSocket(int fd)
:PosixServerSocket(fd){

}

TCPServerSocket::~TCPServerSocket() {
}
