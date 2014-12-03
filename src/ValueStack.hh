/*
 * ValueStack.hh
 *
 *  Created on: Apr 21, 2011
 *      Author: toffa006
 */

#ifndef VALUESTACK_HH_
#define VALUESTACK_HH_

#include <stack>
#include "Value.hh"
#include "RuntimeException.hh"

using namespace std;

class ValueStack {
	stack<Value *> values;
public:

	ValueStack(){}

	void Allocate(){
		values.push((Value *)new UnknownValue());
	}

	void Deallocate(){
		values.pop();
	}

	Value *get(){
		// Check for initialization
		Value *result = values.top();
		if (!result->initialized()) throw RuntimeException("Trying to access an uninitialized variable.");

		return result;
	}

	void set(Value *value){
		values.top() = value;
	}

	void print(std::ostream& os = std::cout){
		stack<Value *> backupValues;
		while(!values.empty()){
			Value *value = values.top();
			values.pop();
			value->print(os);
			os << " --> ";
			backupValues.push(value);
		}

		os << " | ";

		// Restore backup
		while(!backupValues.empty()){
			values.push(backupValues.top());
			backupValues.pop();
		}
	}

};

#endif /* VALUESTACK_HH_ */
