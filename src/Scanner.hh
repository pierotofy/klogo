/*
 * Scanner.h
 *
 *  Created on: Feb 3, 2011
 *      Author: toffa006
 */

#ifndef SCANNER_H_
#define SCANNER_H_

#include <string>
#include "StringTable.h"
#include "ScannerException.h"

using namespace std;

class Scanner{
	StringTable *stringTable; // This holds our string table object

	/* Initialize the string table with all the languages reserved words */
	void InitializeStringTable();

	/* Helper function to replace a string */
	string& StrReplace(const string &search, const string &replace, string &subject);

	/* Takes a lexeme and converts it to a canonical form */
	void ConvertToCanonical(string &lexeme);

	/* Display token information */
	void DisplayTokenInfo(const string &lexeme, int token, int lineNumber, int charNumber);

	/* Convert a string to a number */
	double ConvertToDouble(const string &number) throw (ScannerException);

	/* Convert a string lexeme into an effective string */
	string ConvertToString(const string &lexeme);
public:
	/* Constructor */
	Scanner();

	/* If inputFile is empty, the function will operate in interactive mode (reading from the std input) */
	void ParseTokens(const string &inputFile);
};


#endif /* SCANNER_H_ */
