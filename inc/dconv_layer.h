#ifndef LDCON
#define LDCON

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "defines.h"

typedef struct _DeconvLayer{
  int       n_in;     //number of input maps
  int       n_out;    //number of output maps
  int       k_size;   //kernel size (assuming H=W)
  int       str;
  uint8_t   act;       //run activation function at the end?


  uint8_t   state;    //layer can be active or not

  float*     w;        // pointer to the weights
  float*     b;        // pointer to the biases
  float*     in;       // pointer to the input maps
  float*     out;      // pointer to the output maps
} DeconvLayer;

#include "layer_func.h"

DeconvLayer* DeconvLayer_init(int n_in, float* in, int k_size,int n_out,int stride,uint8_t act, float* w, float* b,float* out );
uint8_t DeconvLayer_run(DeconvLayer* dcl, int in_row, int in_col, uint8_t actfunc);

#endif
