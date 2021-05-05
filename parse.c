// Coy Torreblanca
#include "parse.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>
#define MAX_SYMBOL_TABLE_SIZE 1000
// counter for parsing
int pacounter;
int maxcounter;
int scounter;
char *lex_str;
symbol *symbol_table;

// print error
void error(char *msg)
{
	printf("%s", msg);
	exit(0);
}


// print a symbol
void print_sy(symbol x)
{
	printf("\nkind:%d name:%s val:%d level:%d addr:%d mark: %d\n", x.kind, x.name, x.val, x.level, x.addr, x.bit);
}

// print all symbols
void pall_sy()
{
	for(int i = 0; i < scounter; i++)
		print_sy(symbol_table[i]);
}

// add symbol to symbol table
void add_st(int kind, char *name, int val, int level, int addr, int mark)
{
	// no more room
	if (scounter == MAX_SYMBOL_TABLE_SIZE)
		return;
	if (strlen(name) > 11)
	{
		error("Parser error: name too large");
		return;
	}
		
	// add corresponding values
	symbol_table[scounter].kind = kind;
	symbol_table[scounter].val = val;
	symbol_table[scounter].level = level;
	symbol_table[scounter].addr = addr;
	symbol_table[scounter].bit = mark;
	strcpy(symbol_table[scounter++].name,name);
}

// find an unmarked symbol in the symbol table with all the given values 
int find_st_um(char *name, int type)
{
	// loop until the end or until broken
	for(int i = (scounter - 1); i >= 0; i--)
	{
		symbol re = symbol_table[i];
		// check if symbol matches giving
		if (type == -1 && !strcmp(name, re.name) && re.bit == 0)
			return i;
		if (!strcmp(name,re.name) && re.bit == 0 && re.kind == type)
			return i;
	}
	return -1;
}

// find a symbol in the symbol table with all the given name 
int find_st(char *name)
{
	// loop until the end or until broken
	for(int i = (scounter - 1); i >= 0; i--)
	{
		symbol re = symbol_table[i];
		// check if symbol matches givin
		if (!strcmp(name,re.name))
			return i;
	}
	return -1;
}


void mark_symbols(int num, int ll)
{
	for (int i = (scounter - 1); i >= 0; i--)
	{
		if (num == 0)
			break;
		if(symbol_table[i].bit == 0 && symbol_table[i].level == ll)
		{
			symbol_table[i].bit = 1;
			num--;
		}
	}
}

void init(char *scanner_output)
{
	symbol_table = (symbol *)malloc(sizeof(symbol) * MAX_SYMBOL_TABLE_SIZE);
	scounter = 0;
	pacounter = 0;
	lex_str = scanner_output;
	maxcounter = strlen(lex_str);
}

int getT()
{
	int pcounter = pacounter;
	// read token by parsing the next one-two numbers
	// returns -1 if token not found
	switch (lex_str[pcounter++])
	{
		// according to token return appropriate token type
		case '1':
			switch (lex_str[pcounter++])
			{

				case '0':
					return 10;
					break;
				case ' ':
					return 1;
					break;
				case '1':
					return 11;
					break;
				case '2':
					return 12;
					break;
				case '3':
					return 13;
					break;
				case '4':
					return 14;
					break;
				case '5':
					return 15;
					break;
				case '6':
					return 16;
					break;
				case '7':
					return 17;
					break;
				case '8':
					return 18;
					break;
				case '9':
					return 19;
					break;
			}
			break;

		case '2':
			switch (lex_str[pcounter++])
			{
				case ' ':
					return 2;
					break;
				case '0':
					return 20;
					break;
				case '1':
					return 21;
					break;
				case '2':
					return 22;
					break;
				case '3':
					return 23;
					break;
				case '4':
					return 24;
					break;
				case '5':
					return 25;
					break;
				case '6':
					return 26;
					break;
				case '7':
					return 27;
					break;
				case '8':
					return 28;
					break;
				case '9':
					return 29;
					break;
			}
			break;
			
		case '3':
			switch (lex_str[pcounter++])
			{
				case ' ':
					return 3;
					break;
				case '0':
					return 30;
					break;
				case '1':
					return 31;
					break;
				case '2':
					return 32;
					break;
				case '3':
					return 33;
					break;
			}
			break;
		case '4':
			return 4;
			break;
		case '5':
			return 5;
			break;
		case '6':
			return 6;
			break;
		case '7':
			return 7;
			break;
		case '8':
			return 8;
			break;
		case '9':
			return 9;
			break;
		default:
			return -1;
			break;
	}
}

