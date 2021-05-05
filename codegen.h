#ifndef __CODEGEN_H
#define __CODEGEN_H

#include "parse.h"
#include "pm.h"

extern int cxc;

ins* runCodeGen(symbol* st, int flag);
void emit(int op, int r, int l, int m);
void expressionc(int,int);
void termc(int, int);
void factorc(int, int);
ins* programc();
int blockc(int, int);
void statementc();
void conditionc();
void printcode();

#endif
