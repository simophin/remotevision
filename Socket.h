/*
 * Socket.h
 *
 *  Created on: 2011-3-28
 *      Author: simophin
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <boost/noncopyable.hpp>
#include <string>

class SocketAddress;
class Socket: public boost::noncopyable {
public:
	Socket();
	virtual ~Socket();

	const SocketAddress * getAddress() const;
	ssize_t read  (unsigned char *, size_t);
	ssize_t write (const unsigned char *, size_t);
	void close();

protected:
	virtual ssize_t doRead (unsigned char *, size_t) = 0;
	virtual ssize_t doWrite (const unsigned char *, size_t) = 0;
	virtual void doClose();
	virtual const SocketAddress * doGetAddress() const = 0;
};

#endif /* SOCKET_H_ */
