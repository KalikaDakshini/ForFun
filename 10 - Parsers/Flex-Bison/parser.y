%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.8"
%defines

%define api.token.constructor
%define api.value.type variant
%define api.parser.class { Parser }
%define api.namespace { Kalika }

%define parse.assert
%define parse.trace
%define parse.error verbose

%locations

%code requires
{
    /* Forward declaration of classes needed */
	namespace Kalika {
        struct Scanner;
        struct Driver;
    }
}

/*
	Bison calls yylex(). Define it in here and place it at
	the top of generated .cc file.
*/
%code top
{
    #include "scanner.hpp"
    #include "driver.hpp"

    #include "parser.hh"
    #include "location.hh"
    
    // yylex() arguments are defined in parser.y
    static Kalika::Parser::symbol_type yylex(Kalika::Scanner &scn) {
        return scn.get_next_token();
    }
}

/* Parameters to pass to Scanner and Parser classes */
%lex-param { Kalika::Scanner &scn }
%parse-param { Kalika::Scanner &scn }
%parse-param { Kalika::Driver &drv }

%token END 0 "end of file"
%token ADD MUL EOL
%token <int> NUM

%type <int> exp

%left ADD
%left MUL

%start program

%%
program:  EOL
		| exp EOL	{ std::cout << "Result: " << $1 << '\n'; }
		;

exp: exp ADD exp	{ $$ = $1 + $3; }
	|exp MUL exp	{ $$ = $1 * $3; }
	|NUM
	;

%%

// Implementation for throwing error
void Kalika::Parser::error(
	const location &loc , const std::string &message
)
{
	std::cout << "Error: " << message << "\nLocation: " << loc << '\n';
}
