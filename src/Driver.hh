#ifndef DRIVER_HH
#define DRIVER_HH

#include <string>
#include <map>
#include "StringTable.hh"
#include "SymbolTable.hh"
#include "ScannerException.hh"
#include "Parser.hh"
#include "AST.hh"

// Tell Flex the lexer's prototype ...
# define YY_DECL                                        \
  yy::Parser::token_type yylex (yy::Parser::semantic_type* yylval,      \
             yy::Parser::location_type* yylloc,      \
             Driver& driver)
// ... and declare it for the parser's sake.
YY_DECL;

// Conducting the whole scanning and parsing of Calc++.
class Driver
{
private:
	bool parseErrorFound;

	/* Initialize the string table with all the languages reserved words */
	void InitializeStringTable();

	/* Helper function to replace a string */
	static string& StrReplace(const string &search, const string &replace, string &subject);

	/* Display token information */
	void DisplayTokenInfo(const string &lexeme, int token, int lineNumber, int charNumber);

	int nextUserDefinedToken;
public:
	Driver ();
	virtual ~Driver ();

	StringTable stringTable; // This holds our string table object

	static SymbolTable symbolTable; // This is our symbol table

	/* Convert a string to a number */
	static double ConvertToDouble(const string &number) throw (ScannerException);

	/* Convert a string lexeme into an effective string */
	static string ConvertToString(const string &lexeme);

	/* Takes a lexeme and converts it to a canonical form */
	static void ConvertToCanonical(string &lexeme);

	/* Increments the value of the next available token number */
	void IncrementUserDefinedTokens();

	/* Returns the next available token number */
	int GetNextAvailableUserDefinedToken();

	/* Parse error found? */
	bool ParseErrorFound(){ return parseErrorFound; }

	/* Returns the current line number processed by flex */
	int GetLineNumber();

	AST_Node *result;

	// Handling the scanner.
	void ScanBegin ();
	void ScanEnd ();
	bool trace_scanning;

	// Run the parser.  Return 0 on success.
	int Parse (const std::string& file);
	std::string fileToParse;
	bool trace_parsing;

	// Error handling.
	void error (const yy::location& l, const std::string& m);
	void error (const std::string& m);


	/* If inputFile is empty, the function will operate in interactive mode (reading from the std input) */
	//void ParseTokens(const string &inputFile);
};

#endif
