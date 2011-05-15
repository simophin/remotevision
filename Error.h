/*
 * Error.h
 *
 *  Created on: 2011-5-14
 *      Author: simophin
 */

#ifndef ERROR_H_
#define ERROR_H_

#include <string>
#include <ostream>

typedef int syserrno_t;
typedef std::wstring errorstring_t;
extern syserrno_t syserrno_success;

class Error {
public:
	explicit Error(syserrno_t e = syserrno_success);
	explicit Error(const std::string &);
	virtual ~Error();

	syserrno_t getErrno() const;
	void setErrno(syserrno_t,bool fetchFromSystem = true);

	errorstring_t getErrorString () const;
	void setErrorString (const errorstring_t &);

	friend std::wostream& operator <<(std::wostream &os,const Error &obj);

	class Impl;
private:
	Impl *d;
};


#endif /* ERROR_H_ */