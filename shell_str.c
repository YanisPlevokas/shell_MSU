#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/io.h>
#include "biblioteka.h"

/*
 
@description convet our input into massiv of lexems
@params int *counter - pointer at the value of words in the line
 
*/

char ** input_massiv(int *counter)
{
    char **massiv1;
    int status = 0, status_symb = 0;
    int count = 0;
    char *string = NULL, *check_symbol = NULL;
    list *spisok = NULL;
    if ((check_symbol = malloc(1)) == NULL)
    {
        printf("Impossible to allocate\n");
        exit(-1);
    }
    while (status == 0)
    {
        string = input_str(&status, check_symbol, &status_symb);
        if (status == 0)
        {
            spisok = add_node(spisok, string);
        }
        if (status == 1)
        {
            free(check_symbol);
            status = 0;
            status_symb = 0;
            return NULL;
        }
        if (status == 2)
        {
            spisok = free_list(spisok);
            status = 0;
        }
        if (status == 3)
        {
            if ((string != NULL) && ((strcmp(string, "")) != 0) )
            {
                spisok = add_node(spisok, string);
            }
            massiv1 = list_to_mas(spisok, &count);
            free(check_symbol);
            *counter = count;
            status = 0;
            status_symb = 0;
            spisok = free_list(spisok);
            return massiv1;
        }
    }
}


/*
 
@description replases all the |-symbols in massiv into NULL
@params char **mass - pointer at massiv of lexems,
int currentPoint - currentPoint, where we stand, int *counter - pointer at value of words, 
int *blockCounter - counter of blocks in line
 
*/
char **delete_vertical(char **mass, int currentPoint, int *counter, int *blockCounter)
{
    int start = currentPoint;
    while (mass[currentPoint] != NULL)
    {
        if (strcmp(mass[currentPoint], "|\0" ) == 0 )
        {
            mass[currentPoint] = NULL;
            (*counter)++;
        }
        currentPoint++;
    }
    (*counter)++;
    (*blockCounter) = currentPoint - start;
    return mass;


}

/*
 
@description convert massiv into massiv_of_massiv structure
@params  char **mass - pointer at structure maasiv, int *elems - number of elements in massiv
 
*/
char *** mas_to_massives(char **mass, int *elems)
{
    char ***massiv_massivov = NULL;
    char *tester = "|\0";
    int currentPosition = 0, CurrentWordInMassiv, startPoint = 0, i = 0, j = 0, k = 0, counter = 0, flag = 0;
    while (mass[i] != NULL)  /* counter for sentences between | */
    {
        if ((strcmp(mass[i], "|")) == 0)
        {
            counter++;
        }
        i++;
    }
    if ((massiv_massivov = malloc(sizeof(char**)*(counter + 1))) == NULL)
    {
        printf("Impossible to allocate\n");
        free(massiv_massivov);
        exit(-1);
    }
    printf("ok\n");
    i = 0;
    while (1)
    {
        if (mass[currentPosition] != NULL) printf("%s\n", mass[currentPosition]);
        flag = 0;
        if (mass[currentPosition] == NULL)
        {
                printf("CYCLE1\n");
                if (j != 0) /* if there was anything before | */
                {
                    if ((massiv_massivov[i] = malloc(sizeof(char*)*(j+1))) == NULL)
                    {
                        printf("Impossible to allocate\n");
                        free(massiv_massivov);
                        exit(-1);
                    }
                    for (CurrentWordInMassiv = 0; CurrentWordInMassiv < j; CurrentWordInMassiv++)
                    {
                        printf("%s\n", mass[startPoint]);
                        massiv_massivov[i][CurrentWordInMassiv] = mass[startPoint];
                        startPoint++;
                    }
                    startPoint++;

                }
                massiv_massivov[i][j] = NULL;
                print_massiv(massiv_massivov[i]);
                break;
            }

        if ((strcmp(mass[currentPosition], tester)) == 0 ) /* if we meet | */
        {
            printf("CYCLE1\n");
            if (j != 0) /* if there was anything before | */
            {
                if ((massiv_massivov[i] = malloc(sizeof(char*)*(j+1))) == NULL)
                {
                    printf("Impossible to allocate\n");
                    free(massiv_massivov);
                    exit(-1);
                }
                for (int CurrentWordInMassiv = 0; CurrentWordInMassiv < j; CurrentWordInMassiv++)
                {
                    printf("%s\n", mass[startPoint]);
                    massiv_massivov[i][CurrentWordInMassiv] = mass[startPoint];
                    startPoint++;
                }
                startPoint++;

            }
            massiv_massivov[i][j] = NULL;
            print_massiv(massiv_massivov[i]);
            i++;
            j = 0;
            flag = 1;
        }
        if (flag == 0) /* if we didn't meet | */
        {
            printf("CYCLE2\n");
            j++;
        }
        currentPosition++;
    }
    *elems = i + 1;
    return massiv_massivov;
}