// skip to next token
void nextT()
{
	// eliminate token
	while(pacounter < maxcounter && lex_str[pacounter] != ' ')
		pacounter++;
		
	// eliminate whitespace after
	while(pacounter < maxcounter && lex_str[pacounter] == ' ')
		pacounter++;
}

// get the next token as a string
char *nextS()
{
	// get the length of the next token
	int tmpc = pacounter;
	while(tmpc < maxcounter && lex_str[tmpc] != ' ')
		tmpc++;
	int length = tmpc - pacounter;
	
	// copy next token to memory
	char *re = (char *)malloc(sizeof(char) * (length + 1));
	for (int i = 0; i < length; i++)
		re[i] = lex_str[i+pacounter];
	re[length] = '\0';
	return re;
}

// get next token as an int
int nextI()
{
	char* endptr;
	return strtoimax(nextS(),&endptr,10);
}

void statement(int ll)
{
	int tInt = getT();
	int st_index;
	symbol tmp;
	switch(tInt)
	{

		// if token is ident
		case 2:
			// skip over ident token
			nextT();
			// get ident symbol table index
			st_index = find_st_um(nextS(), 2);
			// if name not found in st
			if (st_index == -1)
				error("Parse Error: 11 Undeclared identifier in ident");
			// get symbol from symbol table
			tmp = symbol_table[st_index];
			// if symbol is not a variable
			if (tmp.kind != 2)
				error("Parse Error: 12 Assignment to constant or procedure not allowed");
			// skip over name
			nextT();
			
			// peek
			tInt = getT();
			// if token is not becomes
			if (tInt != 20)
				error("Parse error: 13 Assignment operator expected");
			// skip becomes symbol
			nextT();
			expression(ll);
		break;
		
		// if token is call
		case 27:
			// skip over call token
			nextT();
			// skip ident token
			if (getT() != 2)
				error("Parse Error: identifier to follow call");
			nextT();
			
			// get ident symbol table index
			st_index = find_st_um(nextS(), 3);
			if (st_index == -1)
				error("Parse Error: 11 Undeclared identifierin call");
				
			// skip name
			nextT();
		break;
			
		// if token is begin
		case 21:
			// skip begin
			nextT();
			statement(ll);
			
			// peek
			tInt = getT();
			// while token equals semicolon
			while (tInt == 18)
			{
				// skip semicolon
				nextT();
				statement(ll);
				tInt = getT();
				// error 17?
			}
			
			// peek
			tInt = getT();
			// if next token doesn't equal end, error
			if (tInt != 22)
				error("Parse error: 17 end missing"); // wrong error
			// skip end
			nextT();
		break;
		
		// if token is if
		case 23:
			// skip if token
			nextT();
			condition(ll);
			
			// peek
			tInt = getT();
			
			// if token doesnt equal then, error
			if (tInt != 24)
				error("Parse Error: 16 if without then");
			
			// skip then
			nextT();
			statement(ll);
			
			// check for else
			if (getT() == 33)
			{
				// skip else
				nextT();
				statement(ll);
			}
			break;
		
		// if token is while
		case 25:
			// skip while
			nextT();
			condition(ll);
			
			// peek
			tInt = getT();
			// if next token does not equal do, error
			if (tInt != 26)
				error("Parse Error: 18 do expected");
			// skip do
			nextT();
			statement(ll);
			break;
		
		// if token is read
		case 32:
			// skip read token
			nextT();
			
			// peek
			tInt = getT();
			// if token is ident
			if (tInt != 2)
				error("Parse Error: read without ident");
			// skip ident token
			nextT();
			
			// get ident symbol table index
			st_index = find_st_um(nextS(), 2);
			// if name not found in st
			if (st_index == -1)
				error("Parse Error: 11 Undeclared identifier in read");
			// get symbol from symbol table
			symbol tmp = symbol_table[st_index];
			// if symbol is not a variable
			if (tmp.kind != 2)
				error("Parse Error: 12 Assignment to constant or procedure not allowed");
			// skip over name
			nextT();
			break;
		
		// if token is write
		case 31:
			// skip write token
			nextT(); 
			expression(ll);
			break;
	}
}
			
			

