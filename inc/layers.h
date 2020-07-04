#ifndef LAYERS
#define LAYERS

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "ppm.h"
#include "settings.h"
#include "defines.h"
#include "debugTools.h"

#include "conv_layer.h"
#include "dconv_layer.h"
#include "eltw_layer.h"
#include "io_layer.h"
#include "layer_func.h"

typedef enum{CONV=1,POOL=5,DCONV=2,ELTW=3,FCON=9,CORR=7,INPUT=0,CAT=4,RSMP=7}L_type;

// - - - Data Structures - - -
typedef struct _Layer{
    union{
        ConvLayer*   conv;
        DeconvLayer* dconv;
        EltwLayer*   eltw;
        InputData*   input;
        CatLayer*    cat;
    };
    L_type ltype;
    int     w_out;
    int     h_out;
    int     n_out;
    float*  out;
    uint8_t save;
}Layer;


// - - - general functions - - -
Layer* Layer_init(L_type ltype,Layer* in_lyr,int k_size,int str,uint8_t act,int n_out,float* w, float* b, float* out,uint8_t n_par,...);
Layer* Input_Layer_init(char* name,float* map,int n_in,int h_in,int w_in);
uint8_t freeLayer(Layer* lyr);
uint8_t freeLayerParams(Layer* lyr);
void RunLayer(Layer* lyr);

CatLayer* CatLayer_init(Layer* in_net1,uint8_t n_par,va_list valist);
float* Cat_NewLayer(Layer* base, int in_n);

//- - - allocation and deallocation - - -
float*   map_allocate(int row, int col, int lev);
void     map_deallocate(float* map);
float*   weight_allocate(int size, int lev);
float*   bias_allocate(int lev);


#endif
