#ifndef LFUNC
#define LFUNC

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "settings.h"
#include "defines.h"
#include "debugTools.h"

//#include "layers.h"

// - - - math operators - - -
  #define isInside(x,y)            ((x>=0) && (x<in_row) && (y>=0) && (y<in_col))
  #define isInsideBox(x,y,a,b)     ((x>=0) && (x<a) && (y>=0) && (y<b))

  float* softmax(float* in, int n_in);
  float meanpool(float* pool, uint8_t n);
  float maxpool(float* pool, uint8_t n);

  inline float relu(float x){
    if(x >= 0)
      return x;
    else
      return x*RELU_NS;
    return 0;
  }

  inline float tanhyp(float x){
    if(x > 0)
      return tanhf(x);
    else
      return 0;
  }


#endif
