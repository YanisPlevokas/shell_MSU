//
// Created by yanis on 19.11.2019.
//

typedef struct list
{
    char *s;
    struct list *next;
} list;
char ** list_to_mas(list *, int *);
list * add_node(list *, char *);
void print_list (list *node);
char* input_str(int *, char *, int *);
list * free_list(list *);
int spisok_count(list *);
void print_massiv(char **);
void free_massiv(char **, int);
char ** input_massiv(int *);
char *** mas_to_massives(char **, int *);
char **delete_vertical(char **, int, int*, int*);
//UNTITLED1_BIBLIOTEKA_H