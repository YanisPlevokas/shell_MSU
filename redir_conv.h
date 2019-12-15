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
 
@description находим начало следующего блока конвейера
@params char **massiv - pointer at massiv of lexems, int currentPosition - our current position, 
int LocalEnd - конечная позиция для данного массива
 
*/

int find_next_start_of_block_conveyor(char **, int , int );
/*
@description Считает количество процессов в данном блоке, который должен отправиться на конвейер, возвращает количество процессов
@params char **massiv - Указатель на массив лексем, int startPoint - начальная точка для подсчета, 
int endPoint - конечная точка для подсчета
*/
int count_number_of_processes(char **, int , int );
/*
@definition Находим конец данного конвейера
@params char **massiv - массив лексем, int currentPoint - текущая точка конвейера, int LocalEnd - локальный конец данной строчки

*/
int find_next_conveyor_start(char **, int * , int  );
/*

@description Находим начало следующего конвейера и получает статус, что мы должны делать с предыдущим
В случае стандартной обработки (ничего не было) - 0
В случае фонового режима - 1
В случае && - 2
В случае || - 3
@params char **massiv - массив лексем, int startPoint - начальная точка обработки для данного конвейера,
int LocalEnd - конец именно этой цепочки (потенциально внутри скобок), int endPoint - конец текущего конвейера


*/
int find_the_end_of_conveyor(char **, int , int );
/*
 
@description execute conveyor
@params int startPoint - адрес начальной точки, char **massiv - pointer at massiv of lexems,
int programMode - режим работы программы
int GlobalEnd - конец всей заданной строки, int endPoint - конец данного конвейера,
int GlobalEndForCleaning - конец всего массива для очистки памяти
 
*/
int conveyor(char **, int , int , int , int , int);

