#ifndef __PARSE_H
#define __PARSE_H

typedef struct symbolftable
{
	int kind; // const = 1, var = 2, proc = 3
	char name[10]; //name up to 11 chars
	int val;	// number
	int level;	// l level
	int addr;	// m address
	int bit;
} symbol;

extern int pacounter;
extern int maxcounter;
extern int scounter;
extern char *lex_str;
extern symbol *symbol_table;

void block(int);
void add_st(int kind, char *name, int val, int level, int addr, int bit);
void init(char *scanner_output);
int getT();
void nextT();
char *nextS();
int nextI();
int find_st(char *name);
int find_st_um(char*,int);
symbol* parse(char *text);
void term(int);
void expression(int);
void factor(int);
void condition(int);
void statement(int);
void error(char *);
symbol getToken();

#endif
