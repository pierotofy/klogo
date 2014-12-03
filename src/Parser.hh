/* A Bison parser, made by GNU Bison 2.4.2.  */

/* Skeleton interface for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002-2010 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C++ LALR(1) parser skeleton written by Akim Demaille.  */

#ifndef PARSER_HEADER_H
# define PARSER_HEADER_H

/* "%code requires" blocks.  */

/* Line 34 of lalr1.cc  */
#line 6 "Parser.yy"

# include <string>
# include <math.h>
# include "AST.hh"
# include "StringTableEntry.hh"
class Driver;



/* Line 34 of lalr1.cc  */
#line 53 "Parser.hh"


#include <string>
#include <iostream>
#include "stack.hh"


namespace yy {

/* Line 34 of lalr1.cc  */
#line 64 "Parser.hh"
  class position;
  class location;

} // yy

/* Line 34 of lalr1.cc  */
#line 71 "Parser.hh"

#include "location.hh"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)		\
do {							\
  if (N)						\
    {							\
      (Current).begin = (Rhs)[1].begin;			\
      (Current).end   = (Rhs)[N].end;			\
    }							\
  else							\
    {							\
      (Current).begin = (Current).end = (Rhs)[0].end;	\
    }							\
} while (false)
#endif


namespace yy {

/* Line 34 of lalr1.cc  */
#line 116 "Parser.hh"

  /// A Bison parser.
  class Parser
  {
  public:
    /// Symbol semantic values.
#ifndef YYSTYPE
    union semantic_type
    {

/* Line 34 of lalr1.cc  */
#line 30 "Parser.yy"

  AST_Node *astValue;
  double       doubleValue;
  std::string *stringValue;
  StringTableEntry *stringTableEntryPtr;
  int		   intValue;
  ExpressionType		typeValue;



/* Line 34 of lalr1.cc  */
#line 140 "Parser.hh"
    };
#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef location location_type;
    /// Tokens.
    struct token
    {
      /* Tokens.  */
   enum yytokentype {
     END = 0,
     OPENPARENTHESES = 258,
     CLOSEPARENTHESES = 259,
     SEMI = 260,
     IDENTIFIER = 261,
     STRING = 262,
     NUMBER = 263,
     IF = 264,
     IFELSE = 265,
     FOR = 266,
     WHILE = 267,
     OPENCURLYBRACKET = 268,
     CLOSECURLYBRACKET = 269,
     OPENSQUAREBRACKET = 270,
     CLOSESQUAREBRACKET = 271,
     OPEQUAL = 272,
     OPNOTEQUAL = 273,
     UNARYPLUS = 274,
     UNARYMINUS = 275,
     OPMULTIPLY = 276,
     OPDIVIDE = 277,
     OPLESSTHAN = 278,
     OPGREATERTHAN = 279,
     OPLESSTHANEQUAL = 280,
     OPGREATERTHANEQUAL = 281,
     TYPENUMBER = 282,
     TYPEBOOL = 283,
     TYPESTRING = 284,
     TYPELIST = 285,
     TYPEVOID = 286,
     BOOLTRUE = 287,
     BOOLFALSE = 288,
     BOOLOR = 289,
     BOOLAND = 290,
     BOOLNOT = 291,
     MAKECOMMAND = 292,
     RETURNCOMMAND = 293,
     PENUP = 294,
     PENDOWN = 295,
     SETPENSIZE = 296,
     SETPENCOLOR = 297,
     FORWARD = 298,
     BACKWARD = 299,
     LEFTTURN = 300,
     RIGHTTURN = 301,
     SHOWTURTLE = 302,
     HIDETURTLE = 303,
     POSITION = 304,
     HEADING = 305,
     SETPOSITION = 306,
     SETHEADING = 307,
     MATHABS = 308,
     MATHMINUS = 309,
     MATHROUND = 310,
     MATHFLOOR = 311,
     MATHCEIL = 312,
     MATHCOS = 313,
     MATHSIN = 314,
     MATHTAN = 315,
     MATHARCTAN = 316,
     MATHEXP = 317,
     MATHLN = 318,
     MATHRANDOM = 319,
     ISNUMBER = 320,
     ISSTRING = 321,
     ISBOOLEAN = 322,
     ISLIST = 323,
     LISTSTRINGFIRST = 324,
     LISTSTRINGREST = 325,
     LISTSTRINGNTH = 326,
     ISLISTSTRINGEMPTY = 327,
     ISLISTSTRINGEQUAL = 328,
     SYSPRINT = 329,
     SYSWAIT = 330,
     SYSREAD = 331
   };

    };
    /// Token type.
    typedef token::yytokentype token_type;

