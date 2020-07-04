#ifndef SCONV_H
#define SCONV_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include "ppm.h"
#include "settings.h"
#include "defines.h"

//#define CONV_TEST
//#define CONV_TEST_RGB
//#define DCONV_TEST
//#define DCONV_TEST_RGB
#define CAT_ELTW_TEST_RGB

PPMImg*       In_img;
float***      In_map;
float***      In_map2;
float***      c1_out;
float***      cat_out;
float***      eltw_out;

ConvLayer*     conv1;
DeconvLayer*  dconv1;
EltwLayer*      eltw;


float*** conv1_w;
float*   conv1_b;


uint8_t s_conv_init_run(){
    uint8_t err=0;
    int im_row = 360;
    int im_col = 480;

    printf("Sconv_init\n");

  #ifdef CONV_TEST_RGB
    conv1_w = weight_allocate(5,3*2);
    conv1_b = bias_allocate(2);
    In_map  = map_allocate(im_row,im_col,3);
    c1_out  = map_allocate(im_row,im_col,2);
    loadWeights("testNetParams/conv1_w.bin",conv1_w,5,3*2);
    loadBiases("testNetParams/conv1_b.bin",conv1_b,2);
    // loadMatrix("cat_gray.txt", In_map, im_row, im_col, 1, 't');
    loadMatrix("testFrames/cat_r.txt", In_map[0], im_row, im_col,'t');
    loadMatrix("testFrames/cat_g.txt", In_map[1], im_row, im_col,'t');
    loadMatrix("testFrames/cat_b.txt", In_map[2], im_row, im_col,'t');

    conv1 = ConvLayer_init(3, In_map,5,2,1,2,conv1_w, conv1_b,c1_out);
    ConvLayer_run(conv1,im_row,im_col,1);
  #endif

  #ifdef DCONV_TEST
    conv1_w = weight_allocate(4,2);
    conv1_b = bias_allocate(2);
    In_map  = map_allocate(im_row,im_col,3);
    c1_out  = map_allocate(im_row*2,im_col*2,2);
    loadWeights("testNetParams/Dconv1_w.bin",conv1_w,4,2);
    loadBiases("testNetParams/Dconv1_b.bin",conv1_b,2);

    loadMatrix("cat_gray.txt", In_map[0], im_row, im_col,'t');

    // conv1 = ConvLayer_init(3, In_map,5,2,1,2,conv1_w, conv1_b,c1_out);
    dconv1 = DeconvLayer_init(1, In_map, 4, 2, 2, conv1_w, conv1_b, c1_out);
    DeconvLayer_run(dconv1,im_row,im_col,0);
  #endif

  #ifdef DCONV_TEST_RGB
    conv1_w = weight_allocate(4,2*3);
    conv1_b = bias_allocate(2);
    In_map  = map_allocate(im_row,im_col,3);
    c1_out  = map_allocate(im_row*2,im_col*2,2);
    loadWeights("testNetParams/Dconv1_w.bin",conv1_w,4,2*3);
    loadBiases("testNetParams/Dconv1_b.bin",conv1_b,2);

    loadMatrix("testFrames/cat_r.txt", In_map[0], im_row, im_col,'t');
    loadMatrix("testFrames/cat_g.txt", In_map[1], im_row, im_col,'t');
    loadMatrix("testFrames/cat_b.txt", In_map[2], im_row, im_col,'t');

    // conv1 = ConvLayer_init(3, In_map,5,2,1,2,conv1_w, conv1_b,c1_out);
    dconv1 = DeconvLayer_init(3, In_map, 4, 2, 2, conv1_w, conv1_b, c1_out);
    DeconvLayer_run(dconv1,im_row,im_col,1);
  #endif

  #ifdef CAT_ELTW_TEST_RGB
    int n_out=0;
    In_map  = map_allocate(im_row,im_col,3);
    In_map2 = map_allocate(im_row,im_col,3);
    cat_out = (float***) malloc(2*3*sizeof(float**));
    eltw_out = map_allocate(im_row, im_col, 3);

    eltw = EltwLayer_init(3,In_map,NULL,20,0,SUM,eltw_out);

    loadMatrix("testFrames/cat_r.txt", In_map[0], im_row, im_col,'t');
    loadMatrix("testFrames/cat_g.txt", In_map[1], im_row, im_col,'t');
    loadMatrix("testFrames/cat_b.txt", In_map[2], im_row, im_col,'t');

    loadMatrix("testFrames/cat_inv_r.txt", In_map2[0], im_row, im_col,'t');
    loadMatrix("testFrames/cat_inv_g.txt", In_map2[1], im_row, im_col,'t');
    loadMatrix("testFrames/cat_inv_b.txt", In_map2[2], im_row, im_col,'t');

    map_concat(cat_out, &n_out,4,In_map,3,In_map2,3);

  #endif

return err;
}




#endif
