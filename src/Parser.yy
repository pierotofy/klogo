%skeleton "lalr1.cc"                          /*  -*- C++ -*- */
%require "2.4.1"
%defines
%define parser_class_name "Parser"

%code requires {
# include <string>
# include <math.h>
# include "AST.hh"
# include "StringTableEntry.hh"
class Driver;
}

// The parsing context.
%parse-param { Driver& driver }
%lex-param   { Driver& driver }

%locations
%initial-action
{
  // Initialize the initial location.
  @$.begin.filename = @$.end.filename = &driver.fileToParse;
};

%debug
%error-verbose

// Symbols.
%union
{
  AST_Node *astValue;
  double       doubleValue;
  std::string *stringValue;
  StringTableEntry *stringTableEntryPtr;
  int		   intValue;
  ExpressionType		typeValue;
};

%code {
# include "Driver.hh"
bool error_found = false;
}

/* Parentheses */
%token OPENPARENTHESES "("
%token CLOSEPARENTHESES ")"

/* Punctuation */
%token SEMI ";"

/* Variable identifiers */
%token <stringTableEntryPtr> IDENTIFIER "identifier"

/* Variable values */
%token <stringValue> STRING "string"
%token <doubleValue> NUMBER "number"

/* Loops, ifs, etc. */
%token IF "if"
%token IFELSE "ifelse"
%token FOR "for"
%token WHILE "while"

/* Brackets */
%token OPENCURLYBRACKET "{" 
%token CLOSECURLYBRACKET "}"

%token OPENSQUAREBRACKET "["
%token CLOSESQUAREBRACKET "]"

/* Operands */
%token OPEQUAL "=="
%token OPNOTEQUAL "<>"
%token UNARYPLUS "+"
%token UNARYMINUS "-"
%token OPMULTIPLY "*"
%token OPDIVIDE "/"
%token OPLESSTHAN "<"
%token OPGREATERTHAN ">"
%token OPLESSTHANEQUAL "<="
%token OPGREATERTHANEQUAL ">="

/* Variable types */
%token TYPENUMBER "Number"
%token TYPEBOOL "Bool"
%token TYPESTRING "String"
%token TYPELIST "List"
%token TYPEVOID "Void"


/* Boolean values */
%token BOOLTRUE "True"
%token BOOLFALSE "False"

/* Boolean algebra */
%token	BOOLOR  "or"
%token	BOOLAND "and"
%token	BOOLNOT "not"

/* Make command */
%token MAKECOMMAND "make"

/* Return command */
%token RETURNCOMMAND "return"

/* Pen and color Commands */
%token PENUP "penup"
%token PENDOWN "pendown"
%token SETPENSIZE "setpensize"
%token SETPENCOLOR "setpencolor"

/* Moving and turning commands */
%token FORWARD "forward"
%token BACKWARD "backward"
%token LEFTTURN "leftturn"
%token RIGHTTURN "rightturn"

/* Turtle and position commands */
%token SHOWTURTLE "showturtle"
%token HIDETURTLE "hideturtle"
%token POSITION "position"
%token HEADING "heading"
%token SETPOSITION "setposition"
%token SETHEADING "setheading"

/* Math functions */
%token MATHABS "abs"
%token MATHMINUS "minus"
%token MATHROUND "round"
%token MATHFLOOR "floor"
%token MATHCEIL "ceil"
%token MATHCOS "cos"
%token MATHSIN "sin"
%token MATHTAN "tan"
%token MATHARCTAN "arctan"
%token MATHEXP "exp"
%token MATHLN "ln"
%token MATHRANDOM "random"

/* Compare functions */
%token ISNUMBER "number?"
%token ISSTRING "string?"
%token ISBOOLEAN "boolean?"
%token ISLIST "list?"

/* List/string functions */
%token LISTSTRINGFIRST "first"
%token LISTSTRINGREST "rest"
%token LISTSTRINGNTH "nth"
%token ISLISTSTRINGEMPTY "empty?"
%token ISLISTSTRINGEQUAL "equal?"

/* System functions */
%token SYSPRINT "print"
%token SYSWAIT "wait"
%token SYSREAD "read"

%token END 0 "end of file" 

/* Parser Types */
%type  <astValue>  unit
%type  <typeValue> varType
%type  <astValue>  vardeclaration
%type  <astValue>  returnCommand
%type  <astValue>  statement
%type  <astValue>  statements
%type  <astValue>  assignment
%type  <astValue>  expression
%type  <astValue>  id
%type  <astValue>  stringValue
%type  <astValue>  numberValue
%type  <astValue>  boolValue
%type  <astValue>  listValue
%type  <astValue>  listElement
%type  <astValue>  listElements
%type  <astValue>  codeBlock
%type  <astValue>  ifStatement
%type  <astValue>  ifElseStatement
%type  <astValue>  whileStatement
%type  <astValue>  forStatement
%type  <astValue>  stepExpression
%type  <astValue>  argument
%type  <astValue>  arguments
%type  <astValue>  functionCall
%type  <astValue>  sysFunctionCall
%type  <astValue>  functionCallStatement
%type  <intValue>  sysReservedFunctionToken
%type  <astValue>  newCommand
%type  <astValue>  parameter
%type  <astValue>  parameters


