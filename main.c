#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/uio.h>
#include "shell_str.h"
#include "redir_conv.h"
#include "skobki.h"







int main(int argc, char **argv)

{
    int i, count = 0, startPoint = 0, status = 0, GlobalEnd, flagSkobka, localEnd, backgroundSon, descriptor;

    int nextConveyorStart = 0, programMode = 0, savedProgramMode = 0;

    char **massiv;

    int saveOfStandardInput, saveOfStandardOutput;

    int endOfCurrentConveyor;

    saveOfStandardInput = dup(STDIN_FILENO);
    saveOfStandardOutput = dup(STDOUT_FILENO);

    redirection_in_beginning(argc, argv);

    while (1)
    {
        flagSkobka = 0;
        printf("Please, input data\n");
        massiv = input_massiv(&GlobalEnd);
        
        if (massiv == NULL)
        {
            printf("End of input\n");
            exit(-1);
        }


        if (skobkaInLine(massiv))
        {
            if (is_balanced(massiv))
                {
                    printf("Not balanced\n");
                    flagSkobka = 1;
                }
        }
        if (!flagSkobka)
        {
            startPoint = redirect(massiv, startPoint);
            massiv = delete_vertical(massiv, startPoint);
            nextConveyorStart = startPoint;
            while (nextConveyorStart < GlobalEnd)
            {

                if (strcmp(massiv[startPoint], "(\0") == 0)
                {
                    localEnd = find_local_end(massiv, startPoint);
                    printf("%d - localEnd\n", localEnd);
                    if (massiv[localEnd + 1] != NULL)
                    {
                        if (strcmp(massiv[localEnd + 1],"&\0") == 0)
                        {
                            if (fork() == 0) /*Мы в сыне, созданном для фонового процесса внутри скобки*/
                            {
                                printf("It is background mode\n");
                                descriptor = open("/dev/null", O_RDONLY, 0666);
                                dup2(descriptor, 0);
                                close(descriptor);
                                signal(SIGINT,SIG_IGN);
                                 /* Создаем внука для запуска фонового процесса*/
                                if (fork() != 0)
                                {
                                    free_massiv(massiv, GlobalEnd);
                                    exit(0);
                                }
                                else
                                {
                                    status = recursive_skobki(massiv, startPoint + 1, localEnd - 1, GlobalEnd);
                                }
                            }
                            else
                            {

                            }
                        }
                    }
                    else
                    {
                        printf("here\n");
                        print_massiv(&massiv[1]);
                        status = recursive_skobki(massiv, startPoint + 1, localEnd, GlobalEnd);
                    }

                }

                printf("%d - startPoint %d - GlobalEnd\n", startPoint, GlobalEnd );

                endOfCurrentConveyor = find_the_end_of_conveyor(massiv, startPoint, GlobalEnd);
                printf("%d - endOfCurrentConveyor\n", endOfCurrentConveyor );
                programMode = find_next_conveyor_start(massiv, &nextConveyorStart, endOfCurrentConveyor);
                printf("%d - programMode, %d - nextConveyorStart\n", programMode, nextConveyorStart );


                if ((savedProgramMode == 2 && status == 0) || (savedProgramMode == 3 && status != 0)
                || (savedProgramMode == 0) || (savedProgramMode == 1))
                {
                    status = conveyor(massiv, startPoint, programMode, endOfCurrentConveyor, GlobalEnd, GlobalEnd);
                }
                if ((savedProgramMode == 2 && status != 0) || (savedProgramMode == 3 && status == 0) )
                {
                    break;
                }
                savedProgramMode = programMode;


                startPoint = nextConveyorStart;
                printf("%d - status\n", status);
                saveInOutPUT(saveOfStandardInput, saveOfStandardOutput);
            }
            saveInOutPUT(saveOfStandardInput, saveOfStandardOutput);
            free_massiv(massiv, GlobalEnd);
            nextConveyorStart = 0;
            startPoint = 0;
            status = 0;
            savedProgramMode = 0;
            programMode = 0;
            putchar('\n');
        }
    }

    return 0;
}
