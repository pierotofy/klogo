/*
 * Value.h
 *
 *  Created on: Apr 21, 2011
 *      Author: toffa006
 */

#ifndef VALUE_HH_
#define VALUE_HH_

#include <iostream>
#include <string>
#include <assert.h>
#include <list>
#include "RuntimeException.hh"

using namespace std;

class AST_NewCommand;

typedef enum { TypeVoid, TypeNumber, TypeString, TypeBool, TypeList, TypeGeneric } ExpressionType;



class Value {
public:
	Value(){}

	static Value *CreateNew(ExpressionType type);
	virtual void print(std::ostream& os = std::cout){}
	virtual void read(std::istream& is = std::cin){}

		// returns 0 if equal, -1 if object is less than other, 1 if object is more than other
	virtual bool equals(Value *other){}
	virtual bool initialized(){ return false; }
	virtual ExpressionType getType(){ return TypeVoid; }
	virtual Value *clone() { }
};

class UnknownValue : Value {
public:
	UnknownValue(){ }

	void print(std::ostream& os = std::cout){ os << "?"; }
	void read(std::istream& is = std::cin){ assert(false); }
	bool equals(Value *other){ throw RuntimeException("Comparing undefined value."); }
	bool initialized(){ return false; }
	ExpressionType getType(){ return TypeVoid; }
	Value *clone() { return (Value *)new UnknownValue(); }
};

class NumberValue : Value{
	double value;
public:
	NumberValue(double value){
		this->value = value;
	}
	NumberValue(){
		this->value = 0;
	}

	double getDouble(){ return value; }
	void setDouble(double value){ this->value = value; }

	void print(std::ostream& os = std::cout){ os << value; }
	void read(std::istream& is);
	bool equals(Value *other){
		if (this->getType() != other->getType()) return false;

		NumberValue *obj = (NumberValue *)other;
		return obj->value == this->value;
	}
	bool initialized(){ return true; }
	ExpressionType getType(){ return TypeNumber; }
	Value *clone() { return (Value *)new NumberValue(value); }
};


class StringValue : Value{
	string value;
public:
	StringValue(const string &str){
		this->value = string(str);
	}
	StringValue(){
		this->value = "";
	}

	string getString(){
		return value;
	}

	void print(std::ostream& os = std::cout){ os << value; }
	void read(std::istream& is);
	bool equals(Value *other){
		if (this->getType() != other->getType()) return false;

		StringValue *obj = (StringValue *)other;
		return obj->value == this->value;
	}
	bool initialized(){ return true; }
	ExpressionType getType(){ return TypeString; }
	Value *clone() { return (Value *)new StringValue(value); }
};


class BoolValue : Value{
	short int value;
public:
	BoolValue(short int value){
		this->value = value;
	}
	BoolValue(){
		this->value = 0;
	}

	BoolValue(bool value){
		if (value) this->value = 1;
		else this->value = 0;
	}

	bool getBool(){
		return value != 0;
	}

	bool isFalse(){
		return value == 0;
	}
	bool isTrue(){
		return !isFalse();
	}

	void print(std::ostream& os = std::cout){ os << (value == 0 ? "False" : "True"); }
	void read(std::istream& is);
	bool equals(Value *other){
		if (this->getType() != other->getType()) return false;

		BoolValue *obj = (BoolValue *)other;
		return obj->value == this->value;
	}
	bool initialized(){ return true; }
	ExpressionType getType(){ return TypeBool; }
	Value *clone() { return (Value *)new BoolValue(value); }
};

class ListValue : Value{
	list<Value *> items;
public:
	ListValue(){
	}

	void addItem(Value *value){
		items.push_back(value);
	}

	void popFront(){
		items.pop_front();
	}

	int size(){
		return items.size();
	}

	Value *getItem(int index){
		if (items.size() <= index) throw RuntimeException("Trying to access an element of the list out of bounds.");
		else{
			// Iterate through list and return item corresponding to index
		    list<Value *>::iterator listItem = items.begin();
		    for (int i = 0; i<index; i++) listItem++;
		    return *listItem;
		}
	}

	void print(std::ostream& os = std::cout){
    	os << "[ ";
	    for (list<Value *>::iterator item = items.begin(); item != items.end(); ++item)
	    {
	    	Value *value = (Value *)(*item);

	    	if (value->getType() == TypeString) os << "\"";
	    	value->print(os);
	    	if (value->getType() == TypeString) os << "\"";

	    	os << " ";
	    }
    	os << "]";
	}
	void read(std::istream& is);
	bool equals(Value *other){
		if (this->getType() != other->getType()) return false;

		ListValue *obj = (ListValue *)other;

		if (obj->size() != this->size()) return false;
		else{
			// Sizes are the same, scan each item
			for (int i = 0; i<this->size(); i++){
				Value *a = this->getItem(i);
				Value *b = obj->getItem(i);
				if (!a->equals(b)) return false;
			}

			// Went through the scan and everything matched
			return true;
		}
	}
	bool initialized(){ return true; }
	ExpressionType getType(){ return TypeList; }
	Value *clone() {
		ListValue *resList = new ListValue();

		// Copy all items
	    for (list<Value *>::iterator item = items.begin(); item != items.end(); ++item)
	    {
			resList->addItem((*item)->clone());
	    }

	    return (Value *)resList;
	}
};

class FunctionPointerValue : Value{
	AST_NewCommand *command;
public:
	FunctionPointerValue(AST_NewCommand *command){
		this->command = command;
	}

	AST_NewCommand* getPointer(){
		return command;
	}

	void print(std::ostream& os = std::cout){ os << "Function Pointer: " << command; }
	void read(std::istream& is = std::cin){ assert(false); }
	bool initialized(){ return true; }
	Value *clone(){ assert(false); }
};

#endif /* VALUE_HH_ */