    /// Build a parser object.
    Parser (Driver& driver_yyarg);
    virtual ~Parser ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

  private:
    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Generate an error message.
    /// \param state   the state where the error occurred.
    /// \param tok     the lookahead token.
    virtual std::string yysyntax_error_ (int yystate, int tok);

#if YYDEBUG
    /// \brief Report a symbol value on the debug stream.
    /// \param yytype       The token type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    virtual void yy_symbol_value_print_ (int yytype,
					 const semantic_type* yyvaluep,
					 const location_type* yylocationp);
    /// \brief Report a symbol on the debug stream.
    /// \param yytype       The token type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    virtual void yy_symbol_print_ (int yytype,
				   const semantic_type* yyvaluep,
				   const location_type* yylocationp);
#endif


    /// State numbers.
    typedef int state_type;
    /// State stack type.
    typedef stack<state_type>    state_stack_type;
    /// Semantic value stack type.
    typedef stack<semantic_type> semantic_stack_type;
    /// location stack type.
    typedef stack<location_type> location_stack_type;

    /// The state stack.
    state_stack_type yystate_stack_;
    /// The semantic value stack.
    semantic_stack_type yysemantic_stack_;
    /// The location stack.
    location_stack_type yylocation_stack_;

    /// Internal symbol numbers.
    typedef unsigned char token_number_type;
    /* Tables.  */
    /// For a state, the index in \a yytable_ of its portion.
    static const short int yypact_[];
    static const signed char yypact_ninf_;

    /// For a state, default rule to reduce.
    /// Unless\a  yytable_ specifies something else to do.
    /// Zero means the default is an error.
    static const unsigned char yydefact_[];

    static const signed char yypgoto_[];
    static const short int yydefgoto_[];

    /// What to do in a state.
    /// \a yytable_[yypact_[s]]: what to do in state \a s.
    /// - if positive, shift that token.
    /// - if negative, reduce the rule which number is the opposite.
    /// - if zero, do what YYDEFACT says.
    static const short int yytable_[];
    static const signed char yytable_ninf_;

    static const short int yycheck_[];

    /// For a state, its accessing symbol.
    static const unsigned char yystos_[];

    /// For a rule, its LHS.
    static const unsigned char yyr1_[];
    /// For a rule, its RHS length.
    static const unsigned char yyr2_[];

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#endif

#if YYERROR_VERBOSE
    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    virtual std::string yytnamerr_ (const char *n);
#endif

#if YYDEBUG
    /// A type to store symbol numbers and -1.
    typedef signed char rhs_number_type;
    /// A `-1'-separated list of the rules' RHS.
    static const rhs_number_type yyrhs_[];
    /// For each rule, the index of the first RHS symbol in \a yyrhs_.
    static const unsigned short int yyprhs_[];
    /// For each rule, its source line number.
    static const unsigned short int yyrline_[];
    /// For each scanner token number, its symbol number.
    static const unsigned short int yytoken_number_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    /* Debugging.  */
    int yydebug_;
    std::ostream* yycdebug_;
#endif

    /// Convert a scanner token number \a t to a symbol number.
    token_number_type yytranslate_ (int t);

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg        Why this token is reclaimed.
    /// \param yytype       The symbol type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    inline void yydestruct_ (const char* yymsg,
			     int yytype,
			     semantic_type* yyvaluep,
			     location_type* yylocationp);

    /// Pop \a n symbols the three stacks.
    inline void yypop_ (unsigned int n = 1);

    /* Constants.  */
    static const int yyeof_;
    /* LAST_ -- Last index in TABLE_.  */
    static const int yylast_;
    static const int yynnts_;
    static const int yyempty_;
    static const int yyfinal_;
    static const int yyterror_;
    static const int yyerrcode_;
    static const int yyntokens_;
    static const unsigned int yyuser_token_number_max_;
    static const token_number_type yyundef_token_;

    /* User arguments.  */
    Driver& driver;
  };

} // yy

/* Line 34 of lalr1.cc  */
#line 400 "Parser.hh"



#endif /* ! defined PARSER_HEADER_H */
