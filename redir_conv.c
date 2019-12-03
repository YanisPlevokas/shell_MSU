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
        dup2(fileDeskriptor, 1);
        close(fileDeskriptor);
    }
    if ((isOperatorEqualLess = strcmp(operator, "<\0")) == 0) /* file uses as a common output */
    {
        fileDeskriptor = open(name, O_RDONLY, 0666);
        dup2(fileDeskriptor, 0);
        close(fileDeskriptor);
    }
    if ((isOperatorEqualBiggerX2 = strcmp(operator, ">>\0")) == 0) /* file uses as a common output & adds to the end of file*/
    {
        fileDeskriptor = open(name, O_APPEND | O_CREAT | O_WRONLY, 0666);
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
        exit(0);
    }
}

/*
 
@description find next position after |-symbol
@params char **massiv - pointer at massiv of lexems, int currentPosition - our current position, int blockCounter - counter of blocks
totally in the line
 
*/

int find_next(char **massiv, int currentPosition, int blockCounter)
{
    for (int i = currentPosition; i < blockCounter; i++)
    {
        if (massiv[i] == NULL)
        {
            return (i+1);
        }
    }
}

/*
 
@description execute conveyor
@params int numberOfArguments - number of Arguments in our line, int startPoint - the point where we start,
char **massiv - pointer at massiv of lexems, int counterOfWords - counter of words, int backgroundFlag - flag of the background mode
 
*/

void conveyor(int numberOfArguments, int startPoint,  char **massiv, int counterOfWords, int backgroundFlag)
{
    int childProcess, mainProcess, descriptor,  fileDescriptor[2], i = 0, nextStartPoint = startPoint, difference = 0;

    mainProcess=fork();
    if (mainProcess==0)
    {
        if (backgroundFlag)
        {
            printf("It is background mode\n");
            descriptor = open("/dev/null", O_RDONLY, 0666);
            dup2(descriptor, 0);
            signal(SIGINT,SIG_IGN);
        }
        while (++i <= numberOfArguments)
        {
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
                fprintf(stderr, "Something has gone wrong\n");
                free_massiv(massiv, counterOfWords);
                free(massiv);
                exit(1);

            }
            else {
                if (childProcess == -1)
                {
                    printf("Error while creating son-process\n");
                    close(fileDescriptor[0]);
                    close(fileDescriptor[1]);
                    free_massiv(massiv, counterOfWords);
                    free(massiv);
                    exit(1);
                }
                else
                {
                    dup2(fileDescriptor[0], 0);
                    close(fileDescriptor[0]);
                    close(fileDescriptor[1]);
                    if (i != numberOfArguments)
                    {
                        nextStartPoint = find_next(massiv, nextStartPoint, counterOfWords);
                    }
                }
            }
            free_massiv(massiv, counterOfWords);
            free(massiv);
        }
        if (!backgroundFlag)
            {
                while (wait(NULL)!=-1);
                exit(1);
            }
    }
    else if (mainProcess==-1)
    {
        printf("Problem happened.\n");
        return;
    }
    else
    {
        if (!backgroundFlag) while (wait(NULL)!=-1);
    }
}

