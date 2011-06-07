/*
 * Error.h
 *
 *  Created on: 2011-5-14
 *      Author: simophin
 */

#ifndef ERROR_H_
#define ERROR_H_

#include <ostream>
#include "RString.h"

typedef int syserrno_t;
typedef String errorstring_t;
extern syserrno_t syserrno_success;

class Error {
public:
	explicit Error(syserrno_t e = syserrno_success);
	explicit Error(const errorstring_t &);
	explicit Error(const char *, size_t size = 0);
	virtual ~Error();


	enum Type {
		ERR_UNKNOWN,
		ERR_SYS_UNKNOWN,
		ERR_STATE,
	};


	void setSystemErrno (syserrno_t, bool fetchFromSystem = true, const String & errMsg = String());
	syserrno_t getSystemErrno () const;




	syserrno_t getErrno() const;
	void setErrno(syserrno_t,bool fetchFromSystem = true);

	errorstring_t getErrorString () const;
	void setErrorString (const errorstring_t &);
	void setErrorString (const char *, size_t size = 0);

	String toString() const;
	static Error fromString(const String &,bool *ok = NULL);

	friend std::ostream& operator <<(std::ostream &os,const Error &obj);

private:
	syserrno_t errorNumber;
	errorstring_t errorString;
};


#endif /* ERROR_H_ */