/* Precedence */
%left BOOLNOT BOOLAND BOOLOR

%left UNARYPLUS UNARYMINUS;
%left OPMULTIPLY OPDIVIDE;

%left OPEQUAL OPNOTEQUAL OPLESSTHAN OPGREATERTHAN OPLESSTHANEQUAL OPGREATERTHANEQUAL;




%printer    { debug_stream () << ($$)->GetName(); } "identifier"
%destructor { delete $$; } "identifier"

%printer    { debug_stream () << $$; } <doubleValue>

%%
unit: statements { 
			if (error_found){
            	driver.result = NULL;
            }else{
            	driver.result = (AST_Node *)(new AST_Statements($1)); 
            }                          
		}

statements: statements statement {
               if (error_found) {
                 $$ = NULL;
               }
               else {
                 if ($1 != 0) {
                   AST_Statement *current = (AST_Statement *) $1;
                   while (current->next != 0)
                     current = (AST_Statement *) current->next;
                   current->next = ((AST_Statement *) $2);
                 }
                 else {
                   $$ = $2;
                 }
               }
             }
           | /* Nothing.  */        { $$ = (AST_Node *) 0; 
};
		
statement: assignment { $$ = (AST_Node *) (new AST_Statement($1, driver.GetLineNumber())); }
		|  vardeclaration { $$ = (AST_Node *) (new AST_Statement($1, driver.GetLineNumber())); }
		|  ifStatement { $$ = (AST_Node *) (new AST_Statement($1, driver.GetLineNumber())); }
		|  ifElseStatement { $$ = (AST_Node *) (new AST_Statement($1, driver.GetLineNumber())); }
		|  whileStatement { $$ = (AST_Node *) (new AST_Statement($1, driver.GetLineNumber())); }
		|  forStatement { $$ = (AST_Node *) (new AST_Statement($1, driver.GetLineNumber())); }
		|  functionCallStatement { $$ = (AST_Node *) (new AST_Statement($1, driver.GetLineNumber())); }
		|  newCommand { $$ = (AST_Node *) (new AST_Statement($1, driver.GetLineNumber())); }
		|  returnCommand { $$ = (AST_Node *) (new AST_Statement($1, driver.GetLineNumber())); }

vardeclaration:	varType id SEMI { $$ = (AST_Node *) (new AST_VariableDeclaration($1,$2));  }

varType: TYPENUMBER { $$ = TypeNumber; }
	|	  TYPEBOOL { $$ = TypeBool; }
	|     TYPELIST { $$ = TypeList; }
	|     TYPESTRING { $$ = TypeString; }
	|     TYPEVOID { $$ = TypeVoid; }

assignment:
     MAKECOMMAND id expression SEMI { $$ = (AST_Node *) (new AST_Assignment($2,$3)); }
   | MAKECOMMAND id error SEMI { yyerrok; $$ = NULL; }
   | error id SEMI { yyerrok; $$ = NULL; }
   | error SEMI { yyerrok; $$ = NULL; }
   
id: IDENTIFIER { $$ = (AST_Node *) (new AST_Identifier($1)); };
   
expression: expression UNARYPLUS expression   { $$ = (AST_Node *) (new AST_BinaryOp(token::UNARYPLUS,$1,$3)); }
   | expression UNARYMINUS expression  { $$ = (AST_Node *) (new AST_BinaryOp(token::UNARYMINUS,$1,$3)); }
   | expression OPMULTIPLY expression  { $$ = (AST_Node *) (new AST_BinaryOp(token::OPMULTIPLY,$1,$3)); }
   | expression OPDIVIDE expression { $$ = (AST_Node *) (new AST_BinaryOp(token::OPDIVIDE,$1,$3)); }
   
   | expression OPLESSTHAN expression { $$ = (AST_Node *) (new AST_BinaryOp(token::OPLESSTHAN,$1,$3)); }
   | expression OPGREATERTHAN expression { $$ = (AST_Node *) (new AST_BinaryOp(token::OPGREATERTHAN,$1,$3)); }
   | expression OPLESSTHANEQUAL expression { $$ = (AST_Node *) (new AST_BinaryOp(token::OPLESSTHANEQUAL,$1,$3)); }
   | expression OPGREATERTHANEQUAL expression { $$ = (AST_Node *) (new AST_BinaryOp(token::OPGREATERTHANEQUAL,$1,$3)); }
   
   | expression BOOLAND expression { $$ = (AST_Node *) (new AST_BinaryOp(token::BOOLAND,$1,$3)); } 
   | expression BOOLOR expression { $$ = (AST_Node *) (new AST_BinaryOp(token::BOOLOR,$1,$3)); } 
   | BOOLNOT expression { $$ = (AST_Node *) (new AST_NegationOp($2)); } 
     
   | expression OPEQUAL expression { $$ = (AST_Node *) (new AST_BinaryOp(token::OPEQUAL,$1,$3)); }
   | expression OPNOTEQUAL expression { $$ = (AST_Node *) (new AST_BinaryOp(token::OPNOTEQUAL,$1,$3)); }
     
   | OPENPARENTHESES expression CLOSEPARENTHESES { $$ = $2; }
   | OPENPARENTHESES error CLOSEPARENTHESES { yyerrok; $$ = NULL; }
   | sysFunctionCall 		{ $$ = $1; }
   | functionCall			{ $$ = $1; }
   | numberValue            { $$ = $1; }
   | stringValue 			{ $$ = $1; }
   | boolValue 				{ $$ = $1; }
   | listValue				{ $$ = $1; }
   | id        { $$ = $1; }
   
