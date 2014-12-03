/*
 * Value.cpp
 *
 *  Created on: Apr 21, 2011
 *      Author: toffa006
 */

#include "Value.hh"
#include <iostream>
#include <stack>
#include <sstream>
#include "Driver.hh"

using namespace std;

Value *Value::CreateNew(ExpressionType type){
	switch(type){
	case TypeNumber:
		return (Value *)new NumberValue();
	case TypeString:
		return (Value *)new StringValue();
	case TypeList:
		return (Value *)new ListValue();
	case TypeBool:
		return (Value *)new BoolValue();
	}
}

void NumberValue::read(std::istream& is = std::cin){
	  is >> value;
	  if (is.fail( )) {
	    throw RuntimeException("Not a number");
	  }
}

void StringValue::read(std::istream& is = std::cin){
	getline(is, value, '\n');
}

void BoolValue::read(std::istream& is = std::cin){
	string input;
	is >> input;
	is.ignore();

	Driver::ConvertToCanonical(input);
	if (input == "true") value = 1;
	else if (input == "false") value = 0;
	else throw RuntimeException("Invalid bool value " + input);
}

void ListValue::read(std::istream& is = std::cin){
	string input;
	
	getline(is, input, '\n');

	const char *strList = input.c_str();

	int i = 0; // current location of the string
	int listId = 0; // 1 = current list
	bool listFound = false;
	bool listEnded = false;

	stack<ListValue *> listStack; // Keeps track of which list we are dealing with
	ListValue *currentList;
	stringstream buffer;

	while(i < input.length()){
		if (strList[i] == '['){
			// New list
			listFound = true;
			listId++;

			// Current list?
			if (listId == 1){
				listStack.push(this);
				currentList = listStack.top();
			}
			// Is this a sublist?
			else if (listId > 1){
				listStack.push((ListValue *)Value::CreateNew(TypeList));

				// Add to the parent
				currentList->addItem(listStack.top());
				currentList = listStack.top();
			}
		}else if (strList[i] == ']'){
			// End list
			listEnded = true;
			listId--;

			listStack.pop();
			if (!listStack.empty()) currentList = listStack.top();
		// Numbers start with +,- or digit
		}else if (strList[i] == '+' || strList[i] == '-' ||
				(strList[i] >= '0' && strList[i] <= '9')){

			buffer.str(string("")); // Clear buffer
			while(strList[i] != ' ' && strList[i] != ']' && i < input.length()){
				buffer << strList[i];

				i++;
			}

			// Go back if the char was a ]
			if (strList[i] == ']'){
				i--;
			}

			double number = 0;
			try{
				number = Driver::ConvertToDouble(buffer.str());
			}catch(const ScannerException &e){
				// Simply throw a runtime exception
				throw RuntimeException("Invalid number list element");
			}

			currentList->addItem((Value *)new NumberValue(number));
		}

		// Bool values start with 't', 'T', 'f' or 'F'
		else if (strList[i] == 't' || strList[i] == 'T' ||
				strList[i] == 'f' || strList[i] == 'F'){

			buffer.str(string("")); // Clear buffer
			while(strList[i] != ' ' && strList[i] != ']' && i < input.length()){
				buffer << strList[i];

				i++;
			}

			// Go back if the char was a ]
			if (strList[i] == ']'){
				i--;
			}

			string matchedIdentifier = buffer.str();
			Driver::ConvertToCanonical(matchedIdentifier);
			if (matchedIdentifier == "true") currentList->addItem((Value *)new BoolValue(true));
			else if (matchedIdentifier == "false") currentList->addItem((Value *)new BoolValue(false));
			else throw RuntimeException("Invalid identifier " + matchedIdentifier + " in list");
		}

		// String values start with "
		else if (strList[i] == '"'){

			i++;

			buffer.str(string("")); // Clear buffer
			while((strList[i] != '"' || strList[i-1] == '\\') && i < input.length()){
				buffer << strList[i];

				i++;
			}

			// Valid string terminator?
			if (strList[i] == '"'){
				string matched = "\"" + buffer.str() + "\"";
				matched = Driver::ConvertToString(matched);
				currentList->addItem((Value *)new StringValue(matched));
			}
			else throw RuntimeException("Invalid string in list (are you missing a double quote?)");
		}

		i++;
	}

	if (!listFound || !listEnded || listId != 0){
		cerr << listFound << endl;
		cerr << listEnded << endl;
		cerr << listId << endl;

		items.clear(); //Cleanup before throwing an exception
		throw RuntimeException("Invalid list");
	}

}
