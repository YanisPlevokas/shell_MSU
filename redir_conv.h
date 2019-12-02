/*
 
@description redirects input/output, if there were any redirections
@params char *operator - pointer for the first parameter of redirection (maybe <,> or >>), char *name - name of a file
 
*/

void redirection_funct(char *, char *);

/*
 
@description redirects input/output, if there were any redirections in the beginnind (command line)
@params int numberOfArguments - number of arguments in command line, char **arguments - pointer at them
 
*/

void redirection_in_beginning(int , char **);

/*
 
@description execute conveyor
@params int numberOfArguments - number of Arguments in our line, int startPoint - the point where we start,
char **massiv - pointer at massiv of lexems, int counterOfWords - counter of words, int backgroundFlag - flag of the background mode
 
*/

void conveyor(int , int, char **, int, int);

/*
 
@description find next position after |-symbol
@params char **massiv - pointer at massiv of lexems, int currentPosition - our current position, int blockCounter - counter of blocks
totally in the line
 
*/

int find_next(char **, int , int );
