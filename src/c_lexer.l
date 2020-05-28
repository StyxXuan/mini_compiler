%{
#include <iostream>
#include "compiler.hpp"
#include "parserTreeNode.h"
#pragma clang diagnostic ignored "-Wdeprecated-register"
void yyerror(char const *s);
using namespace std;
%}

IDENTIFIER               ([_a-zA-Z][_a-zA-Z0-9]*)
ASSIGNMENT_OPERATOR      (([<>][<>]|[*\/%+\-&^|])[=])
INTEGERSUFFIX            ([uU][lL]|[lL][uU]|[uUlL])
DECIMALCONSTANT          ([1-9][0-9]*)
OCTALCONSTANT            ([0][0-7]*)
HEXCONSTANT              ([0][xX][0-9A-Fa-f]+)
WHITESPACE               ([\t\r\n]+)
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


"!"            {return NOT;}
"."		     {return DOT; }


([0-9]+\.[0-9]*)              {yylval.node = new Node("FLOAT", std::string(yytext)); return FLOAT; }

([1-9][0-9]*)|0               {yylval.node = new Node("INT", std::string(yytext)); return INT; }
.                             { fprintf(stderr, "Invalid token\n"); exit(1); }

%%

int yywrap(void)
{
	return 1;
}