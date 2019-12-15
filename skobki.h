/*
@description Находит адрес конца текущего скобочного выражения
@params char **massiv - массив лексем, int localStart - точка локального старта
*/
int find_local_end(char **, int );



/*
@description Находим режим, в котором запускается скобка
@params char **massiv - массив лексем, int endPoint - позиция закрывающей соответствующей скобки
*/

int find_mode_of_skobka(char **, int );





/*

@description Возвращает первоначальный ввод-вывод
@params int saveOfStandardInput - сохраненный дескриптор стандартного ввода, int saveOfStandardOutput - сохраненный дескриптор стандартного вывода

*/

void saveInOutPUT(int , int );



/*

@definition Осуществляет перенаправления в начале строки, возвращает позицию после всех перенаправлений - наша рабочая начальная позиция
@params char **massiv - Массив лексем

*/


int redirect(char **, int );


/*

@description Рекурсивная функция для работы со скобками, возвращает статус завершения процесса внутри скобок
@params char **massiv - массив лексем, int localStart - адрес начальной точки, с которой мы начинаем работу,
int localEnd - конец для данного выражения внутри скобок, int GlobalEnd - конец всей строки (для очистки памяти)

*/

int recursive_skobki(char **, int , int , int );