numberValue: NUMBER { $$ = (AST_Node *) (new AST_Number($1)); }
	  | UNARYMINUS NUMBER { $$ = (AST_Node *) (new AST_Number(-$2)); };
	  | UNARYPLUS NUMBER { $$ = (AST_Node *) (new AST_Number($2)); };
	 
	 
stringValue: STRING { $$ = (AST_Node *) (new AST_String(*$1)); };

boolValue: BOOLTRUE { $$ = (AST_Node *) (new AST_Bool(1)); }
	|      BOOLFALSE{ $$ = (AST_Node *) (new AST_Bool(0)); };
	
listValue: OPENSQUAREBRACKET listElements CLOSESQUAREBRACKET { $$ = (AST_Node *)(new AST_List((AST_Node *)$2)); }

listElements: listElements listElement  {      
			   if (error_found) {
                 $$ = NULL;
               }
               else {
                 if ($1 != 0) {
                   AST_ListElement *current = (AST_ListElement *) $1;
                   while (current->next != 0)
                     current = (AST_ListElement *) current->next;
                   current->next = ((AST_ListElement *) $2);
                 }
                 else {
                   $$ = $2;
                 }
               }
             }
           | /* Nothing.  */        { $$ = (AST_Node *) 0; }

listElement: numberValue { $$ = (AST_Node *) (new AST_ListElement($1)); }
		|    boolValue  { $$ = (AST_Node *) (new AST_ListElement($1));  }
		|    listValue { $$ = (AST_Node *) (new AST_ListElement($1));  }
		|    stringValue { $$ = (AST_Node *) (new AST_ListElement($1));  }
		|    id			{ $$ = (AST_Node *) (new AST_ListElement($1)); }
		
arguments: arguments argument {      
               if (error_found) {
                 $$ = NULL;
               }
               else {
                 if ($1 != 0) {
                   AST_Argument *current = (AST_Argument *) $1;
                   while (current->next != 0)
                     current = (AST_Argument *) current->next;
                   current->next = ((AST_Argument *) $2);
                 }
                 else {
                   $$ = $2;
                 }
               }
             }
           | /* Nothing.  */        { $$ = (AST_Node *) 0; }

argument: expression { $$ = (AST_Node *)(new AST_Argument($1)); }

functionCallStatement: sysFunctionCall SEMI { $$ = $1; }
	  | 	  		   functionCall SEMI { $$ = $1; };

functionCall: id OPENPARENTHESES arguments CLOSEPARENTHESES { $$ = (AST_Node *) (new AST_FunctionCall($1, $3)); };

sysFunctionCall: sysReservedFunctionToken OPENPARENTHESES CLOSEPARENTHESES { $$ = (AST_Node *) (new AST_SysFunctionCall($1)); };
	  |          SYSREAD OPENPARENTHESES varType CLOSEPARENTHESES { $$ = (AST_Node *)(new AST_SysFunctionCall(token::SYSREAD, $3)); }
	  |          sysReservedFunctionToken OPENPARENTHESES expression CLOSEPARENTHESES { $$ = (AST_Node *) (new AST_SysFunctionCall($1, $3)); };
	  |          sysReservedFunctionToken OPENPARENTHESES expression expression CLOSEPARENTHESES { $$ = (AST_Node *) (new AST_SysFunctionCall($1, $3, $4)); };
	  