/*
 
@description convert list to massiv structure
@params  list *node - pointer at structure list, int *count - value of words
 
*/
char ** list_to_mas(list *node, int* count)
{
    char **massiv;
    int i = 0;
    if ((massiv = malloc(sizeof(char*)*(1 + spisok_count(node)))) == NULL)
    {
        printf("Impossible to allocate\n");
        free(massiv);
        exit(1);
    }
    while (node != NULL)
    {
        massiv[i] = node->s;
        node = node->next;
        i = i + 1;
    }
    massiv[i] = NULL;
    *count = i;
    return massiv;
}

/*
 
@description prints "massiv" structure
@params  char **massiv - pointer at structure "massiv"
 
*/
void print_massiv(char **massiv)
{
    int i = 0;
    while (massiv[i] != NULL)
    {
        printf("%s\n", massiv[i]);
        i++;
    }

}

/*
 
@description free "massiv" structure
@params  char **massiv - pointer at structure "massiv", int counter - counter of words
 
*/
void free_massiv(char **massiv, int counter)
{
    int i = 0;
    while ( massiv[i] != NULL )
    {
        free(massiv[i]);
        massiv[i] = NULL;
        i++;
    }
}

/*
 
@description add node in list
@params  list *node - pointer at structure list, char *stringNew - new string, that is going to be added
 
*/
list * add_node(list *node, char *stringNew)
{
    list *newNode;
    if (node == NULL)
    {
        if ((newNode = malloc(sizeof(list))) == NULL)
        {
            printf("Impossible to allocate\n");
            exit(1);
        }
        newNode->s = stringNew;
        newNode->next = NULL;
        node = newNode;
        return newNode;
    }
    else
    {
        node->next = add_node(node->next, stringNew);
    }
    return node;
}

/*
 
@description print list
@params  list *node - pointer at structure list
 
*/
void print_list (list *node)
{
    while (node != NULL)
    {
        printf("%s\n", node->s);
        node = node->next;
    }
}
/*
 
@description count number of nodes
@params  list *node - pointer at structure list
 
*/

