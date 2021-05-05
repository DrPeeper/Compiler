#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "codegen.h"

ins* cx;
int cxc;
symbol* st;

ins* runCodeGen(symbol* calabaza, int flag)
{FILE *fp;
	char name[] = "assembly.txt";
	fp = fopen(name,"w");
	if (fp == NULL)
	{
			printf("please delete lines codegen:14-18 in codegen. They were only for printing the assembly to a file.");
			exit(0);
	}
	fclose(fp);
	
	cx = (ins*)malloc(sizeof(ins)*MAX_CODE_LENGTH);
	pacounter = 0;
	cxc = 0;
	st = calabaza;
	ins* tmp = programc(symbol_table);
	if (flag)
		printcode();
	fp = fopen(name,"w");
	if (fp == NULL)
	{
			printf("please delete lines codegen:33-34 in codegen. They were only for printing the assembly to a file.");
			exit(0);
	}
	for(int i = 0; i < cxc; i++)
			fprintf(fp, "%d %s %d %d %d %d\n", i + 1, decipherOp(cx[i].op), cx[i].op, cx[i].r, cx[i].l, cx[i].addr);
	fclose(fp);
	return tmp;
}

// search backwards
int search(int kind, char* name, int val, int level, int addr, int mark)
{
	int flag = 1;
	
	for (int i = (scounter - 1); i >= 0; i--)
	{
		flag = 1;
		if (kind != -1 && symbol_table[i].kind != kind)
			flag = 0;
		if (name != NULL && strcmp(name, symbol_table[i].name))
			flag = 0;
		if (val != -1 && symbol_table[i].val != val)
			flag = 0;
		if (level != -1 && symbol_table[i].level != level)
			flag = 0;
		if (addr != -1 && symbol_table[i].addr != addr)
			flag = 0;
		if (mark != -1 && symbol_table[i].bit != mark)
			flag = 0;
		if (flag == 1)
			return i;
	}
	return -1;
}
			

ins* programc(symbol* st)
{
	//kill
	//printcode();
	// give an id to every procedure
	int counter = 1;
	for (int i = 0; i < scounter; i++)
	{
		// check if symbol is variable
		symbol tmp = symbol_table[i];
		if ( symbol_table[i].kind == 3)
		{
			symbol_table[i].val = counter;
			counter++;
			emit(7,0,0,0);
		}
	}

	blockc(0, 1);
	
	// FIX PROCS
	// for every procedure in code
	// get id of procedure (line.addr) and search for it in symbol table
	// get symbol table address to line address
	for (int i = 0; cx[i].op == 7; i++)
	{
			int index = search(3, NULL, i+1, -1, -1, -1);
			if (index == -1)
			{
				printf("error fixing procs");
				exit(0);
			}
			cx[i].addr = symbol_table[index].addr;
	}
	// FIX CALLS
	for (int i = 0; i < cxc; i++)
		if (cx[i].op == 5)
		{
			int index = search(3, NULL, cx[i].addr, -1, -1, -1);
			if (index == -1)
			{
				exit(0);
				printf("error fixing calls");
			}
			cx[i].addr = symbol_table[index].addr;
		}		
	// emit halt
	emit(9,0,0,3);
	return cx;
}

int blockc(int ll, int proc)
{
	int numvars = 0;
	int numsyms = 0;
	int index;
	// if const
	if (getT() == 28)
	{
		do
		{
			numsyms++;
			// skip const or comma
			nextT();
			// skip ident
			nextT();
			char * name = nextS();
			
			index = search(1,  name, -1, ll, -1, 1);
			symbol_table[index].bit = 0;
			// skip name
			nextT();
			// skip eq
			nextT();
			// skip number token
			nextT();
			// skip number
			nextT();
		} while (getT() == 17);
	
		// skip semicolon	
		nextT();
	}
	
	// var
	if (getT() == 29)
	{
		do
		{
			numvars++;
			numsyms++;
			// skip var or comma
			nextT();
			// skip idenjt
			nextT();
			index = search(2, nextS(), -1, ll, -1, 1);
			symbol_table[index].bit = 0;
			// skip name
			nextT();
		} while (getT() == 17);
		// skip semicolon
		nextT();
	}
	
	// token is procedure
	if (getT() == 30)
	{
		do
		{
			numsyms++;
			// skip proc
			nextT();
			// skip ident
			nextT();
			// unmark
			index = search(3, nextS(), -1, ll, -1, 1);
			symbol_table[index].bit = 0;
			// skip name
			nextT();
			// skip semi
			nextT();
			blockc(ll+1,symbol_table[index].val);
			emit(2,0,0,0);
			// skip semi
			nextT();
		} while (getT() == 30);	
	}
	
	// update procedure address
	index = search(3, NULL, proc, -1, -1, -1);
	if (index == -1)
		error(0);
		
	symbol_table[index].addr = cxc;
	emit(6, 0, 0, 3 + numvars);
			
	// emit increment
	statementc(ll);
	
	// remark block
	int j = 0;
	for (int i = (scounter - 1); i >= 0; i--)
	{
		if (j == numsyms)
			break;
		if (symbol_table[i].bit == 0)
		{
			symbol_table[i].bit = 1;
			j++;
		}
	}
}

