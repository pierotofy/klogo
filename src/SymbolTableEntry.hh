/*
 * SymbolTableEntry.hh
 *
 *  Created on: Apr 12, 2011
 *      Author: toffa006
 */

#ifndef SYMBOLTABLEENTRY_HH_
#define SYMBOLTABLEENTRY_HH_

#include "AST.hh"
#include <stack>

class SymbolTableEntry{
	ExpressionType type;
	int scopeLevel;
public:
	SymbolTableEntry(ExpressionType type, int scopeLevel){
		this->type = type;
		this->scopeLevel = scopeLevel;
	}

	int GetScopeLevel(){ return scopeLevel; }
	ExpressionType GetType(){ return type; }

	void print(std::ostream& os = std::cout){
		os << "Type: " << type << " Scope: " << scopeLevel;
	}
};

#endif /* SYMBOLTABLEENTRY_HH_ */
