/*
 * SymbolTable.hh
 *
 *  Created on: Apr 12, 2011
 *      Author: toffa006
 */

#ifndef SYMBOLTABLE_HH_
#define SYMBOLTABLE_HH_

#include "StringTableEntry.hh"
#include "SymbolTableEntry.hh"
#include <map>
#include <list>

class PartOfScope
{
	int scope;
public:
	PartOfScope(int scope)
	{
		this->scope = scope;
	}

	bool operator()(SymbolTableEntry& entry)
	{
		return entry.GetScopeLevel() == scope;
	}
};


class SymbolTable{
private:
	typedef list<SymbolTableEntry> SymbolTableList;
	typedef map<StringTableEntry *, SymbolTableList > SymbolTableMap;
	SymbolTableMap data;

	int currentScopeLevel;

	void IncreaseScopeLevel(){
		currentScopeLevel++;
	}

	void DecreaseScopeLevel(){
		currentScopeLevel--;
	}

	/* Delete all items that belong to a certain scope level */
	void ClearScope(int scopeLevel){
		SymbolTableMap::const_iterator end = data.end();
	    for (SymbolTableMap::const_iterator mapItem = data.begin(); mapItem != end; ++mapItem)
	    {
	    	SymbolTableList *list = (SymbolTableList *)&mapItem->second; //second = value

	    	list->remove_if(PartOfScope(scopeLevel));
	    }
	}
public:

	SymbolTable() { currentScopeLevel = 0; }

	void AddSymbol(StringTableEntry *stringTableEntry, ExpressionType type){
		data[stringTableEntry].push_back(SymbolTableEntry(type, currentScopeLevel));
	}

	int GetCurrentScopeLevel(){
		return currentScopeLevel;
	}


	SymbolTableEntry *LookupSymbol(StringTableEntry *stringTableEntry){
		SymbolTableEntry *result = 0;

    	SymbolTableList list = data[stringTableEntry];

	    for (SymbolTableList::reverse_iterator listItem = list.rbegin(); listItem != list.rend(); listItem++){
			if (listItem->GetScopeLevel() <= currentScopeLevel){
				result = &*listItem;
				break;
			}
	    }

	    assert(result != 0);

	    return result;
	}

	bool IsSymbolInCurrentOrLowerScopeLevel(StringTableEntry *stringTableEntry){

    	SymbolTableList list = data[stringTableEntry];

    	// Need to look inside the list
	    for (SymbolTableList::iterator listItem = list.begin(); listItem != list.end(); listItem++){
			if (listItem->GetScopeLevel() <= currentScopeLevel){
				return true;
			}
	    }

	    return false;
	}


	bool IsSymbolInCurrentScopeLevel(StringTableEntry *stringTableEntry){

    	SymbolTableList list = data[stringTableEntry];

    	// Need to look inside the list
	    for (SymbolTableList::iterator listItem = list.begin(); listItem != list.end(); listItem++){
			if (listItem->GetScopeLevel() == currentScopeLevel){
				return true;
			}
	    }

	    return false;
	}

	void EnterScope(){
		IncreaseScopeLevel();
	}

	void ExitScope(){
		ClearScope(currentScopeLevel);
		DecreaseScopeLevel();
	}

	list<StringTableEntry *> GetEntriesInCurrentScopeLevel(){
		list<StringTableEntry *> result;

		SymbolTableMap::const_iterator end = data.end();
	    for (SymbolTableMap::const_iterator mapItem = data.begin(); mapItem != end; ++mapItem)
	    {
	    	StringTableEntry *stringTableEntry = mapItem->first;
	    	SymbolTableList list = mapItem->second; //second = value

	    	// Need to look inside the list
		    for (SymbolTableList::iterator listItem = list.begin(); listItem != list.end(); listItem++){
		    	if (listItem->GetScopeLevel() == currentScopeLevel){
		    		result.push_back(stringTableEntry);
		    	}
		    }
	    }

		return result;
	}

	void dump(std::ostream& os = std::cout){
		os << "Begin dump of symbol table... " << endl;
		os << "=======" << endl;

		SymbolTableMap::const_iterator end = data.end();
	    for (SymbolTableMap::const_iterator mapItem = data.begin(); mapItem != end; ++mapItem)
	    {
	    	StringTableEntry *stringTableEntry = mapItem->first;
	    	os << *stringTableEntry->GetName() << ":" << endl;

	    	SymbolTableList list = mapItem->second; //second = value

	    	// Need to look inside the list
		    for (SymbolTableList::iterator listItem = list.begin(); listItem != list.end(); listItem++){
		    	os << "  ";
				listItem->print(os);
				os << endl;
		    }
	    }
	    os << "=======" << endl;
	}

};

#endif /* SYMBOLTABLE_HH_ */
