#ifndef LIO
#define LIO

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

  typedef struct _InputData{
    float*   map;
    int      lev;
  }InputData;

  typedef struct _CatLayer{
    float*   map;
    int      h_out;
    int      w_out;
    int      n_out;
  }CatLayer;

#include "settings.h"
#include "defines.h"

// - - - IO and bin files functions - - -
  uint8_t  loadWeights(const char* fname,float* weights, int size,int lev);
  uint8_t  loadBiases(const char* fname,float* biases, int lev);
  uint8_t loadMatrix(const char* fname, float* img, int row, int col,int lev,const char ftype);

  uint8_t  loadListW(const char* listfile,char*** list,int* k, int* n_in, int* n_out);
  uint8_t  loadListB(const char* listfile,char*** list,int* n_out);


#endif
