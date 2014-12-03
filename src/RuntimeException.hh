/*
 * RuntimeException.hh
 *
 *  Created on: Apr 22, 2011
 *      Author: toffa006
 */

#ifndef RUNTIMEEXCEPTION_HH_
#define RUNTIMEEXCEPTION_HH_

#include <stdexcept>
#include <string>

using namespace std;

class RuntimeException : public std::exception {
public:
	RuntimeException(string m="Runtime exception!") : pMessage(m) {}
	~RuntimeException() throw() {}
	const char* what() const throw() { return pMessage.c_str(); }

private:
  string pMessage;
};

#endif /* RUNTIMEEXCEPTION_HH_ */
