#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "shell_str.h"
#include <signal.h>
#include "skobki.h"



/*
 
@description redirects input/output, if there were any redirections
@params char *operator - pointer for the first parameter of redirection (maybe <,> or >>), char *name - name of a file
 
*/



void redirection_funct(char *operator, char *name)
{
    int fileDeskriptor;
    int isOperatorEqualBigger, isOperatorEqualBiggerX2, isOperatorEqualLess;
    if ((isOperatorEqualBigger = strcmp(operator, ">\0")) == 0) /* file uses as a common output */
    {
        fileDeskriptor = open(name, O_TRUNC | O_CREAT | O_WRONLY, 0666);
        if (fileDeskriptor == -1)
        {
            printf("Problem with redirection\n");
            exit(-1);
        }
        dup2(fileDeskriptor, 1);
        close(fileDeskriptor);
    }
    if ((isOperatorEqualLess = strcmp(operator, "<\0")) == 0) /* file uses as a common output */
    {
        fileDeskriptor = open(name, O_RDONLY, 0666);
        if (fileDeskriptor == -1)
        {
            printf("Problem with redirection\n");
            exit(-1);
        }
        dup2(fileDeskriptor, 0);
        close(fileDeskriptor);
    }
    if ((isOperatorEqualBiggerX2 = strcmp(operator, ">>\0")) == 0) /* file uses as a common output & adds to the end of file*/
    {
        fileDeskriptor = open(name, O_APPEND | O_CREAT | O_WRONLY, 0666);
        if (fileDeskriptor == -1)
        {
            printf("Problem with redirection\n");
            exit(-1);
        }
        dup2(fileDeskriptor, 1);
        close(fileDeskriptor);
    }
}

/*
 
@description redirects input/output, if there were any redirections in the beginnind (command line)
@params int numberOfArguments - number of arguments in command line, char **arguments - pointer at them
 
*/


void redirection_in_beginning(int numberOfArguments, char **Arguments)
{
    int fileDescriptor;
    if (numberOfArguments == 2)
    {
        if ( (fileDescriptor = open (Arguments[1], O_RDONLY, 0666) ) == -1)
        {
            printf("Impossible\n");
            exit(-1);
        }
        else
        {
            dup2(fileDescriptor, 0);
            close(fileDescriptor);
        }

    }
    else if (numberOfArguments>2)
    {
        printf("Impossible\n");
        exit(-1);
    }
}

/*
 
@description находим начало следующего блока конвейера
@params char **massiv - pointer at massiv of lexems, int currentPosition - our current position, 
int LocalEnd - конечная позиция для данного массива
 
*/

int find_next_start_of_block_conveyor(char **massiv, int currentPosition, int LocalEnd)
{
    int TheEndOfBlockCondition = -1, nextStartPoint;
    for (; currentPosition < LocalEnd; currentPosition++)
    {
        if (massiv[currentPosition] == NULL)
        {
            break;
        }
    }
    /* Достигли ситуации, что мы либо получим адрес следующего элемента конвейера, либо мы дошли до его конца */
    nextStartPoint = currentPosition + 1;
    if ( nextStartPoint == LocalEnd )
        return TheEndOfBlockCondition;
    else
        return nextStartPoint;
}

/*
@description Считает количество процессов в данном блоке, который должен отправиться на конвейер, возвращает количество процессов
@params char **massiv - Указатель на массив лексем, int startPoint - начальная точка для подсчета, 
int endPoint - конечная точка для подсчета
*/


int count_number_of_processes(char **massiv, int startPoint, int endPoint)
{
    int counter = 0, currentPosition;
    for (currentPosition = startPoint; currentPosition <= endPoint; currentPosition++)
    {
        if (massiv[currentPosition] == NULL)
            counter++;
    }
    return counter;
}


/*
@definition Находим конец данного конвейера
@params char **massiv - массив лексем, int currentPoint - текущая точка конвейера, int LocalEnd - локальный конец данной строчки

*/



int find_the_end_of_conveyor(char **massiv, int currentPoint, int LocalEnd)
{
    for (;currentPoint < LocalEnd; currentPoint++)
    {
        if (massiv[currentPoint] != NULL)
        {
            if ( (strcmp(massiv[currentPoint], "&\0") == 0) || (strcmp(massiv[currentPoint], "&&\0") == 0)
            || (strcmp(massiv[currentPoint], "||\0") == 0) || (strcmp(massiv[currentPoint],";\0") == 0) )
            {
                return currentPoint;
            }
        }
    }
    return LocalEnd;
}



