#include "pm.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX_STACK_HEIGHT 1000

// Team: Coy Torreblanca

// global processor values
int sp;     // stack pointer
int bp;     // base pointer
int pc;     // program counter
int halt;   // halt flag
int *R;     // registers
ins ir;     // current instruction
int *stack; // holds active routines
ins *code;  // holds program


// initialize global variables
void initVM()
{
	sp = MAX_STACK_HEIGHT;
	bp = sp - 1;
	pc = 0;
	halt = 0;
	ir.op = 0;
	ir.r = 0;
	ir.l = 0;
	ir.addr = 0;

	R = (int *) malloc (8 * sizeof(int));
	for( int i = 0; i < 8; i++)
		R[i] = 0;
	stack = (int *) malloc (MAX_STACK_HEIGHT * sizeof(int));
	if (stack == NULL || R == NULL)
	{
		perror("not enough memory available");
		exit(EXIT_FAILURE);
	}
}

int readTextVM(char *file)
{

	FILE * fp = fopen(file, "r");
	if (fp == NULL)
	{
		perror("Cannot open file");
		exit(EXIT_FAILURE);
	}

	int counter = 0;
	while ((fscanf(fp, "%d %d %d %d", &code[counter].op, &code[counter].r, &code[counter].l, &code[counter].addr)) != EOF)
		counter++;
	
	return counter;
		
	fclose(fp);
}

// returns base pointer of activation record l records from record given base
int base(int l, int base)
{
	int b1;
	b1 = base;

	while (l > 0)
	{
		b1 = stack[b1];
		l--;
	}
	return b1;
}

char* decipherOp(int ir)
{
	char *instruction = (char *) malloc (sizeof(char) * 4);
	switch (ir)
	{
		case 1:
			strcpy(instruction, "lit");
			break;
		case 2:
			strcpy(instruction, "rtn");
			break;
		case 3:
			strcpy(instruction, "lod");
			break;
		case 4:
			strcpy(instruction, "sto");
			break;
		case 5:
			strcpy(instruction, "cal");
			break;
		case 6:
			strcpy(instruction, "inc");
			break;
		case 7:
			strcpy(instruction, "jmp");
			break;
		case 8:
			strcpy(instruction, "jpc");
			break;
		case 10:
			strcpy(instruction, "neg");
			break;
		case 11:
			strcpy(instruction, "add");
			break;
		case 12:
			strcpy(instruction, "sub");
			break;
		case 13:
			strcpy(instruction, "mul");
			break;
		case 14:
			strcpy(instruction, "div");
			break;
		case 15:
			strcpy(instruction, "odd");
			break;
		case 16:
			strcpy(instruction, "mod");
			break;
		case 17:
			strcpy(instruction, "eql");
			break;
		case 18:
			strcpy(instruction, "neq");
			break;
		case 19:
			strcpy(instruction, "lss");
			break;
		case 20:
			strcpy(instruction, "leq");
			break;
		case 21:
			strcpy(instruction, "gtr");
			break;
		case 22:
			strcpy(instruction, "geq");
			break;
		default:
			strcpy(instruction, "sys");
			break;
	}
	return instruction;
}

// print stack
void prints(int tpc)
{
	char *instruction = decipherOp(ir.op);

	// print globals
	printf("pc: %d bp: %d sp: %d\n", pc, bp, sp);
	// print instruction
	printf("%d %s %d %d %d\n", tpc, instruction, ir.r, ir.l, ir.addr);
	
	// print registers
	printf("Registers: ");
	
	for (int i = 0; i < 8; i++)
		printf("%d ", R[i]);

	// print stacks
	printf("\nStack: ");
	
	int index = MAX_STACK_HEIGHT - 1;

 	
	while(index > (sp - 1))
	{
		/*
		int tbp = bp;
		while (tbp < MAX_STACK_HEIGHT - 1)
		{
			if (tbp == index)
			{
				printf("| ");
				break;
			}
			//printf("\nbp: %d tbp: %d\n", bp, tbp);
			tbp = stack[tbp];
			//printf("\nbp: %d tbp: %d\n", bp, tbp);
		}
		*/
		printf("%d ", stack[index--]);
	}
	printf("\n\n");
}

void runVM(ins* input, int lines, int k)
{
	initVM();
	code = input;
	/*
	if (file == NULL)
	{
		perror("Error: Code generated is null");
		exit(EXIT_FAILURE);
	}

	int lines = readTextVM(file);
	*/
	
	printf("\n");

	while (!halt)
	{
		// fetch	
		int tpc = pc;
		ir = code[pc++];
		
		// if flag print
		if (k)
			prints(tpc);
		
		// execute
		switch (ir.op)
		{
			// load literal into register
			case 1:
				R[ir.r] = ir.addr;
				break;
			// return to previous activation record environment
			case 2:
				sp = bp + 1;
				bp = stack[sp - 2];
				pc = stack[sp - 3];
				break;
			case 3:
				R[ir.r] = stack[base(ir.l,bp) - ir.addr];
				break;
			case 4:
				stack[base(ir.l, bp) - ir.addr] = R[ir.r];
				break;
			// create a new activation record
			case 5:
				stack[sp - 1] = base(ir.l,bp); // points to a given record
				stack[sp - 2] = bp;            // points to the current record
				stack[sp - 3] = pc;            // points to the instruction to return to
				bp = sp - 1;
				pc = ir.addr;
				break;
			// lower stack pointer
			case 6:
				sp = sp - ir.addr;
				break;
			// change program counter
			case 7:
				pc = ir.addr;
				break;
			// branch
			case 8:
				if (R[ir.r] == 0)
					pc = ir.addr;
				break;
			// io and halt
			case 9:
				switch (ir.addr)
				{
					// print
					case 1:
					      printf("Register %d: %d\n\n", ir.r, R[ir.r]);
					      break;
					// scan
					case 2:
						printf("Please enter a number: ");
					      scanf("%d", &R[ir.r]);
					      break;
					// halt	
					case 3:
					      halt = 1;
					      break;
				}
				break;
				
			// negate	
			case 10:
				R[ir.r] = -1 * R[ir.r];
				break;
			// add	
			case 11:
				R[ir.r] = R[ir.l] + R[ir.addr];
				break;
			// subtract
			case 12:
				R[ir.r] = R[ir.l] - R[ir.addr];
				break;
			// multiply	
			case 13:
				R[ir.r] = R[ir.l] * R[ir.addr];
				break;
			// divide	
			case 14:
				R[ir.r] = R[ir.l] / R[ir.addr];
				break;
			// check if odd	
			case 15:
				R[ir.r] = R[ir.r] % 2;
				break;
			// remainder	
			case 16:
				R[ir.r] = R[ir.l] % R[ir.addr];
				break;
			// equal	
			case 17:
				R[ir.r] = R[ir.l] == R[ir.addr];
				break;
			// not equal
			case 18:
				R[ir.r] = R[ir.l] != R[ir.addr];
				break;
			// less than
			case 19:
				R[ir.r] = R[ir.l] < R[ir.addr];
				break;
			// less than equal to
			case 20:
				R[ir.r] = R[ir.l] <= R[ir.addr];
				break;
			// greater than
			case 21:
				R[ir.r] = R[ir.l] > R[ir.addr];
				break;
			// greater than equal to
			case 22:
				R[ir.r] = R[ir.l] >= R[ir.addr];
				break;
		}
	}
}
/*
int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		perror("incorrect number of arguments");
		exit(EXIT_FAILURE);
	}
	
	runVM(argv[1]);
	return 0;
}
*/