void statementc(int ll)
{
	//printf("%d\n", getT());
	int tmpc; 
	int tmpc2; 
	int index = -1;
	symbol token;
	switch(getT())
	{
		case 2:
			// skip 2
			nextT();
			/*
			char* name = nextS();
			for (int i = 0; i < scounter; i++)
			{
				/*
				if (st[i].kind == 2 && !strcmp(name, st[i].name) && st[i].bit == 0)
				{
					if (index == -1)
						index = i;
					else
					{
						int existing_range = st[index].level - ll;
						if (existing_range < 0)
							existing_range *= -1;
						int new_range = st[i].level - ll;
						if (new_range < 0)
							new_range *= -1;
						if (new_range < existing_range)
							index = i;
					}
				}
				
				

			}
			*/
			index = search(2,nextS(),-1,-1,-1,0);
			if (index == -1)
				error(0);

			// skip name
			nextT();
			
			// skip becomes
			nextT();
			
			expressionc(0, ll);
			
			// emit sto
			emit(4,0,ll - symbol_table[index].level , symbol_table[index].addr);
			break;
		case 27:
			// skip call
			nextT();
			// skip ident
			nextT();
			// search for procedure
			char* name = nextS();
			index = search(3, name, -1, -1, -1, 0);
			// skip name
			nextT();
			emit(5, 0, ll - symbol_table[index].level, symbol_table[index].val);
	
			break;
			
		// case begin
		case 21:
			// skip begin
			nextT();
			
			statementc(ll);
			
			// while token is semicolon
			while (getT() == 18)
			{
				nextT();
				statementc(ll);
			}
			nextT();
			break;
		// case if
		case 23:
			//skip if
			nextT();
			conditionc(ll);
			
			// save instruction for jpc
			tmpc = cxc;
			// emit jpc
			emit(8,0,0,0);
			// skp then
			nextT();
			statementc(ll);
			// if else
			if (getT() == 33)
			{
				//skip else
				nextT();

				// save current index for jump
				tmpc2 = cxc;
				emit (7,0,0,0);

				// fix jpc instruction with current index
				cx[tmpc].addr = cxc;

				statementc(ll);
				// fix jump from earlier
				cx[tmpc2].addr = cxc;
			}
			else
			// fix jump
			cx[tmpc].addr = cxc;
			break;
		// while
		case 25:
			// skip while
			nextT();
			// save jump addr
			tmpc = cxc;
			conditionc(ll);
			// skip do
			nextT();
			// save jpc addr
			tmpc2 = cxc;
			// emit jpc
			emit(8,0,0,0);
			statementc(ll); 
			// emit jump with condition addr
			emit(7,0,0,tmpc);
			// fix earlier jpc
			cx[tmpc2].addr = cxc;
			break;
			/*
		// read
		case 32:
			// skip read
			nextT();
			 token = getToken();
			// skip ident
			nextT();
			// skip name
			nextT();
			// emit read
			emit(9,0,0,2);
			// emit store
			emit(4,0,0,token.addr);
			break;
		// write
		case 31:
			//skip write
			nextT();
			 token = getToken();
			if (token.kind == 2)
			{
				// emit lod
				emit(3,0,0,token.addr);
				// emit write
				emit(9,0,0,1);
			}
			if (token.kind == 1)
			{
				// emit lit
				(1,0,0,token.addr);
				// emit write
				emit(9,0,0,1);
			}
			// skip ident
			nextT();
			// skip name
			nextT();
			break;
	}
	*/
		// read
		case 32:
			// skip read
			nextT();
			// skip ident
			nextT();
			// get var
			index = search(2, nextS(), -1, -1, -1, 0);
			// skip name
			nextT();
			// emit read
			emit(9,0,0,2);
			// emit sto
			emit(4,0,ll-symbol_table[index].level,symbol_table[index].addr);
			break;
		//write
		case 31:
			// skip write
			nextT();
			expressionc(0,ll);
			emit(9,0,0,1);
			break;
	}
}

void conditionc(int ll)
{
	// odd
	if (getT() == 8)
	{
		// skip odd
		nextT();
		expressionc(0,ll);
		// emit odd
		emit(15,0,0,0);
	}
	else
	{
		expressionc(0,ll);
		// = k
		if (getT() == 9)
		{
			// skip =
			nextT();
			expressionc(1,ll);
			// emit eql
			emit(17,0,0,1);
		}
		// <>
		else if (getT() == 10)
		{
			nextT();
			expressionc(1,ll);
			// emit not equal
			emit(18,0,0,1);
		}
		// <
		else if (getT() == 11)
		{
			nextT();
			expressionc(1,ll);
			// emit less
			emit(19,0,0,1);
		}
		// <=
		else if (getT() == 12)
		{
			nextT();
			expressionc(1,ll);
			// emit leq
			emit(20,0,0,1);
		}
		// >
		else if (getT() == 13)
		{
			nextT();
			expressionc(1,ll);
			emit(21,0,0,1);
		}
		// >=
		else if (getT() == 14)
		{
			nextT();
			expressionc(1,ll);
			emit(22,0,0,1);
		}
	}
}

