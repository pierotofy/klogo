/*
 * StringTable.cc
 *
 *  Created on: Feb 8, 2011
 *      Author: toffa006
 */

#include "StringTable.hh"

StringTable::StringTable(){
}

void StringTable::Add(const string &key, int token){
	assert(!IsEntryInTable(key));

	data[key] = new StringTableEntry(key, token);
}

StringTableEntry* StringTable::GetEntry(const string &key){
	StringTableEntry *result = data[key];
	assert(result != 0);

	return result;
}

/* Is an item already in our string table? */
bool StringTable::IsEntryInTable(const string &key){
	return (data.count(key) > 0);
}


