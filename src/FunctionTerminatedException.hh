/*
 * FunctionTerminatedException.hh
 *
 *  Created on: Apr 22, 2011
 *      Author: toffa006
 */

#ifndef FUNCTIONTERMINATEDEXCEPTION_HH_
#define FUNCTIONTERMINATEDEXCEPTION_HH_

#include <stdexcept>
#include <string>

class Value;

using namespace std;

class FunctionTerminatedException : public std::exception {
public:
	FunctionTerminatedException(Value *retVal) : returnValue(retVal) {}
	~FunctionTerminatedException() throw() {}
	const char* what() const throw() { return "Function returned."; }
	Value *getValue() const{ return returnValue; }

private:
  Value *returnValue;
};

#endif /* FUNCTIONTERMINATEDEXCEPTION_HH_ */
