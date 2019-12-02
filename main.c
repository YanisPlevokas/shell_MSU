#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/io.h>
#include "shell_str.h"
#include "redir+conv.h"



int main(int argc, char **argv)
{
    int i, count = 0, backgroundFlag, startPoint, sonStatus, currentPoint, counterOfProcesses, counterOfWords;
    int process, exec;
    char **massiv/*, ***massiv_massivov*/;
    int saveOfStandardInput, saveOfStandardOutput;
 
    saveOfStandardInput = dup(STDIN_FILENO);
    saveOfStandardOutput = dup(STDOUT_FILENO);

    redirection_in_beginning(argc, argv);

    while (1)
    {
        i = 0;
        backgroundFlag = 0;
        startPoint = 0;
        printf("Please, input data\n");
        count = 0;
        massiv = input_massiv(&count);
        if (count)
            if (strcmp(massiv[count - 1], "&\0") == 0)
            {
                backgroundFlag = 1;
                free(massiv[count]);
                free(massiv[count - 1]);
                massiv[count - 1] = NULL;
                count = count - 1;
                printf("%d\n", count);
            }
        if (massiv == NULL)
        {
            printf("End of input\n");
            exit(-1);
        }
        while (1) /* redirect our files */
        {
            if ( (!(strcmp(massiv[i], "<\0") ) ) || (!(strcmp(massiv[i], ">\0")) ) || (!(strcmp(massiv[i], ">>\0")) ) )
            {
                redirection_funct(massiv[i], massiv[i+1]);
                i = i + 2;
            }
            else break;
        }

        counterOfProcesses = 0;
        counterOfWords = 0;
        massiv = delete_vertical(massiv, i, &counterOfProcesses, &counterOfWords); /* Counter of words - how many words totally are */
        conveyor(counterOfProcesses, i, massiv, counterOfWords, backgroundFlag);
        free_massiv(massiv, counterOfWords + 1);
        free(massiv);
        putchar('\n');
        dup2(saveOfStandardInput, STDIN_FILENO);
        dup2(saveOfStandardOutput, STDOUT_FILENO);
    }

    return 0;
}