/*

@description Находим начало следующего конвейера и получает статус, что мы должны делать с предыдущим
В случае стандартной обработки (ничего не было) - 0
В случае фонового режима - 1
В случае && - 2
В случае || - 3
@params char **massiv - массив лексем, int startPoint - начальная точка обработки для данного конвейера,
int LocalEnd - конец именно этой цепочки (потенциально внутри скобок), int endPoint - конец текущего конвейера


*/




int find_next_conveyor_start(char **massiv, int *nextStartPoint, int endPoint)
{
    int endOfProgram = -1, commonEnd = 0, backgroundEnd = 1, firstCondition = 2, secondCondition = 3;

        if (massiv[endPoint] == NULL)
        {
            *nextStartPoint = endPoint + 1 ;
            return commonEnd; /* 0 */
        }

        if (strcmp(massiv[endPoint],";\0") == 0)
        {
            *nextStartPoint = endPoint + 1 ;
            free(massiv[endPoint]);
            massiv[endPoint] = NULL;
            return commonEnd; /* 0 */

        }

        if (strcmp(massiv[endPoint],"&\0") == 0)
        {
            *nextStartPoint = endPoint + 1 ;
            free(massiv[endPoint]);
            massiv[endPoint] = NULL;
            return backgroundEnd; /* 1 */
        }
        if (strcmp(massiv[endPoint],"&&\0") == 0)
        {
            *nextStartPoint = endPoint + 1 ;
            massiv[endPoint] = NULL;
            return firstCondition; /* 2 */
        }
        if (strcmp(massiv[endPoint],"||\0") == 0)
        {
            *nextStartPoint = endPoint + 1 ;
            massiv[endPoint] = NULL;
            return secondCondition; /* 3 */        
        }
    return -1;
}





/*
 
@description execute conveyor
@params int startPoint - адрес начальной точки, char **massiv - pointer at massiv of lexems,
int programMode - режим работы программы
int GlobalEnd - конец всей заданной строки, int endPoint - конец данного конвейера,
int GlobalEndForCleaning - конец всего массива для очистки памяти
 
*/

int conveyor(char **massiv, int startPoint, int programMode, int endPoint, int GlobalEnd, int GlobalEndForCleaning)
{
    int childProcess, mainProcess, descriptor, backgroundSon = 2, fileDescriptor[2], i = 0, nextStartPoint = startPoint, flag = 0;
    int status, numberOfArguments;

    numberOfArguments = count_number_of_processes(massiv, startPoint, endPoint);
    printf("%d - numberOfArguments\n", numberOfArguments );

    mainProcess = fork();
    if (mainProcess == 0)
    { /*MAIN_SON, CREATES CONVEYOR*/
        if (programMode == 1)
        {
            printf("It is background mode\n");
            descriptor = open("/dev/null", O_RDONLY, 0666);
            dup2(descriptor, 0);
            close(descriptor);
            signal(SIGINT,SIG_IGN);
            backgroundSon = fork(); /* Создаем внука для запуска фонового процесса*/
            if (backgroundSon != 0)
            {
                free_massiv(massiv, GlobalEndForCleaning);
                exit(0);
            }
            flag = 1;
        }
        while (++i <= numberOfArguments)
        { /*ЗАПУСК САМОГО КОНВЕЙЕРА ПОБЛОЧНО*/
            pipe(fileDescriptor);
            childProcess = fork();
            if (childProcess == 0)
            {
                if ( i != numberOfArguments)
                {
                    dup2(fileDescriptor[1], 1);
                }
                close(fileDescriptor[1]);
                close(fileDescriptor[0]);
                execvp(massiv[nextStartPoint], &(massiv[nextStartPoint]));
                printf("Some problem occured\n");
                free_massiv(massiv, GlobalEndForCleaning);
                exit(-1);

            }
            else {
                if (childProcess == -1)
                {
                    printf("Error while creating son-process\n");
                    close(fileDescriptor[0]);
                    close(fileDescriptor[1]);
                    free_massiv(massiv, GlobalEndForCleaning);
                    exit(-1);
                }
                else
                { /*Отец, который находит начало следующего блока конвейера*/
                    dup2(fileDescriptor[0], 0);
                    close(fileDescriptor[0]);
                    close(fileDescriptor[1]);
                    if (i != numberOfArguments)
                    {
                        nextStartPoint = find_next_start_of_block_conveyor(massiv, nextStartPoint, endPoint);
                    }
                }
            }
        }
        while (wait(&status) != -1);
        if (status)
            _exit(60);
        exit(status);

    }
    else if (mainProcess == -1)
    {
        printf("Problem happened.\n");
        free_massiv(massiv, GlobalEndForCleaning);
        return -1;
    }
    else
    {
        while (wait(&status) != -1);
        return status;
    }
    return 0;
}

