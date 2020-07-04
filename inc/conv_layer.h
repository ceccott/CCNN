#ifndef LCONV
#define LCONV

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "defines.h"

typedef struct _ConvLayer{
  float*     in;       // pointer to the input maps
  float*     out;      // pointer to the output maps
  float*     w;        // pointer to the weights
  float*     b;        // pointer to the biases

  int       n_in;      //number of input maps
  int       n_out;     //number of output maps (i.e. number of filters)
  int       k_size;    //kernel size (assuming H=W)
  int       group;     //number of different kernels (depth)
  int       str;       //stride
  uint8_t   act;       //run activation function at the end?

  uint8_t   state;    //layer can be active or not
} ConvLayer;

#include "layer_func.h"
// #include "settings.h"
// #include "defines.h"
// #include"../../debug_tools/debugTools.h"

ConvLayer* ConvLayer_init(int n_in, float* in, int k_size,int n_out, int group,int stride,uint8_t act,float* w,float* b,float* out );

uint8_t ConvLayer_run(ConvLayer* cl, int in_row, int in_col, uint8_t actfunc);


#endif
