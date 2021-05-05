#ifndef __PM_H
#define __PM_H
#define MAX_CODE_LENGTH 500

typedef struct instruction
{
	int op; // operation
	int r;  // register
	int l;  // lexigraphical level
	int addr;  // extra information
} ins;
 
// global processor values
extern int sp;     // stack pointer
extern int bp;     // base pointer
extern int pc;     // program counter
extern int halt;   // halt flag
extern int *R;     // registers
extern ins ir;     // current instruction
extern int *stack; // holds active routines
extern ins *code;  // holds program

void initVM();
int readTextVM(char *file);
int base(int l, int base);
char* decipherOp(int ir);
void prints(int tpc);
void runVM(ins* input, int lines, int k);

#endif