void condition(int ll)
{
	int tInt = getT();
	// if next token is odd
	if (tInt == 8)
	{
		// skip token
		nextT();
		expression(ll);
	}
	else
	{
		expression(ll);
		// peek
		tInt = getT();
		// if next token is not =, <>, <, <=, >, >=
		if (tInt != 9 && tInt != 10 && tInt != 11 && tInt != 12 && tInt != 13 && tInt != 14)
			error("Parse Error: 20 Relational operator expected");
		// skip
		nextT();
		expression(ll);
	}
}

void expression(int ll)
{
	int tInt = getT();
	// if next symbol is + or -, skip
	if (tInt == 4 || tInt == 5)
		nextT();
	term(ll);
	
	// while next symbol is + -, skip
	while (getT() == 4 || getT() == 5)
	{
		nextT();
		term(ll);
	}
}

void term(int ll)
{
	factor(ll);
	// while peeked is * or /
	while (getT() == 6 || getT() == 7)
	{
		// skip operator
		nextT();
		factor(ll);
	}
}

void factor(int ll)
{
	// get next token and skip it
	int tInt = getT();
	nextT();
	char *name;
	switch (tInt)
	{
		// ident
		case 2:
			// get string name
			name = nextS();
			
			int var_index = find_st_um(name, 2);
			int const_index = find_st_um(name,1);
			// if ident not found throw error
			if (var_index == -1 && const_index == -1)
				error("Parse Error: 11 Undeclared identifier in factor");

			// skip name
			nextT();
			break;
		
		// number
		case 3:
			// skip number
			nextT();
			break;
		
		// if open parethesis
		case 15:
			// get expression
			expression(ll);
			// if next token isn't ) throw error
			if (getT() != 16)
				error("Parse Error: 22 Right parethesis missing");
			
			// skip right parenthesis
			nextT();
			break;
		default:
			error("Parse Error: Factor default");
	}
}

int varDeclaration(int ll)
{
	// counter for number of variables
	int numVars = 0;
	// peek
	int tInt = getT();
	// if next token is not var exit procedure
	if (tInt != 29)
		return numVars;
			
	do
	{
		// skip var token first loop, then comma token
		nextT();
		
		numVars++;
		// peek
		tInt = getT();
		// if token is not ident throw error
		if (tInt != 2)
			error("Parse Error: 4 var must be followed by identifier");
		// skip ident token
		nextT();
		
		// get ident name
		char *name = nextS();
		// get ident symbol table index
		int st_index = find_st(name); // verify
		// if name found in st, unmarked, and at the given lex level, throw error
		if (st_index != -1 && symbol_table[st_index].bit == 0 && symbol_table[st_index].level == ll)
			error("Parse Error: var, ident already exists");
		// skip name
		nextT();
		// add to table
		add_st(2, name, 0, ll, numVars + 2, 0);
		
		// peek
		tInt = getT();
	} while (tInt == 17); // while next token equals comma
	
	// if next token does not equal semicolon
	if (tInt != 18)
		error("Parse Error: 17 Semicolon or } expected.");
	// skip semicolon
	nextT();
	return numVars;
}