void expressionc(int reg, int ll)
{
	// if token is addition
	if (getT() == 4)
		// skip operator
		nextT();
	
	// if token is minus
	if (getT() == 5)
	{
		// skip operator
		nextT();
		
		termc(reg, ll);
		
		// emit negation
		emit(10, reg, 0, 0);
		
		// continue
		while(getT() == 4 || getT() == 5);
		{
			// if plus
			if (getT() == 4)
			{
				// skip operator
				nextT();
				termc(reg + 1, ll);
				// emit add
				emit(11, reg, reg, reg + 1);
			}
			
			else if (getT() == 5)
			{
				// skip operator
				nextT();
				termc(reg + 1,ll);
				// emit sub
				emit(12, reg, reg, reg + 1);
			}
		}
		return;
	}
	
	termc(reg,ll);
	
	while (getT() == 4 || getT() == 5)
	{
		if (getT() == 4)
		{
			nextT();
			termc(reg + 1, ll);
			emit(11, reg, reg, reg + 1);
		}
		else if(getT() == 5)
		{
			nextT();
			termc(reg + 1,ll);
			emit(12, reg, reg, reg + 1);
		}
	}
}
				
void termc(int reg, int ll)
{
	factorc(reg, ll);
	// while peeked is * or /
	while (getT() == 6 || getT() == 7)
	{
		// if token equals multiply
		if (getT() == 6)
		{
			// skip operator
			nextT();
			// get next factor
			factorc(reg + 1, ll);
			// emit multiply
			emit(13, reg, reg, reg + 1);
		}
		
		else if (getT() == 7)
		{
			// skip operator
			nextT();
			// get next factor
			factorc(reg + 1, ll);
			// emit divide
			emit(14, reg, reg, reg + 1);
		}
	}
} 

/*
void factorc(int reg)
{
	// if token is ident
	if (getT() == 2)
	{
		// skip ident
		nextT();
		
		// get token
		symbol token = getToken();
		// skip name
		nextT();
		
		// if token is const
		if (token.kind == 1)
			emit(1, reg, 0, token.val);
		// if token is var
		else if (token.kind == 2)
			emit(3, reg, 0, token.addr);
		
	}
	
	// if token is number
	else if (getT() == 3)
	{
		// skip 3
		nextT();
		
		emit(1, reg, 0, nextI());
		
		// skip number
		nextT();
	}
	// token is open parenthesis
	else
	{
		// skip open parenthesis
		nextT();
		
		expressionc(reg);
		
		// skip closed parenthesis
		nextT();
	}		
}
*/
void factorc (int reg, int ll)
{
	// if ident
	if(getT() == 2)
	{
		// skip ident
		nextT();
		// get var
		int indexv = search(2,nextS(),-1,-1,-1,0);
		// get const
		int indexc = search(1,nextS(),-1,-1,-1,0);
		//skip name
		nextT();
		// find closest val to ll
		int index = -1;
		if(indexv == -1)
			index = indexc;
		else if (indexc == -1)
			index = indexv;
		else if (symbol_table[indexv].level - ll < symbol_table[indexc].level - ll)
			index = indexv;
		else
			index = indexc;

		// if val is constj
		if (symbol_table[index].kind == 1)
		{
			emit(1,reg,0,symbol_table[index].val);

		}
		else
			emit(3,reg,ll - symbol_table[index].level,symbol_table[index].val);

	}
	else if (getT() == 3)
	{
			// skip num token
			nextT();
			char* what;
			int num = strtoimax(nextS(),&what,10);
			// skip num
			nextT();
			// emit load
			emit(1, reg, 0, num);
	}
	else
	{
			nextT();
			expressionc(reg,ll);
			nextT();
	}
}

void printcode()
{
	int c = 0;
	while(c < cxc)
	{
		ins i = cx[c];
                printf("%d %s %d %d %d %d\n", c + 1, decipherOp(i.op), i.op, i.r, i.l, i.addr);
		c++;
	}
}

void emit(int op, int r, int l, int m)
{
	FILE *fp;
	char name[] = "assembly.txt";
	fp = fopen(name,"a");

	if (fp == NULL)
	{
		printf("Backup file error... please delete lines 668-675 in codegen.c and try again.");
	}

	fprintf(fp, "%d %s %d %d %d %d\n", cxc + 1, decipherOp(op), op, r, l, m);
	fclose(fp);

	if (cxc >= MAX_CODE_LENGTH)
		error("Too much code");
	else
	{
		cx[cxc].op = op;
		cx[cxc].r = r;
		cx[cxc].l = l;
		cx[cxc].addr = m;
		cxc++;
	}
}
