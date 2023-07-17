#ifndef PARSE_H
#define PARSE_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Ax 0
#define Ay 1
#define Az 2

#define PI 3.14159265358979323846

/*Объявляется структура
Считывается файл (readFile)
Выделяется память (callocStruct)
Файл читается повторно (scanFile)
Очищается память (structFree)*/

/*Структура для хранения информации об объекте*/
typedef struct data {
  int numVertex;      // кол-во вершин
  int allSides;       // кол-во сторон
  int *numSides;      // массив сторон
  double **vertexes;  // массив координат вершин [x][y][z]
  int **edges;        // массив граней
  int numEdges;       // кол-во граней
} data_t;

/*Первоначально считывание файла, для определеия размеров массивов
В случае не нахождения файла возвращает 1.*/
int readFile(char const file_name[], data_t *data);

/*Проверка вершина или нет.
Если да, то возвращает 1.*/
int isVertexes(char *str);

/*Проверка грань или нет.
Если да, то возвращает 1.*/
int isEdges(char *str);

/*Выделяет память для массивов.*/
void callocStruct(data_t *data);

/*Повторно читает файл, для записи массивов
В случае не нахождения файла возвращает 1.*/
int scanFile(char const file_name[], data_t *data);

/*Считывает строку верши и записывает в массив.*/
void scanVertexes(FILE *obj, data_t *data, int countV);

/*Считывает строку и записывает в массив.*/
void scanEdges(FILE *obj, data_t *data, int countF);

/*Сдвиг от начальной точки на указанное расстояние по определенной оси.*/
void shifting(data_t *data, double shiftX, double shiftY, double shiftZ);

/*Изменение масштаба от изначального.*/
void scaling(data_t *data, data_t *scaleble, double scale);

/*Поворот от начальной точки на указанный угол по какой-то 1 из осей
Угол указывается в градусы и в функции переводится в радианы.*/
void rotation(data_t *data, data_t *rotated, int axis, double angle);

/*Создает такую же структуру и заполняет данными, для дальнейших изменений над
 * ней.*/
void dup(data_t *data, data_t *mod);

/*Вывод данных в структуре.*/
// void printstruc(data_t *print);

/*Очищает память.*/
void structFree(data_t *data);

#endif  // PARSE_H
