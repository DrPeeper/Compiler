// Coy Torreblanca
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAX_CODE_LENGTH 10000
#define MIN_TABLE_SIZE 2 

// global variables
char *text; // holds source code
int counter; // counter to place in source code
char *table; // holds lexeme table
int tcounter; // counter to lexeme table
char * reserved [] = { "const", "var", "procedure", "call", "begin", "end", "if", "then", "else", "while", "do", "read", "write", "odd" }; // list of reserved words

// initializes glovals
void initLX()
{
	text = (char *) malloc ((MAX_CODE_LENGTH + 1) * sizeof(char));
	table = (char *) malloc ((MIN_TABLE_SIZE + 1) * sizeof(char));
	table[MIN_TABLE_SIZE] = '\0';
	tcounter = 0;
	text[MAX_CODE_LENGTH] = '\0';

	for (int i = 0; i < MIN_TABLE_SIZE; i++)
		table[i] = ' ';

	if (text == NULL || table == NULL)
	{
		perror("not enough memory available");
		exit(EXIT_FAILURE);
	}

	counter = 0;
}

// reads from file into text and prints source code
void readTextLX(char *file)
{
	initLX();
	FILE * fp = fopen(file, "r");
	if (fp == NULL)
	{
		perror("Cannot open file");
		exit(EXIT_FAILURE);
	}

	int counter = 0;
	printf("Source Code:\n");
	while (counter < MAX_CODE_LENGTH && (fscanf(fp,"%c",&text[counter]) != EOF))
	{
		//print source code
		 printf("%c", text[counter]);
		counter++;
	}
	printf("\n");
	text[counter] = '\0';	
	fclose(fp);
}

// adds character to table
// expands table if table is full
void add(char c)
{	
	// if table is full, create a new table and copy over old table
	if(tcounter == strlen(table))
	{
		int l = strlen(table);
		// create new table
		char *tmp = (char*) malloc ((l * 2) * sizeof(char));
		tmp[(l * 2) - 1] = '\0';
		
		// copy over old table
		for(int i = 0; i < l; i++)
			tmp[i] = table[i];
		for(int i = l; i < (l * 2) - 1; i++)
			tmp[i] = ' ';
		
		free(table);
		table = tmp;
	}
	table[tcounter++] = c;
}

// add the given string to table
void adds(char* s)
{
	for (int i = 0; i < strlen(s); i++)
		add(s[i]);
}

// prints the lexeme table accordint to project specifications
void printT()
{
	printf("Lexeme List: %s\n", table);
	printf("Lexeme Table\nlexeme\ttoken type\n");
	int pcounter = 0;
	
	while (pcounter != tcounter)
	{
		// read token
		switch (table[pcounter++])
		{
			// accordint to token print out appropriate token type
			case '1':
				switch (table[pcounter++])
				{

					case '0':
						printf("<>\t10\n");
						break;
					case ' ':
						printf("null\t1\n");
						pcounter--;
						break;
					case '1':
						printf("<\t11\n");
						break;
					case '2':
						printf("<=\t12\n");
						break;
					case '3':
						printf(">\t13\n");
						break;
					case '4':
						printf(">=\t14\n");
						break;
					case '5':
						printf("(\t15\n");
						break;
					case '6':
						printf(")\t16\n");
						break;
					case '7':
						printf(",\t17\n");
						break;
					case '8':
						printf(";\t18\n");
						break;
					case '9':
						printf(".\t19\n");
						break;
				}
				pcounter++;
				break;

			case '2':
				switch (table[pcounter++])
				{
					case ' ':
						while (table[pcounter] != ' ')
							printf("%c", table[pcounter++]);
						printf("\t2\n");
						pcounter--;
						break;
					case '0':
						printf(":=\t20\n");
						break;
					case '1':
						printf("begin\t21\n");
						break;
					case '2':
						printf("end\t22\n");
						break;
					case '3':
						printf("if\t23\n");
						break;
					case '4':
						printf("then\t24\n");
						break;
					case '5':
						printf("while\t25\n");
						break;
					case '6':
						printf("do\t26\n");
						break;
					case '7':
						printf("call\t27\n");
						break;
					case '8':
						printf("const\t28\n");
						break;
					case '9':
						printf("var\t29\n");
						break;
				}
				pcounter++;
				break;
			
			case '3':
				switch (table[pcounter++])
				{
					case ' ':
						while (table[pcounter] != ' ')
							printf("%c", table[pcounter++]);
						printf("\t3\n");
						pcounter--;
						break;
					case '0':
						printf("procedure\t30\n");
						break;
					case '1':
						printf("write\t31\n");
						break;
					case '2':
						printf("read\t32\n");
						break;
					case '3':
						printf("else\t33\n");
						break;
				}
				pcounter++;
				break;
			case '4':
				printf("+\t4\n");
				pcounter++;
				break;
			case '5':
				printf("-\t5\n");
				pcounter++;
				break;
			case '6':
				printf("*\t6\n");
				pcounter++;
				break;
			case '7':
				printf("/\t7\n");
				pcounter++;
				break;
			case '8':
				printf("odd\t8\n");
				pcounter++;
				break;
			case '9':
				printf("=\t9\n");
				pcounter++;
				break;

	
		}
	}
}

// returns 1 if the current string found in the text at the counter
int seek(char *s)
{
	return !memcmp(&text[counter],s,strlen(s));
}

