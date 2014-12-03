#include "StringTableEntry.hh"

StringTableEntry::StringTableEntry(const string &name, int token){
	this->token = token;
	this->name = string(name);
	value = 0;
}
