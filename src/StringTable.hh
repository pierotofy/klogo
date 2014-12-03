/*
 * StringTable.h
 *
 *  Created on: Feb 8, 2011
 *      Author: toffa006
 */

#ifndef STRINGTABLE_HH_
#define STRINGTABLE_HH_

#include <string>
#include <assert.h>
#include <map>
#include "StringTableEntry.hh"

using namespace std;

class StringTable{
	map<string, StringTableEntry *> data;
public:
	StringTable();

	/* Adds a new value to the string table */
	void Add(const string &key, int token);

	/* Returns a pointer to an element in the string table */
	StringTableEntry *GetEntry(const string &key);

	/* Is an item already in our string table? */
	bool IsEntryInTable(const string &key);
};

#endif /* STRINGTABLE_HH_ */