// fill lexeme table according to given text
int tokenize()
{
	// for every character in given text
	for (counter = 0; counter < strlen(text); counter++)
	{
		// skip white space characters
		if (text[counter] == ' ' || text[counter] == '\n' || text[counter] == '\t')
			continue;
		// if character is alphabetical begin reading identifier or reserved word
		else if (isalpha(text[counter]))
		{
			// flag if reserved word
			int flag = 0;
			// check if reserved workd
			for (int j = 0; j < 14; j++)
			{
				if (seek(reserved[j]))
				{
					// if reserved word raise flag and write token
					flag = 1;
					counter+= strlen(reserved[j]) - 1;
					switch (j)
					{
						case 0:
							adds("28 ");
							break;
						case 1:
							adds("29 ");
							break;
						case 2:
							adds("30 ");
							break;
						case 3:
							adds("27 ");
							break;
						case 4:
							adds("21 ");
							break;
						case 5:
							adds("22 ");
							break;
						case 6:
							adds("23 ");
							break;
						case 7:
							adds("24 ");
							break;
						case 8:
							adds("33 ");
							break;
						case 9:
							adds("25 ");
							break;
						case 10:
							adds("26 ");
							break;
						case 11:
							adds("32 ");
							break;
						case 12:
							adds("31 ");
							break;
						case 13:
							adds("8 ");
							break;
					}
					break;
				}
			}
			// if reserved word was found skip
			if (flag)
				continue;

			// counter to find identifier length
			int tmp = 0;

			// add identifer token number to table
			adds("2 ");
			// while character is alphanumeric we are still reading identifier
			while (isalnum(text[counter]))
			{
				// if identifier is too long, throw error
				if (++tmp == 12)
				{
					printf("Scanner Error: Identifier too long\n");
					return 0;
				}
				// add identifier character
				add(text[counter++]);
				
				// check if remaining characters are reserved words
				for (int j = 0; j < 14; j++)
					if (seek(reserved[j]))
						break;
			}
			counter--;
			add(' ');
			continue;
		}

		// if next token is number
		else if (isdigit(text[counter]))
		{
			int tmp = 0;
			int tmpc = counter;
			int flag = 0;
			
			// read ahead up to 6 characters to see if number is less than 6 digits and does not have characters
			while (isdigit(text[tmpc]) && !flag)
			{
				tmpc++;
				if(++tmp == 6)
				{
					printf("Scanner Error: Number too long\n");
					exit(0);
					return 0;
					flag = 1;
				}
			}

			// if there is character in number raise flag
			if (!flag && isalpha(text[tmpc]))
			{
				printf("Scanner Error: Invalid ident\n");
				return 0;
				flag = 1;
			}

			// if flag is raised skip number
			if (flag)
			{
				while (isalnum(text[counter]))
					counter++;
				counter--;
				continue;
			}
			// if flag isnt raised add number
			else
			{
				adds("3 ");
				while (isdigit(text[counter]))
				{
					add(text[counter++]);
				}
				counter--;
				add(' ');
				continue;
			}
		}
		// if the character is a required symbol then write to table
		else if (text[counter] == '+')
		{
			adds("4 ");
			continue;
		}
		else if (text[counter] == '-')
		{
			adds("5 ");
			continue;
		}
		else if (text[counter] == '*')
		{
			adds("6 ");
			continue;
		}
		else if (text[counter] == '=')
		{
			adds("9 ");
			continue;
		}
		else if (text[counter] == '(')
		{
			adds("15 ");
			continue;
		}
		else if (text[counter] == ')')
		{
			adds("16 ");
			continue;
		}
		else if (text[counter] == ',')
		{
			adds("17 ");
			continue;
		}
		else if (text[counter] == ';')
		{
			adds("18 ");
			continue;
		}
		else if (text[counter] == '.')
		{
			adds("19 ");
			continue;
		}
		
		// less than may be a multi character symbol, check for that, and if it is, then write the token
		else if (text[counter] == '<')
		{
			switch(text[++counter])
			{
				case '>':
					adds("10 ");
					break;
				case '=':
					adds("12 ");
					break;
				default:
					counter--;
					adds("11 ");
					break;
			}
			continue;
		}
		else if (text[counter] == '>')
		{
			if (text[counter + 1] == '=')
			{
				counter++;
				adds("14 ");
			}
			else
				adds("13 ");
			continue;
		}
		// check for belongs too symbol
		else if (text[counter] == ':' && text[counter + 1] == '=')
		{
			counter++;
			adds("20 ");
			continue;
		}
		// check for slash symbol or comment
		else if (text[counter] == '/')
		{
			// if true then comment
			if (text[counter + 1] == '*')
			{
				counter += 2;
				// skip comment
				while(counter < strlen(text) && text[counter] != '*' && text[counter] != '/')
					counter++;
				counter++;
				continue;
			}
			// otherwise, slash symbol
			else
			{
				adds("7 ");
				continue;
			}
		}	
		// if we arrive here then there is an invalid symbol
		else
		{
			printf("Scanning Error: Invalid Symbol: %c\n", text[counter]);
			return 0;
		}
	}
	return 1;
}

char* runLX(char *file, int flag)
{
	readTextLX(file);
	tokenize();
	if (flag)
		printT();
	//printf("\n%s", table);
	return table;
}
/*
int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		perror("incorrnct number of arguments");
		exit(EXIT_FAILURE);
	}

	runLX(argv[1]);
	return 1;
}
*/
