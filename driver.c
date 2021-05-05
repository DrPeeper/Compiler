#include <stdio.h>
#include <stdlib.h>
#include "lex.h"
#include "parse.h"
#include "codegen.h"
#include "pm.h"

int main(int argc, char **argv)
{
	printf("Starting driver...");
	int af = 0;
	int vf = 0;
	int lf = 0;
	
	if (argc < 2)
		printf("error : please include the file name");
	// for testing
	if (argc == 5)
	{
		af = 1;
		vf = 1;
		lf = 1;
	}
	else if (argc != 2)
	{
		if (argv[2][1] == 'v')
			vf = 1;
		else if (argv[2][1] == 'a')
			af = 1;
		else
			lf = 1;
	}
	if (argc == 4)
	{
		if (argv[3][1] == 'v')
			vf = 1;
		else if (argv[3][1] == 'a')
			af = 1;
		else
			lf = 1;
	}
	
	//printf("%s\n", runLX(argv[2]));
	char *lex_string = runLX(argv[1],lf);
	symbol* st = parse(lex_string);
	ins* x = runCodeGen(symbol_table, af);
	runVM(x, cxc, vf);
	return 0;
}
