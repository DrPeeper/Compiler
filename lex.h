#ifndef __LEX_H
#define __LEX_H

extern char *text; // holds source code
extern int counter; // counter to place in source code
extern char *table; // holds lexeme table
extern int tcounter; // counter to lexeme table
extern char **reserved; // list of reserved words

void initLX();
void readTextLX(char *file);
void add(char c);
void adds(char* s);
void printT();
int seek(char *s);
int tokenize();
char* runLX(char *file, int flag);

#endif
