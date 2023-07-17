#include "parse.h"

int isVertexes(char *str) { return (str[0] == 'v' && str[1] == 0) ? 1 : 0; }

int isEdges(char *str) { return (*str == 'f' && *(str + 1) == 0) ? 1 : 0; }

void scanVertexes(FILE *obj, data_t *data, int countV) {
  char str[255] = {0};
  double num = 0.;
  for (int i = 0; i < 3; i++) {
    fscanf(obj, "%s", str);
    sscanf(str, "%lf", &num);
    data->vertexes[countV][i] = num;
  }
}

void scanEdges(FILE *obj, data_t *data, int countF) {
  char str[255] = {0};
  int num = 0.;
  for (int i = 0; i < data->numSides[countF]; i++) {
    fscanf(obj, "%s", str);
    sscanf(str, "%d", &num);
    data->edges[countF][i] = num;
    if (i != 0) data->numEdges += 1;
  }
}

void callocStruct(data_t *data) {
  data->vertexes = (double **)calloc(data->numVertex + 10, sizeof(double *));
  if (data->numVertex > 1) {
    for (int i = 0; i < data->numVertex + 10; i++)
      data->vertexes[i] = (double *)calloc(3, sizeof(double));
  }
  data->edges = (int **)calloc(data->allSides, sizeof(int *));
  if (data->allSides > 1) {
    for (int i = 0; i < data->allSides; i++)
      data->edges[i] = (int *)calloc(data->numSides[i], sizeof(int));
  }
}

void structFree(data_t *data) {
  for (int i = 0; i < data->numVertex + 10; i++) free(data->vertexes[i]);
  free(data->vertexes);
  for (int i = 0; i < data->allSides; i++) free(data->edges[i]);
  free(data->edges);
  free(data->numSides);
}

int readFile(char const file_name[], data_t *data) {
  int error = 0;
  FILE *obj = fopen(file_name, "r");
  if (obj == NULL) {
    error = 1;
  } else {
    int flag = 0;
    data->numSides = (int *)calloc(1, sizeof(int));
    data->allSides = 0;
    data->numVertex = 0;
    while (!feof(obj)) {
      char str[255];
      if (!flag && data->numVertex != 0) *str = 0;
      if (error) break;
      if (flag == 0) fscanf(obj, "%s", str);
      if (isVertexes(str)) {
        data->numVertex += 1;
        error = (data->numVertex > pow(10, 6)) ? 1 : 0;
      } else if (isEdges(str)) {
        flag = 0;
        data->numSides =
            (int *)realloc(data->numSides, (data->allSides + 1) * sizeof(int));
        data->numSides[data->allSides] = 0;
        while (!feof(obj)) {
          fscanf(obj, "%s", str);
          if (!(*str >= '0' && *str <= '9')) {
            if (*str == 'f') flag = 1;
            break;
          } else {
            data->numSides[data->allSides] += 1;
          }
          if (feof(obj)) {
            data->numSides[data->allSides] -= 1;
            break;
          }
        }
        data->allSides += 1;
      }
    }
    fclose(obj);
  }
  return error;
}

int scanFile(char const file_name[], data_t *data) {
  int error = 0;
  FILE *obj = fopen(file_name, "r");
  if (obj == NULL) {
    error = 1;
  } else {
    int countV = 0, countF = 0;
    data->numEdges = 0;
    while (!feof(obj)) {
      char str[255] = {0};
      fscanf(obj, "%s", str);
      if (isVertexes(str)) {
        scanVertexes(obj, data, countV);
        countV++;
      } else if (isEdges(str)) {
        scanEdges(obj, data, countF);
        countF++;
      }
    }
  }
  fclose(obj);

  return error;
}

void shifting(data_t *data, double shiftX, double shiftY, double shiftZ) {
  for (int i = 0; i < data->numVertex; i++) {
    data->vertexes[i][Ax] += shiftX;
    data->vertexes[i][Ay] += shiftY;
    data->vertexes[i][Az] += shiftZ;
  }
}

void scaling(data_t *data, data_t *scaleble, double scale) {
  for (int i = 0; i < data->numVertex; i++) {
    for (int y = 0; y < 3; y++) {
      scaleble->vertexes[i][y] *= scale;
    }
  }
}

void rotation(data_t *data, data_t *rotated, int axis, double angle) {
  angle = angle * (PI / 180);
  for (int i = 0; i < data->numVertex; i++) {
    double x = data->vertexes[i][Ax];
    double y = data->vertexes[i][Ay];
    double z = data->vertexes[i][Az];

    switch (axis) {
      case Ax:
        rotated->vertexes[i][Ay] = y * cos(angle) + z * sin(angle);
        rotated->vertexes[i][Az] = y * sin(angle) - z * cos(angle);
        break;
      case Ay:
        rotated->vertexes[i][Ax] = x * cos(angle) + z * sin(angle);
        rotated->vertexes[i][Az] = -1 * x * sin(angle) + z * cos(angle);

        break;
      case Az:
        rotated->vertexes[i][Ax] = x * cos(angle) + y * sin(angle);
        rotated->vertexes[i][Ay] = x * sin(angle) - y * cos(angle);
        break;
    }
  }
}

void dup(data_t *data, data_t *mod) {
  mod->allSides = data->allSides;
  mod->numVertex = data->numVertex;
  mod->numEdges = data->numEdges;
  mod->numSides = (int *)calloc(mod->allSides, sizeof(int));
  for (int i = 0; i < mod->allSides; i++) {
    mod->numSides[i] = data->numSides[i];
  }
  callocStruct(mod);
  for (int i = 0; i < mod->numVertex; i++) {
    for (int y = 0; y < 3; y++) {
      mod->vertexes[i][y] = data->vertexes[i][y];
    }
  }
  for (int i = 0; i < mod->allSides; i++) {
    for (int y = 0; y < data->numSides[i]; y++) {
      mod->edges[i][y] = data->edges[i][y];
    }
  }
}

// void printstruc(data_t *print) {
// printf("кол-во точек: %d\n", print->numVertex);
// printf("кол-во сторон: %d\n", print->allSides);
// printf("кол-во ребер (всего): %d\n", print->numEdges);
// for (int i = 0; i < print->allSides; i++) {
//   printf("кол-во ребер на стороне [%d]: %d\n", i, print->numSides[i]);
// }
// printf("координаты точек:\n");
// for (int i = 0; i < print->numVertex; i++) {
//   for (int y = 0; y < 3; y++) {
//     printf("%lf\t", print->vertexes[i][y]);
//   }
//   printf("\n");
// }
// printf("ребра:\n");
// for (int i = 0; i < print->allSides; i++) {
//   for (int y = 0; y < print->numSides[i]; y++) {
//     printf("%d\t", print->edges[i][y]);
//   }
//   printf("\n");
// }
// }
