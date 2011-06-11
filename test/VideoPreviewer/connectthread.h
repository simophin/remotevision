/*
 * connectthread.h
 *
 *  Created on: 2011-6-11
 *      Author: simophin
 */

#ifndef CONNECTTHREAD_H_
#define CONNECTTHREAD_H_

#include <QtCore/QThread>
#include <QtCore/QString>


class TCPSocket;
class VideoSource;
class ConnectThread: public QThread {
	Q_OBJECT
Q_SIGNALS:
	void success();
	void error(QString);

public:
	ConnectThread(QObject *parent = 0);
	virtual ~ConnectThread();

	void establishConnection (const QString &addr, int port);
	virtual void run ();

	VideoSource * getVideoSource() const;

private:
	VideoSource *mVideoSource;
	QString mAddress;
	int mPort;

	volatile bool mShouldStop;
};

#endif /* CONNECTTHREAD_H_ */
