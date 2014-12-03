%{                                           
# include <cstdlib>
# include <errno.h>
# include <limits.h>
# include <string>
# include "Driver.hh"
# include "Parser.hh"

/* Work around an incompatibility in flex (at least versions
   2.5.31 through 2.5.33): it generates code that does
   not conform to C89.  See Debian bug 333231
   <http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=333231>.  */
# undef yywrap
# define yywrap() 1

/* By default yylex returns int, we use token::type.
   Unfortunately yyterminate by default returns 0, which is
   not of token::type.  */
#define yyterminate() return token::END;
%}

%{
# define YY_USER_ACTION  yylloc->columns (yyleng);
%}

%{
  typedef yy::Parser::token token;
%}

%option noyywrap nounput batch debug yylineno

/* Basic classes */
DIGIT [0-9]
ALPHA [A-Za-z]
UNSIGNED_REAL_NUMBER {DIGIT}+(([\.])?{DIGIT}+)?
SIGNED_REAL_NUMBER ([+-]){UNSIGNED_REAL_NUMBER}
BLANK [ \t]

IDENTIFIER {ALPHA}({ALPHA}|{DIGIT}|_)*([\?])?
STRING "\""([^\"\\]|\\.)*"\""
NUMBER {UNSIGNED_REAL_NUMBER}(("E"|"e"){SIGNED_REAL_NUMBER})?


%%

"%".*\n				    { yylloc->lines (yyleng); yylloc->step (); }

"("						{ return token::OPENPARENTHESES; }
")"						{ return token::CLOSEPARENTHESES; }
";"						{ return token::SEMI; }
"{"						{ return token::OPENCURLYBRACKET; }
"}"						{ return token::CLOSECURLYBRACKET; }
"["						{ return token::OPENSQUAREBRACKET; }
"]"						{ return token::CLOSESQUAREBRACKET; }

{NUMBER}				{ 
	double number = 0;
	try{
	  number = driver.ConvertToDouble(yytext);
	
	  yylval->doubleValue = number;
	}catch(const ScannerException &e){
	  driver.error(*yylloc, e.what());
	}

	return token::NUMBER; 
}

{STRING} 				{
	string str = driver.ConvertToString(yytext);
	yylval->stringValue = new std::string(str);
	return token::STRING; 
}

{IDENTIFIER}			{ 
  string lexeme = string(yytext);
  driver.ConvertToCanonical(lexeme);

  StringTableEntry *entry;

  if (driver.stringTable.IsEntryInTable(lexeme)){
	  entry = driver.stringTable.GetEntry(lexeme);
  }else{
	  driver.stringTable.Add(lexeme, token::IDENTIFIER);

	  entry = driver.stringTable.GetEntry(lexeme);
  }

  yylval->stringTableEntryPtr = entry;
  return (yy::Parser::token::yytokentype)entry->GetToken(); 
}

"=="					{ return token::OPEQUAL; }
"<>"					{ return token::OPNOTEQUAL; }
"+"						{ return token::UNARYPLUS; }
"-"						{ return token::UNARYMINUS; }
"*"						{ return token::OPMULTIPLY; }
"/"						{ return token::OPDIVIDE; }
"<"						{ return token::OPLESSTHAN; }
">"						{ return token::OPGREATERTHAN; }
"<="					{ return token::OPLESSTHANEQUAL; }
">="					{ return token::OPGREATERTHANEQUAL; }


{BLANK}+   yylloc->step ();
[\n]+      { yylloc->lines (yyleng); yylloc->step (); }
.          driver.error (*yylloc, "invalid character");

%%

void Driver::ScanBegin ()
{
  yy_flex_debug = trace_scanning;
  if (fileToParse == "-i") // Interactive mode
    yyin = stdin;
  else if (!(yyin = fopen (fileToParse.c_str (), "r")))
    {
      error (std::string ("cannot open ") + fileToParse);
      exit (1);
    }
}

int Driver::GetLineNumber(){
	return yylineno;
}

void Driver::ScanEnd ()
{
  fclose (yyin);
}
