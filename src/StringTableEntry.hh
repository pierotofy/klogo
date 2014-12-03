/*
 * StringTableEntry.h
 *
 *  Created on: Feb 7, 2011
 *      Author: toffa006
 */

#ifndef STRINGTABLEENTRY_HH_
#define STRINGTABLEENTRY_HH_

#include <iostream>
#include <string>

class ValueStack;

using namespace std;

/* Rappresents an item in the string table */
class StringTableEntry{
	string name;
	int token;

public:
	StringTableEntry(const string&, int);

	string* GetName() { return &name; }
	int GetToken() { return token; }

	ValueStack *value; // Not all instances have this value
};

#endif /* STRINGTABLEENTRY_HH_ */
