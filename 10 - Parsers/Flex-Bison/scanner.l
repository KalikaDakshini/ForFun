%{
	#include <iostream>
	#include "scanner.hpp"
	#include "driver.hpp"
	
	#include "parser.hh"
	#include "location.hh"

	// Original yyterminate() macro returns int. Since we're using Bison 3 variants
	// as tokens, we must redefine it to change type from `int` to `Parser::semantic_type`
	#define yyterminate() Kalika::Parser::make_END(Kalika::location());

	// This will track current scanner location.
	// Action is called when length of the token is known.
	#define YY_USER_ACTION m_driver.increase_loc(yyleng);
	
	// !!!WARNING!!!
	// Location API is used, but the location is not initialized, 'cause I'm lazy. When making
	// a token with make_{something} method you can pass detailed token location. Current location
	// is accessible with m_driver.location() method. All puzzle elements are there - just
	// pass location value in every action code block below. I'm going to waste more time writing
	// this excuse than putting this boilerplate below...
	//
	// Location class can be found in location.hh and posistion.hh files. It's just a bit too much
	// boilerplate for this small example. Bummer.
%}

%option nodefault
%option noyywrap
%option c++
%option yyclass="Scanner"
%option prefix="Kalika"

%%
[1-9][0-9]* 	{
					return Parser::make_NUM(
						std::stoi(yytext), Kalika::location()
					);
				}

"+"				{ return Parser::make_ADD(Kalika::location()); }
"*"				{ return Parser::make_MUL(Kalika::location()); }
\n				{ return Parser::make_EOL(Kalika::location()); }

[ \t]			{}
.				{}
            
<<EOF>>     { return yyterminate(); }


%%
