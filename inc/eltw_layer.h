#ifndef LELTW
#define LELTW

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

typedef enum{SUM,DIF,DIV,MUL}OP;

typedef struct _EltwLayer{
  int       n_in;
  int       n_out;
  float    coeff1;
  float    coeff2;
  float*    in1;
  float*    in2;
  float*    out;
  OP        op;
}EltwLayer;

#endif