int spisok_count(list *node)
{
    int i = 0;
    while (node != NULL)
    {
        i++;
        node = node->next;
    }
    return i;
}
char* input_str(int *status, char *symboll, int *status_symb)
{
    char symb, *str;
    int lenStr = 0, N = 20, borderFlag = N, input_flag = 0, flag = 0, quotes_flag = 0;
    int more_counter = 0, ampersand_counter = 0, vertical_counter = 0, common_input_status = 0;
    if (*status_symb == 1)
    {
        symb = symboll[0];
        *status_symb = 0;
    }
    else
    {
        symb = getchar();
    }

    if (symb == '\n')
    {
        *status = 3;
        return NULL;
    }
    if (symb != EOF)
    {
        if ((str = malloc(N)) == NULL)
        {
            printf("Impossible to allocate\n");
        }
        str[0] = '\n';
        flag++;
    }
    while ( (symb != EOF) && (flag == 1) )
    {
        if ( (symb != ' ')  || (quotes_flag == 1) )
        {
            if ( (symb == '\n') && (quotes_flag) )
            {
                printf("Quotes are not closed, please, try again\n");
                *status_symb = 0;
                *status = 2;
                return NULL;
            }
            if ( (symb == '\n') && (!quotes_flag) )
            {
                str[lenStr] = '\0';
                *status = 3;
                return str;
            }
            if (lenStr == borderFlag - 1)
            {
                borderFlag = borderFlag + N;
                if ((str = realloc(str, borderFlag)) == NULL)
                {
                    printf("You don't have enough memory\n");
                    exit(0);
                }
            }
            input_flag++;

            if (symb == '"')
            {
                quotes_flag = quotes_flag ^ 1;
            }


            if ((quotes_flag) && ( (str[0] == '>') || (str[0] == '&') || (str[0] == '|') ) )
            {
                str[lenStr] = '\0';
                symboll[0] = '"';
                *status_symb = 1;
                return str;
            }


            if ((!quotes_flag) && (symb != EOF))
            {



                if ( ((symb == ';') || (symb == '>') || (symb == '(') || (symb == ')') || (symb == '<') || (symb == '|') || (symb == '&')) && (((str[0] == ';') || (str[0] == '>') || (str[0] == '<') || (str[0] == '|') || (str[0] == '&') || (str[0] == '(') || (str[0] == ')'))) && (str[0] != symb) )
                {
                    str[lenStr] = '\0';
                    *status_symb = 1;
                    symboll[0] = symb;
                    return str;
                }

                if ( ( symb == '|' )  && (vertical_counter == 0) && ( common_input_status != 0)  )
                {
                    str[lenStr] = '\0';
                    *status_symb = 1;
                    symboll[0] = '|';
                    return str;
                }

                if ( ( symb == '|' ) && ( vertical_counter == 1  ) && ( common_input_status == 0)  )
                {
                    str[lenStr] = symb;
                    lenStr++;
                    str[lenStr] = '\0';
                    symboll[0] = '\0';
                    return str;
                }


                if ( ( symb == '|' ) && ( vertical_counter == 0 ) && ( common_input_status == 0) )
                {
                    str[lenStr] = symb;
                    lenStr++;
                    vertical_counter++;
                }



                if ( ( symb == '&' )  && (ampersand_counter == 0) && ( common_input_status != 0) )
                {
                    str[lenStr] = '\0';
                    *status_symb = 1;
                    symboll[0] = '&';
                    return str;
                }
                if ( ( symb == '&' ) && ( ampersand_counter == 1 ) && ( common_input_status == 0)  )
                {
                    str[lenStr] = symb;
                    lenStr++;
                    str[lenStr] = '\0';
                    symboll[0] = '\0';
                    return str;
                }
                if ( ( symb == '&' ) && ( ampersand_counter == 0 ) && ( common_input_status == 0) )
                {
                    str[lenStr] = symb;
                    lenStr++;
                    ampersand_counter++;
                }


                if ( ( symb == '>' )  && (more_counter == 0) && ( common_input_status != 0) )
                {
                    str[lenStr] = '\0';
                    *status_symb = 1;
                    symboll[0] = '>';
                    return str;
                }
                if ( ( symb == '>' ) && ( more_counter == 1  ) && ( common_input_status == 0)  )
                {
                    str[lenStr] = symb;
                    lenStr++;
                    str[lenStr] = '\0';
                    symboll[0] = '\0';
                    return str;
                }
                if ( ( symb == '>' ) && ( more_counter == 0 ) && ( common_input_status == 0) )
                {
                    str[lenStr] = symb;
                    lenStr++;
                    more_counter++;
                }


                if ( ( symb == '<' )  && ( common_input_status == 0) )
                {
                    str[lenStr] = symb;
                    lenStr++;
                    str[lenStr] = '\0';
                    return str;
                }
                if ( ( symb == '<' )  && ( common_input_status != 0) )
                {
                    str[lenStr] = '\0';
                    lenStr++;
                    *status_symb = 1;
                    symboll[0] = '<';
                    return str;
                }

                if ( ( symb == ';' ) && ( common_input_status == 0)  )
                {
                    str[lenStr] = symb;
                    lenStr++;
                    str[lenStr] = '\0';
                    return str;
                }
                if ( ( symb == ';' )  && ( common_input_status != 0) )
                {
                    str[lenStr] = '\0';
                    lenStr++;
                    *status_symb = 1;
                    symboll[0] = ';';
                    return str;
                }

                if ( ( symb == ')' ) && ( common_input_status == 0)  )
                {
                    str[lenStr] = symb;
                    lenStr++;
                    str[lenStr] = '\0';
                    return str;
                }
                if ( ( symb == ')' )  && ( common_input_status != 0) )
                {
                    str[lenStr] = '\0';
                    lenStr++;
                    *status_symb = 1;
                    symboll[0] = ')';
                    return str;
                }
                if ( ( symb == '(' ) && ( common_input_status == 0)  )
                {
                    str[lenStr] = symb;
                    lenStr++;
                    str[lenStr] = '\0';
                    return str;
                }
                if ( ( symb == '(' )  && ( common_input_status != 0) )
                {
                    str[lenStr] = '\0';
                    lenStr++;
                    *status_symb = 1;
                    symboll[0] = '(';
                    return str;
                }


                if ( (symb != ';') && (symb != '>') && (symb != '<') && (symb != '|') && (symb != '&') && (more_counter + ampersand_counter + vertical_counter > 0 ) )
                {
                    str[lenStr] = '\0';
                    *status_symb = 1;
                    symboll[0] = symb;
                    return str;


                }
            }


            if ( ((symb != ';') && (symb != '>') && (symb != '<') && (symb != '|') && (symb != '&') && (symb != '"' ) ) || ( (quotes_flag) && (symb != '"' ) ) )
            {
                common_input_status++;
                str[lenStr] = symb;
                lenStr++;
            }

        }
        if ( (symb == ' ') && ( (input_flag + more_counter + ampersand_counter + vertical_counter) != 0 ) && (!quotes_flag) )
        {
            str[lenStr] = '\0';
            return str;
        }
        symb = getchar();
    }

    if (input_flag != 0)
    {
        *status_symb = 1;
        symboll[0] = EOF;
        str[lenStr] = '\0';
        return str;
    }
    *status = 1;
    return NULL;
}

/*
 
@description free list
@params  list *spisok - pointer at structure list
 
*/
list * free_list(list *spisok)
{
    list *nextNode;
    while (spisok != NULL)
    {
        nextNode = spisok->next;
        free(spisok);
        spisok = nextNode;
    }
    return spisok;
}