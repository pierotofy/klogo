/*
 * ActivationRecord.h
 *
 *  Created on: Apr 21, 2011
 *      Author: toffa006
 */

#ifndef ACTIVATIONRECORD_HH_
#define ACTIVATIONRECORD_HH_
#include <list>
#include <assert.h>
#include "StringTableEntry.hh"
#include "ValueStack.hh"

class ActivationRecord {
	typedef list<StringTableEntry *> StringTableEntryList;
	StringTableEntryList entries;
public:
	ActivationRecord(){ }

	void AddReference(StringTableEntry *entry){
		entries.push_back(entry);

		if (entry->value == 0) entry->value = new ValueStack();
	}

	void EnterScope(){
		for (StringTableEntryList::const_iterator item = entries.begin(); item != entries.end(); item++){
			(*item)->value->Allocate();
/*
			cout << "Allocate for ";
	    	cout << *(*item)->GetName() << ": ";
			(*item)->value->print(cout);
	    	cout << endl;
	    	*/

	    }
	}

	void ExitScope(){
		for (StringTableEntryList::const_iterator item = entries.begin(); item != entries.end(); item++){
			(*item)->value->Deallocate();

/*
			cout << "Deallocate for ";
	    	cout << *(*item)->GetName() << ": ";
			(*item)->value->print(cout);
	    	cout << endl;*/

	    }
	}

	void print(std::ostream& os = std::cout){
		for (StringTableEntryList::const_iterator item = entries.begin(); item != entries.end(); item++){
			os << *(*item)->GetName() << ": ";
			(*item)->value->print(os);
	    	os << endl;
	    }
	}

};

#endif /* ACTIVATIONRECORD_H_ */
