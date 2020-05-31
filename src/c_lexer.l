%{
#include <iostream>
#include "compiler.hpp"
#include "errorHandler.h"
#include "parserTreeNode.h"
#pragma clang diagnostic ignored "-Wdeprecated-register"
void yyerror(char const *s);
extern string line;
using namespace std;
%}
%option yylineno
IDENTIFIER               ([_a-zA-Z][_a-zA-Z0-9]*)
ASSIGNMENT_OPERATOR      (([<][<]|[>][>]|[*\/%+\-&^\|])[=])
INTEGERSUFFIX            ([uU][lL]|[lL][uU]|[uUlL])
DECIMALCONSTANT 		 ([1-9][0-9]*|[0])
FLOATCONSTANT            ([0-9]+\.[0-9]*)
OCTALCONSTANT            ([0][0-7]*)
HEXCONSTANT              ([0][xX][0-9A-Fa-f]+)
CHARCONSTANT 			 ([']([\\]([abfnrt0?\\'"]|[x][0-9a-fA-F]{2}|[0-7]{3})|[^'\\])['])
STRINGLITERAL 			 (["]([\\]([abfnrt0?\\'"]|[x][0-9a-fA-F]{2}|[0-7]{3})|[^'\\])*["])
WHITESPACE 				 [ \t\r]+
MultiLineAnnotate 		 "/*"("/"*)([^*/]|[^*]"/"|"*"[^/])*("*"*)"*/"
SingleLineAnnotate       "//"(.*)$
ERROR 					 .
NEWLINE					 [\n]
EXCEPT					 [^\n]+
%%
"/*"           {}
"//"[^\n]*     {}

"void"         {yylval.node = new Node("TYPE", "VOID"); return TYPE;}
"int"          {yylval.node = new Node("TYPE", "INT"); return TYPE;}
"float"        {yylval.node = new Node("TYPE", "FLOAT"); return TYPE;}
"if"           {return IF; }
"else"		{return ELSE; }
"return"		{return RETURN; }
"while"		{return WHILE; }
"struct"	{return STRUCT;}
"for"	{return FOR;}
"do"	{return DO;}


";"                 { return SEMI; }
","                 { return COMMA; }
"("                 { return LP; }
")"                 { return RP; }
"{"                 { return LC; }
"}"                 { return RC; }
"["             { return LB; }
"]"             { return RB; }
"="             { return ASSIGNOP; }
" "             {}
{WHITESPACE}    {}

{IDENTIFIER}    {yylval.node = new Node("ID", std::string(yytext)); return ID; }



"||"		{ return OR; }
"&&"		{ return AND; }

"=="		{yylval.node = new Node("RELOP", "=="); return RELOP; }
"!="		{yylval.node = new Node("RELOP", "!="); return RELOP; }
">="        {yylval.node = new Node("RELOP", ">="); return RELOP; }
"<="        {yylval.node = new Node("RELOP", "<="); return RELOP; }
">"            {yylval.node = new Node("RELOP", ">"); return RELOP; }
"<"            {yylval.node = new Node("RELOP", "<"); return RELOP; }


"*"           {yylval.node = new Node("STAR", "*"); return STAR; }
"/"		     {yylval.node = new Node("DIV", "/"); return DIV; }
"+"		     {yylval.node = new Node("PLUS", "+"); return PLUS; }
"-"		     {yylval.node = new Node("MINUS", "-"); return MINUS; }
"%"		     {yylval.node = new Node("MOD", "%"); return MOD; }


"!"            {return NOT;}
"."		     {return DOT; }


{WHITESPACE}   		 { line += string(yytext);}
{IDENTIFIER}  	     { yylval.node = new Node("ID", std::string(yytext)); return ID; }
{MultiLineAnnotate}  {}
{SingleLineAnnotate} {}
{FLOATCONSTANT}      { yylval.node = new Node("FLOAT", std::string(yytext)); return FLOAT; }
{DECIMALCONSTANT}    { yylval.node = new Node("INT", std::string(yytext)); return INT; }
{ERROR}              { fprintf(stderr, "Invalid token\n"); exit(1); }
{NEWLINE}			 { line = "";}
%%

int yywrap(void)
{
	return 1;
}