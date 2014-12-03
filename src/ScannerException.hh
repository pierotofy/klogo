/*
 * ParseException.h
 *
 *  Created on: Feb 10, 2011
 *      Author: toffa006
 */

#ifndef SCANNEREXCEPTION_HH_
#define SCANNEREXCEPTION_HH_

#include <stdexcept>
#include <string>

class ScannerException : public std::exception {
public:
	ScannerException(string m="ScannerException error!") : pMessage(m) {}
	~ScannerException() throw() {}
	const char* what() const throw() { return pMessage.c_str(); }

private:
  string pMessage;
};


#endif /* SCANNEREXCEPTION_HH_ */
