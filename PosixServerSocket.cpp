/*
 * PosixServerSocket.cpp
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#include "PosixServerSocket.h"

PosixServerSocket::PosixServerSocket(int fd)
:PosixSocket(fd){
}

PosixServerSocket::~PosixServerSocket() {
}


SocketAddress *PosixServerSocket::
doAccept(){

}
const SocketAddress * PosixServerSocket::
doGetAddress() const {

}
