/*
 * NonCopyable.hpp
 *
 *  Created on: 2011-6-7
 *      Author: simophin
 */

#ifndef NONCOPYABLE_HPP_
#define NONCOPYABLE_HPP_

namespace Utils {
class NonCopyable {
protected:
	NonCopyable() {}
	~NonCopyable() {}
private:  // emphasize the following members are private
	NonCopyable( const NonCopyable& );
	const NonCopyable& operator=( const NonCopyable& );

};
}


#endif /* NONCOPYABLE_HPP_ */
