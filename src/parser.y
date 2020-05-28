%{
#include "parserTreeNode.h"
#include <stdio.h>
#include <iostream>
extern char *yytext;
extern int column;
extern FILE *yyin;
int yylex(void);
void yyerror(char const *s);
Node* root = NULL;
%}

%union{
	class Node* node;
}

%token <node> INT FLOAT
%token <node> SEMI COMMA ASSIGNOP RELOP
%token <node> PLUS MINUS STAR DIV
%token <node> AND OR DOT NOT
%token <node> TYPE
%token <node> LP RP LB RB LC RC
%token <node> STRUCT
%token <node> RETURN
%token <node> IF ELSE WHILE
%token <node> ID

%type <node> Program ExtDefList ExtDef ExtDecList
// %type <Node> Specifier StructSpecifier OptTag Tag
%type <node> VarDec FunDec VarList ParamDec
%type <node> CompSt StmtList Stmt
%type <node> DefList Def DecList Dec
%type <node> Exp Args
%start Program

%%

Program
	: ExtDefList			{root=$1;}
	;

ExtDefList
	: ExtDef ExtDefList		{$$=new Node("ExtDefList",2,$1,$2);}
	|						{$$=new Node("empty", "");}
	;

ExtDef // Denote for a Global Viriable, Struct, Function
	// Global viriable
	: TYPE ExtDecList SEMI		{$$=new Node("ExtDef",2,$1,$2,2);}
	// Function Decleration
	| TYPE FunDec CompSt		{$$=new Node("ExtDef",3,$1,$2,$3);}  
	;

ExtDecList
	: VarDec						{$$=new Node("ExtDecList",1,$1);}
	| VarDec COMMA ExtDecList		{$$=new Node("ExtDecList",2,$1,$3);}

VarDec
	// :ID								{$$=new Node("VarDec",1,$1);}
	:ID								{$$=$1;}
	// array
	// | VarDec LB INT RB				{$$=new Node("VarDec",4,$1,$2,$3,$4);}
	;

FunDec
	:ID LP VarList RP				{$$=new Node("FunDec",2,$1,$3);}
	| ID LP RP						{$$=new Node("FunDec",1,$1);}
	;

VarList
	:ParamDec COMMA VarList			{$$=new Node("VarList",2,$1,$3);}
	| ParamDec						{$$=new Node("VarList",1,$1);}

ParamDec
	:TYPE VarDec				{$$=new Node("ParamDec",2,$1,$2);}
	;


CompSt
	:LC DefList StmtList RC			{$$=new Node("CompSt",2,$2,$3);}
	;

StmtList
	:Stmt StmtList					{$$=new Node("StmtList",2,$1,$2);}
	|								{$$=new Node("empty", "");}
	;

Stmt
	: CompSt						{$$=$1;}
	| IF LP Exp RP Stmt				{$$=new Node("Stmt_if",2,$3,$5);}
	| IF LP Exp RP Stmt ELSE Stmt	{$$=new Node("Stmt_if_else",3,$3,$5,$7);}
	| RETURN Exp SEMI				{$$=new Node("Stmt_return",1,$2);}
	| WHILE LP Exp RP Stmt			{$$=new Node("Stmt_while",2,$3,$5);}
	| Exp SEMI						{$$=new Node("Stmt_exp",1,$1);}
	;

DefList
	:Def DefList					{$$=new Node("DefList",2,$1,$2);}
	|								{$$=new Node("empty", "");}
	;

Def
	:TYPE DecList SEMI			{$$=new Node("Def",2,$1,$2);}
	;

DecList
	:Dec							{$$=new Node("DecList",1,$1);}
	| Dec COMMA DecList				{$$=new Node("DecList",3,$1,$2,$3);}

Dec
	:VarDec							{$$=new Node("Dec",1,$1);}
	| VarDec ASSIGNOP Exp			{$$=new Node("Dec",2,$1,$3);}


Exp
	:Exp ASSIGNOP Exp				{$$=new Node("Ass_Exp",2,$1,$3);}
	| Exp AND Exp					{$$=new Node("Ternary_Exp",3,$1,$2,$3);}
	| Exp OR Exp					{$$=new Node("Ternary_Exp",3,$1,$2,$3);}
	| Exp PLUS Exp					{$$=new Node("Ternary_Exp",3,$1,$2,$3);}
	| Exp MINUS Exp					{$$=new Node("Ternary_Exp",3,$1,$2,$3);}
	| Exp STAR Exp					{$$=new Node("Ternary_Exp",3,$1,$2,$3);}
	| Exp DIV Exp					{$$=new Node("Ternary_Exp",3,$1,$2,$3);}
	| Exp RELOP Exp					{$$=new Node("Ternary_Exp",3,$1,$2,$3);}
	| LP Exp RP						{$$=$2;}
	| MINUS Exp						{$$=new Node("Binary_Exp",2,$1,$2);}
	| NOT Exp						{$$=new Node("Binary_Exp",2,$1,$2);}
	// value from function
	| ID LP Args RP					{$$=new Node("Fun_Arg_Exp",2,$1,$3);}
	| ID LP RP						{$$=new Node("Fun_Exp",1,$1);}
	| ID							{$$=$1;}
	| INT							{$$=$1;}
	| FLOAT							{$$=$1;}
	;

Args
	:Exp COMMA Args					{$$=new Node("Args",2,$1,$3);}
	| Exp							{$$=new Node("Args",1,$1);}
	;

%%
void yyerror(char const *s)
{
	// fflush(stdout);
	//printf("\n%*s\n%*s\n", column, "^", column, s);
	printf("%----------s--------error here!\n",s);
	// hasError = true;
	// errorNum++;
}