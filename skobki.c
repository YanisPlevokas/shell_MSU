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



/*
@description Находит адрес конца текущего скобочного выражения, очищает и меняет на NULL скобочные позиции
@params char **massiv - массив лексем, int localStart - точка локального старта
*/


int find_local_end(char **massiv, int localStart)
{
	int counter = 1;
	int i = localStart + 1;
	while (massiv[i] != NULL)
	{
		printf("%s, %d\n", massiv[i], counter);
		if (strcmp(massiv[i], "(\0") == 0)
            counter++;
        if (strcmp(massiv[i], ")\0") == 0)
            counter--;
        if (!counter)
        {
            break;
        }
		i++;
	}
	if (counter)
		return -1;
	else
    {
        free(massiv[localStart]);
        massiv[localStart] = NULL;
		free(massiv[i]);
		massiv[i]= NULL;
		return i;
    }
}



/*
@description Находим режим, в котором запускается скобка
@params char **massiv - массив лексем, int endPoint - позиция закрывающей соответствующей скобки
*/


int find_mode_of_skobka(char **massiv, int endPoint)
{
	int endOfProgram = -1, commonEnd = 0, backgroundEnd = 1, firstCondition = 2, secondCondition = 3;
		if (massiv[endPoint + 1] == NULL)
        {
            return commonEnd; /* 0 */
        }

        if (strcmp(massiv[endPoint + 1],";\0") == 0)
        {
            free(massiv[endPoint + 1]);
            massiv[endPoint + 1] = NULL;
            return commonEnd; /* 0 */

        }

        if (strcmp(massiv[endPoint],"&\0") == 0)
        {
            free(massiv[endPoint + 1]);
            massiv[endPoint + 1] = NULL;
            return backgroundEnd; /* 1 */
        }
        if (strcmp(massiv[endPoint],"&&\0") == 0)
        {
            free(massiv[endPoint + 1]);
            massiv[endPoint + 1] = NULL;
            return firstCondition; /* 2 */
        }
        if (strcmp(massiv[endPoint],"||\0") == 0)
        {
            free(massiv[endPoint + 1]);
            massiv[endPoint + 1] = NULL;
            return secondCondition; /* 3 */        
        }
        return endOfProgram;

}
/*

@description Возвращает первоначальный ввод-вывод
@params int saveOfStandardInput - сохраненный дескриптор стандартного ввода, int saveOfStandardOutput - сохраненный дескриптор стандартного вывода

*/

void saveInOutPUT(int saveOfStandardInput, int saveOfStandardOutput)
{
    dup2(saveOfStandardInput, STDIN_FILENO);
    dup2(saveOfStandardOutput, STDOUT_FILENO);
}

/*

@definition Осуществляет перенаправления в начале строки, возвращает позицию после всех перенаправлений - наша рабочая начальная позиция
@params char **massiv - Массив лексем, int localStart - начальная точка, с которой мы делаем перенаправления

*/


int redirect(char **massiv, int localStart)
{
    int isOperatorEqualBigger, isOperatorEqualBiggerX2, isOperatorEqualLess;
    int i = localStart;
    while (1) 
        {
            if (massiv[i] != NULL)
            {
                if ( (!(isOperatorEqualLess = strcmp(massiv[i], "<\0") ) ) || (!(isOperatorEqualBigger = (strcmp(massiv[i], ">\0"))) ) || (!(isOperatorEqualBiggerX2 = strcmp(massiv[i], ">>\0")) ) )
                {
                    redirection_funct(massiv[i], massiv[i+1]);
                    i = i + 2;
                }
                else 
                {
                    return i;
                }
            }
            else
                return i;
        }

}



/*

@description Рекурсивная функция для работы со скобками, возвращает статус завершения процесса внутри скобок
@params char **massiv - массив лексем, int localStart - адрес начальной точки, с которой мы начинаем работу,
int localEnd - конец для данного выражения внутри скобок, int GlobalEnd - конец всей строки (для очистки памяти)

*/



int recursive_skobki(char **massiv, int localStart, int localEnd, int GlobalEnd)
{
	int i, count = 0, status = 0, flagSkobka, backgroundSon;

    int nextConveyorStart = localStart, programMode = 0, savedProgramMode = 0;

    int saveOfStandardInput, saveOfStandardOutput;

    int endOfCurrentConveyor;



    saveOfStandardInput = dup(STDIN_FILENO);
    saveOfStandardOutput = dup(STDOUT_FILENO);
    putchar('\n');
    printf("МЫ В СКОБКАХ, %d - start, %d - end\n", localStart, localEnd);
	while (nextConveyorStart < localEnd)
        {
            printf("%d - localStart %d - LocalEnd\n", localStart, localEnd );

            endOfCurrentConveyor = find_the_end_of_conveyor(massiv, localStart, localEnd);
            printf("%d - endOfCurrentConveyor\n", endOfCurrentConveyor );
            programMode = find_next_conveyor_start(massiv, &nextConveyorStart, endOfCurrentConveyor);
            printf("%d - programMode, %d - nextConveyorStart\n", programMode, nextConveyorStart );


            if ((savedProgramMode == 2 && status == 0) || (savedProgramMode == 3 && status != 0)
            || (savedProgramMode == 0) || (savedProgramMode == 1))
            {
                status = conveyor(massiv, localStart, programMode, endOfCurrentConveyor, localEnd, GlobalEnd);
            }
            if ((savedProgramMode == 2 && status != 0) || (savedProgramMode == 3 && status == 0) )
            {
                break;
            }
            savedProgramMode = programMode;


            localStart = nextConveyorStart;
            printf("%d - status\n", status);
            saveInOutPUT(saveOfStandardInput, saveOfStandardOutput);
        }
    printf("%d - status skobok\n", status);
    return status;





}
