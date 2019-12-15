
typedef struct list
{
    char *string;
    struct list *next;
} list;
/*
 
@description convert list to massiv structure
@params  list *node - pointer at structure list, int *count - value of words
 
*/
char ** list_to_mas(list *, int *);
/*
 
@description add node in list
@params  list *node - pointer at structure list, char *stringNew - new string, that is going to be added
 
*/
list * add_node(list *, char *);
/*
 
@description print list
@params  list *node - pointer at structure list
 
*/
void print_list (list *node);
/*
 
@description inputs string
@params int *status - pointer at status of work in this function, 
char *symboll - pointer at the saved symbol, int *status_symb - status of the symbol, was it saved or not
 
*/
char* input_str(int *, char *, int *);
/*
 
@description free list
@params  list *spisok - pointer at structure list
 
*/
list * free_list(list *);
/*
 
@description count number of nodes
@params  list *node - pointer at structure list
 
*/
int spisok_count(list *);
/*
 
@description prints "massiv" structure
@params  char **massiv - pointer at structure "massiv"
 
*/
void print_massiv(char **);
/*

@description free "massiv" structure
@params  char **massiv - pointer at structure "massiv", int counter - counter of words
 
*/
void free_massiv(char **, int);

/*
 
@description convert our input into massiv of lexems
@params int *counter - pointer at the value of words in the line
 
*/








int find_end(char **massiv, int);



char ** input_massiv(int *);
/*
 
@description convert massiv into massiv_of_massiv structure
@params  char **mass - pointer at structure maasiv, int *elems - number of elements in massiv
 
*/
char *** mas_to_massives(char **, int *);
/*
 
@description replases all the |-symbols in massiv into NULL
@params char **mass - pointer at massiv of lexems,
int currentPoint - currentPoint, where we stand, int *counter - pointer at value of words, 
int *blockCounter - counter of blocks in line
 
*/
char **delete_vertical(char **, int );

/*
@description Возвращает 1, если скобки присутствуют в строке, 0 - иначе
@params char **massiv - массив лексем
*/


int skobkaInLine(char **);

/*
@description Возвращает 0, если лексемы сбалансированны по скобкам, иначе -1
@params char **massiv -  указатель на массив лексем

*/


int is_balanced( char **);
