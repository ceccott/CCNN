#ifndef DBG_TLS
#define DBG_TLS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <strings.h>
#include "layers.h"
#include "time.h"

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define CRST  "\x1B[0m"

void self_time();
void mapPrint(const char* title ,float* map,int in_row,int in_col, int in_n,int lev, int row, int off_row, int col, int off_col);
double chksum(float* map, int row, int col, int lev);
void PrintSubMatrix_int(int** matrix,int row,int row_off,int col, int col_off);
void PrintSubMatrix(float** matrix,int row,int row_off,int col, int col_off);
void SaveToText(float** matrix,char* fname,int row,int col, FILE* fp);

#endif
