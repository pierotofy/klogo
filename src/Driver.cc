/*
 * Driver.cc
 *
 *  Created on: Feb 3, 2011
 *      Author: toffa006
 */

#include "Driver.hh"
#include "Parser.hh"

#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <assert.h>

using namespace std;

// Include the flex lexer library header
#include <FlexLexer.h>

typedef yy::Parser::token token;


Driver::Driver ()
  : trace_scanning (false), trace_parsing (false), parseErrorFound(false)
{
  InitializeStringTable();
}

Driver::~Driver ()
{
}

int Driver::Parse (const std::string &file)
{
  fileToParse = file;
  ScanBegin();
  yy::Parser parser (*this);
  parser.set_debug_level (trace_parsing);
  int res = parser.parse ();
  ScanEnd();
  return res;
}

void Driver::error (const yy::location& l, const std::string& m)
{
  std::cout << l << ": " << m << std::endl;
  parseErrorFound = true;
}

void Driver::error (const std::string& m)
{
  std::cout  << m << std::endl;
  parseErrorFound = true;
}

void Driver::InitializeStringTable(){

	/* KLogo is case insensitive, so we assume all words will be compared
	 * when lower cased
	 */


	stringTable.Add("number",token::TYPENUMBER);
	stringTable.Add("bool",token::TYPEBOOL);
	stringTable.Add("string",token::TYPESTRING);
	stringTable.Add("list",token::TYPELIST);
	stringTable.Add("void",token::TYPEVOID);

	stringTable.Add("true",token::BOOLTRUE);
	stringTable.Add("false",token::BOOLFALSE);

	stringTable.Add("make",token::MAKECOMMAND);
	stringTable.Add("return",token::RETURNCOMMAND);

	stringTable.Add("penup",token::PENUP);
	stringTable.Add("pu",token::PENUP);
	stringTable.Add("pendown",token::PENDOWN);
	stringTable.Add("pd",token::PENDOWN);
	stringTable.Add("setpensize",token::SETPENSIZE);
	stringTable.Add("setpencolor",token::SETPENCOLOR);

	stringTable.Add("forward",token::FORWARD);
	stringTable.Add("fd",token::FORWARD);
	stringTable.Add("backward",token::BACKWARD);
	stringTable.Add("bk",token::BACKWARD);
	stringTable.Add("leftturn",token::LEFTTURN);
	stringTable.Add("lt",token::LEFTTURN);
	stringTable.Add("rightturn",token::RIGHTTURN);
	stringTable.Add("rt",token::RIGHTTURN);

	stringTable.Add("showturtle",token::SHOWTURTLE);
	stringTable.Add("hideturtle",token::HIDETURTLE);
	stringTable.Add("position",token::POSITION);
	stringTable.Add("heading",token::HEADING);
	stringTable.Add("setposition",token::SETPOSITION);
	stringTable.Add("setheading",token::SETHEADING);
	stringTable.Add("abs",token::MATHABS);
	stringTable.Add("minus",token::MATHMINUS);
	stringTable.Add("round",token::MATHROUND);
	stringTable.Add("floor",token::MATHFLOOR);
	stringTable.Add("ceil",token::MATHCEIL);
	stringTable.Add("cos",token::MATHCOS);
	stringTable.Add("sin",token::MATHSIN);
	stringTable.Add("tan",token::MATHTAN);
	stringTable.Add("arctan",token::MATHARCTAN);
	stringTable.Add("exp",token::MATHEXP);
	stringTable.Add("ln",token::MATHLN);
	stringTable.Add("random",token::MATHRANDOM);

	stringTable.Add("number?",token::ISNUMBER);
	stringTable.Add("string?",token::ISSTRING);
	stringTable.Add("boolean?",token::ISBOOLEAN);
	stringTable.Add("list?",token::ISLIST);

	stringTable.Add("first",token::LISTSTRINGFIRST);
	stringTable.Add("rest",token::LISTSTRINGREST);
	stringTable.Add("nth",token::LISTSTRINGNTH);
	stringTable.Add("empty?",token::ISLISTSTRINGEMPTY);
	stringTable.Add("equal?",token::ISLISTSTRINGEQUAL);

	stringTable.Add("print",token::SYSPRINT);
	stringTable.Add("wait",token::SYSWAIT);
	stringTable.Add("read",token::SYSREAD);

	stringTable.Add("if",token::IF);
	stringTable.Add("ifelse",token::IFELSE);
	stringTable.Add("for",token::FOR);
	stringTable.Add("while",token::WHILE);

	stringTable.Add("and",token::BOOLAND);
	stringTable.Add("or",token::BOOLOR);
	stringTable.Add("not",token::BOOLNOT);

}

void Driver::ConvertToCanonical(string &lexeme){
	const int length = lexeme.length();
	for(int i=0; i < length; ++i)
	{
		lexeme[i] = std::tolower(lexeme[i]);
	}
}

void Driver::IncrementUserDefinedTokens(){
	nextUserDefinedToken++;
}

int Driver::GetNextAvailableUserDefinedToken(){
	return nextUserDefinedToken;
}

void Driver::DisplayTokenInfo(const string &lexeme, int token, int lineNumber, int charNumber){
	  cout << "\"" << lexeme << "\" (token #" << token << ") at line " << lineNumber << " character " << charNumber << endl;

}

string& Driver::StrReplace(const string &search, const string &replace, string &subject)
{
    string buffer;

    int sealeng = search.length();
    int strleng = subject.length();

    if (sealeng==0)
        return subject;//no change

    for(int i=0, j=0; i<strleng; j=0 )
    {
        while (i+j<strleng && j<sealeng && subject[i+j]==search[j])
            j++;
        if (j==sealeng)//found 'search'
        {
            buffer.append(replace);
            i+=sealeng;
        }
        else
        {
            buffer.append( &subject[i++], 1);
        }
    }
    subject = buffer;
    return subject;
}

double Driver::ConvertToDouble(const string &number) throw(ScannerException){
	  stringstream ss(number);
	  double d = 0;
	  ss >> d;
	  if (ss.fail( )) {
	    throw ScannerException(number + " could not be stored! (Too big?)");
	  }
	  return (d);
}

string Driver::ConvertToString(const string &lexeme){
	assert(lexeme[0] == '"' && lexeme[lexeme.length()-1] == '"'); // A string should always start and end with double quotes

	string result = string(lexeme);
	result = Driver::StrReplace("\\\\","\\",result); // \\ --> \ conversion
	result = Driver::StrReplace("\\\"","\"",result); // \" --> " conversion
	result = Driver::StrReplace("\\n","\n",result); // \n --> new line conversion


	result = result.substr(1); // Cut out first double quote
	result = result.substr(0,result.length()-1); //Cut out second double quote

	return result;
}