int constDeclaration(int ll)
{
	int numConsts = 0;
	// peek
	int tInt = getT();
	
	// if next token does not equal const, skip procedure
	if (tInt != 28)
		return 0;
		
	do
	{
		// skip const token in first loop, then comma token
		nextT();
		
		// peek
		tInt = getT();
		// if token is ident
		if (tInt != 2)
			error("Parse Error: 4 var must be followed by identifier");
		// skip ident token
		nextT();
			
		// get ident name
		char *name = nextS();
		// get ident symbol table index
		int st_index = find_st(name); // add verification
		// if name found in symbol table, it is unmarked and at the current lex level, throw error
		if (st_index != -1 && symbol_table[st_index].bit == 0 && symbol_table[st_index].level == ll)
			error("Parse Error: const, ident already exists");
		// skip name
		nextT();
		
		// peek
		tInt = getT();
		// if next token does not equal = error
		if (tInt != 9)
			error("Parse Error: 3 Identifier must be followed by =.");
		// skip equal sign
		nextT();
		
		// peek
		tInt = getT();
		 
		// if next token is not a number error
		if (tInt != 3)
		 	error("Parse Eror: 2 = must be followed by a number.");
		 
		 // skip over number token
		 nextT();
		 
		 // get num
		 int num = nextI();
		 
		 // skip over number
		 nextT();
		
		// add const to table
		add_st(1, name, num, ll, 0, 0);
		// increase const counter
		numConsts++;
		
		// peek
		tInt = getT();
	} while (tInt == 17); // while next token equals comma
	
	// if next token does not equal semicolon
	if (getT() != 18)
		error("Parse Error: 17 Semicolon or } expected.");
	// skip semicolon
	nextT();
	return numConsts;
}	

int procedureDeclaration(int ll)
{
	int numProcedures = 0;
	
	// check for procedure token
	if (getT() != 30)
		return numProcedures;
		
	do
	{
		// skip procedure token
		nextT();
		
		// throw error if next symbol inst identifier
		if (getT() != 2)
			error("Parse Error: procedure declaration must be followed by identifier");
		// skip ident token
		nextT();
		
		// get ident name
		char *name = nextS();
		// get ident symbol table index
		int st_index = find_st(name);
		// if name found in symbol table, it is unmarked and at the current lex level, throw error
		if (st_index != -1 && symbol_table[st_index].bit == 0 && symbol_table[st_index].level == ll) // error: verify procedure doesnt exist
			error("Parse Error: const, ident already exists");
		// skip name
		nextT();
		
		add_st(3, name, 0, ll, 0, 0);
		numProcedures++;
		
		// semicolon or error
		if (getT() != 18)
			error("Parse Error: Procedure declaration must end with semicolon");
		// skip semicolon
		nextT();
		block(ll + 1);
		
		// semicolon or error
		if (getT() != 18)
			error("Parse Error: procedure block must end with semicolon");
		// skip semicolon
		nextT();
		
	} while (getT() == 30); // while next token equals procedure
	return numProcedures;
}
void block(int ll)
{
	int numSymbols = 0;
	
	numSymbols += constDeclaration(ll);
	numSymbols += varDeclaration(ll);
	numSymbols += procedureDeclaration(ll);
	statement(ll);
	// mark the last numSymbols number of unmarked symbols
	mark_symbols(numSymbols, ll);
}

void program()
{
	// add main procedure to symbol table
	add_st(3, "main", 0, 0, 0, 0);
	block(0);
	// peek
	if (getT() != 19)
		error("Block error: 9 Period expected");
}

symbol* parse(char *text)
{
	init(text);
	program();
	pall_sy();
	printf("\nProgram syntax correct\n");
	return symbol_table;
}

symbol getToken()
{
	return symbol_table[find_st(nextS())];
}