sysReservedFunctionToken: MATHABS { $$ = token::MATHABS; }
	  | 				  MATHMINUS { $$ = token::MATHMINUS; }
	  | 				  MATHROUND { $$ = token::MATHROUND; }
	  | 				  MATHFLOOR { $$ = token::MATHFLOOR; }
	  | 				  MATHCEIL { $$ = token::MATHCEIL; }
	  | 				  MATHCOS { $$ = token::MATHCOS; }
	  | 				  MATHSIN { $$ = token::MATHSIN; }
	  | 				  MATHTAN { $$ = token::MATHTAN; }
	  | 				  MATHARCTAN { $$ = token::MATHARCTAN; }
	  | 				  MATHEXP { $$ = token::MATHEXP; }
	  | 				  MATHLN { $$ = token::MATHLN; }
	  | 				  MATHRANDOM { $$ = token::MATHRANDOM; }
	  | 				  ISNUMBER { $$ = token::ISNUMBER; }
	  | 				  ISSTRING { $$ = token::ISSTRING; }
	  | 				  ISBOOLEAN { $$ = token::ISBOOLEAN; }
	  | 				  ISLIST { $$ = token::ISLIST; }
	  | 				  LISTSTRINGFIRST { $$ = token::LISTSTRINGFIRST; }
	  | 				  LISTSTRINGREST { $$ = token::LISTSTRINGREST; }
	  | 				  LISTSTRINGNTH { $$ = token::LISTSTRINGNTH; }
	  | 				  ISLISTSTRINGEMPTY { $$ = token::ISLISTSTRINGEMPTY; }
	  | 				  ISLISTSTRINGEQUAL { $$ = token::ISLISTSTRINGEQUAL; }
	  | 				  SYSPRINT { $$ = token::SYSPRINT; }
	  | 				  SYSWAIT { $$ = token::SYSWAIT; }
	  | 				  PENUP { $$ = token::PENUP; }
	  | 				  PENDOWN { $$ = token::PENDOWN; }
	  | 				  SETPENSIZE { $$ = token::SETPENSIZE; }
	  | 				  SETPENCOLOR { $$ = token::SETPENCOLOR; }
	  | 				  FORWARD { $$ = token::FORWARD; }
	  | 				  BACKWARD { $$ = token::BACKWARD; }
	  | 				  LEFTTURN { $$ = token::LEFTTURN; }
	  | 				  RIGHTTURN { $$ = token::RIGHTTURN; }
	  | 				  SHOWTURTLE { $$ = token::SHOWTURTLE; }
	  | 				  HIDETURTLE { $$ = token::HIDETURTLE; }
	  | 				  POSITION { $$ = token::POSITION; }
	  | 				  HEADING { $$ = token::HEADING; }
	  | 				  SETPOSITION { $$ = token::SETPOSITION; }
	  | 				  SETHEADING { $$ = token::SETHEADING; }
	  	  	  
	  
codeBlock: OPENCURLYBRACKET statements CLOSECURLYBRACKET { $$ = (AST_Node *) (new AST_CodeBlock($2)); };

ifStatement: IF expression codeBlock { $$ = (AST_Node *) (new AST_IfStatement($3, $2)); };

ifElseStatement: IFELSE expression codeBlock codeBlock { $$ = (AST_Node *) (new AST_IfElseStatement($3, $4, $2)); };
	  |          IFELSE expression codeBlock error { yyerrok; $$ = NULL; }
	  
whileStatement:  WHILE expression codeBlock { $$ = (AST_Node *) (new AST_WhileStatement($3, $2)); };

forStatement:  FOR OPENCURLYBRACKET id expression expression stepExpression CLOSECURLYBRACKET codeBlock { $$ = (AST_Node *) (new AST_ForStatement($3, $4, $5, $6, $8)); };

stepExpression: expression { $$ = $1; }
		| /* Nothing */ { $$ = (AST_Node *)0; }
		
newCommand: varType id OPENPARENTHESES parameters CLOSEPARENTHESES codeBlock { $$ = (AST_Node *) (new AST_NewCommand($1, $2, $4, $6)); };

returnCommand: RETURNCOMMAND expression SEMI { $$ = (AST_Node *) (new AST_ReturnCommand($2)); }
	| 		   RETURNCOMMAND SEMI { $$ = (AST_Node *) (new AST_ReturnCommand(0)); }

parameters: parameters parameter {      
			   if (error_found) {
                 $$ = NULL;
               }
               else {
                 if ($1 != 0) {
                   AST_Parameter *current = (AST_Parameter *) $1;
                   while (current->next != 0)
                     current = (AST_Parameter *) current->next;
                   current->next = ((AST_Parameter *) $2);
                 }
                 else {
                   $$ = $2;
                 }
               }
             }
           | /* Nothing.  */        { $$ = (AST_Node *) 0; }

parameter: varType id { $$ = (AST_Node *)(new AST_Parameter(new AST_VariableDeclaration($1, $2))); }


%%

void
yy::Parser::error (const yy::Parser::location_type& l,
                          const std::string& m)
{
  error_found = true;
  driver.error (l, m);
}
