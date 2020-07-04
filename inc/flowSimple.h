#ifndef FLOW_S
#define FLOW_S

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include "ppm.h"
#include "settings.h"
#include "defines.h"
#include "debugTools.h"
#include "layers.h"

#define MAX_L   30

int    n_el=0;
char** list_w;
char** list_b;
int   stage = 0;
int   k[MAX_L];
int   n_in[MAX_L];
int   n_out[MAX_L];
uint8_t actf[31] = {0,1,1,1,1,1,1,1,1,1,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0};
int   stride[31] = {0,2,2,2,1,2,1,2,1,2,1,1,2,2,0,1,2,2,0,1,2,2,0,1,2,2,0,1,0,0,0};
L_type ltype[31] = {INPUT,CONV,CONV,CONV,CONV,CONV,CONV,CONV,CONV,CONV,CONV
  ,CONV,DCONV,DCONV,CAT,CONV,DCONV,DCONV,CAT,CONV,DCONV,DCONV,CAT,CONV,DCONV,DCONV,CAT,CONV,ELTW,RSMP,CONV};


  uint8_t FlowSimple_init_run();
  uint8_t FlowSimple_deinit();
  uint8_t FlowSimple_run(int upto, Layer** net);

#ifdef OPT_IDX

  float** map;
  float*  in_map;
  float*  out_map;

  float** weight;
  float**   bias;
  Layer* img_in;
  Layer** net;

  uint8_t loadParams(int i){
      weight[i] = weight_allocate(k[i],n_in[i]*n_out[i]);
      loadWeights(list_w[i],weight[i],k[i],n_in[i]*n_out[i]);
      bias[i] = bias_allocate(n_out[i]);
      loadBiases(list_b[i],bias[i],n_out[i]);
    return 0;
  }

  uint8_t FlowSimple_init_run(){
    int i;
    //--> get lists
    n_el = loadListW("w_list.txt",&list_w,k,n_in,n_out);
    loadListB("b_list.txt",&list_b,n_out);

    //--> maps and layers arrays
    map = (float**) malloc(n_el*sizeof(float*));
    map[0] = map_allocate(IMG_H, IMG_W, 6);
    in_map = map[0];

    net = (Layer**) malloc(n_el*sizeof(Layer*));

    //--> allocate and load weights
    weight = (float**) malloc(n_el*sizeof(float*));
    bias   = (float**) malloc(n_el*sizeof(float*));

  //   for(i=0;i<n_el;i++){
  //    weight[i] = weight_allocate(k[i],n_in[i]*n_out[i]);
  //    loadWeights(list_w[i],weight[i],k[i],n_in[i]*n_out[i]);
  //    bias[i] = bias_allocate(n_out[i]);
  //    loadBiases(list_b[i],bias[i],n_out[i]);
  //  }

   //--> load Inputs
   loadMatrix("testFrames/in6.bin", in_map, IMG_H, IMG_W,6,'b');

   #ifdef CHKSUM
    //chksum(in_map, IMG_H, IMG_W, 6);
    #endif

   //--> init input Layers
   net[0] = Input_Layer_init("input",in_map,6,IMG_H,IMG_W);
   img_in = net[0];

   i=1;
   //contracting section
          map[i] = map_allocate(img_in->h_out/stride[i],img_in->w_out/stride[i],n_out[i]); loadParams(i);
        net[i] = Layer_init(CONV,img_in,k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
                RunLayer(net[i]);freeLayerParams(net[i]);i++;
          freeLayer(net[i-2]);
          map[i] = map_allocate(net[i-1]->h_out/stride[i],net[i-1]->w_out/stride[i],n_out[i]); loadParams(i);
        net[i] = Layer_init(CONV,net[i-1],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0); net[i]->save= 1; //2

    #ifdef M_TIME
    self_time();
     #endif
                RunLayer(net[i]);
    #ifdef M_TIME
    self_time();
     #endif
                freeLayerParams(net[i]);i++;
          freeLayer(net[i-2]);
          map[i] = map_allocate(net[i-1]->h_out/stride[i],net[i-1]->w_out/stride[i],n_out[i]); loadParams(i);
        net[i] = Layer_init(CONV,net[i-1],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
                RunLayer(net[i]);freeLayerParams(net[i]);i++;
          freeLayer(net[i-2]);
          map[i] = map_allocate(net[i-1]->h_out/stride[i],net[i-1]->w_out/stride[i],n_out[i]); loadParams(i);
        net[i] = Layer_init(CONV,net[i-1],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);net[i]->save= 1; //3_1
                RunLayer(net[i]);freeLayerParams(net[i]);i++;
          freeLayer(net[i-2]);
          map[i] = map_allocate(net[i-1]->h_out/stride[i],net[i-1]->w_out/stride[i],n_out[i]); loadParams(i);
        net[i] = Layer_init(CONV,net[i-1],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
                RunLayer(net[i]);freeLayerParams(net[i]);i++;
          freeLayer(net[i-2]);
          map[i] = map_allocate(net[i-1]->h_out/stride[i],net[i-1]->w_out/stride[i],n_out[i]); loadParams(i);
        net[i] = Layer_init(CONV,net[i-1],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);net[i]->save= 1; //4_1
                RunLayer(net[i]);freeLayerParams(net[i]);i++;
          freeLayer(net[i-2]);
          map[i] = map_allocate(net[i-1]->h_out/stride[i],net[i-1]->w_out/stride[i],n_out[i]); loadParams(i);
        net[i] = Layer_init(CONV,net[i-1],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
                RunLayer(net[i]);freeLayerParams(net[i]);i++;
          freeLayer(net[i-2]);
          map[i] = map_allocate(net[i-1]->h_out/stride[i],net[i-1]->w_out/stride[i],n_out[i]); loadParams(i);
        net[i] = Layer_init(CONV,net[i-1],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);net[i]->save= 1; //5_1
                RunLayer(net[i]);freeLayerParams(net[i]);i++;
          freeLayer(net[i-2]);
          map[i] = map_allocate(net[i-1]->h_out/stride[i],net[i-1]->w_out/stride[i],n_out[i]); loadParams(i);
        net[i] = Layer_init(CONV,net[i-1],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
                RunLayer(net[i]);freeLayerParams(net[i]);i++;
          freeLayer(net[i-2]);
          map[i] = map_allocate(net[i-1]->h_out/stride[i],net[i-1]->w_out/stride[i],n_out[i]); loadParams(i);
        net[i] = Layer_init(CONV,net[i-1],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);  //6_1
                RunLayer(net[i]);freeLayerParams(net[i]);i++;

            // expanding section
            freeLayer(net[i-2]);
            map[i] = map_allocate(net[i-1]->h_out/stride[i],net[i-1]->w_out/stride[i],n_out[i]); loadParams(i);
          net[i] = Layer_init(CONV,net[i-1],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
                  RunLayer(net[i]);freeLayerParams(net[i]);i++;

            map[i] = Cat_NewLayer(net[8],n_out[i]);
            //map[i]=map_allocate(net[i-2]->h_out*stride[i],net[i-2]->w_out*stride[i],n_out[i]);
            loadParams(i);
          net[i] = Layer_init(DCONV,net[i-2],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
                  RunLayer(net[i]);freeLayerParams(net[i]);i++;
            freeLayer(net[i-3]);

            map[i] = Cat_NewLayer(net[8],n_out[i]);
            //map[i]=map_allocate(net[i-2]->h_out*stride[i],net[i-2]->w_out*stride[i],n_out[i]);
            loadParams(i);
          net[i] = Layer_init(DCONV,net[i-2],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
                  RunLayer(net[i]);freeLayerParams(net[i]);i++;
            freeLayer(net[i-3]);

          net[i] = Layer_init(CAT,net[8], k[i], stride[i],actf[i], n_out[i], weight[i], bias[i], map[i-1], 2,net[i-2],net[i-1]);
                  RunLayer(net[i]);freeLayerParams(net[i]);i++;

          map[i] = map_allocate(net[i-1]->h_out/stride[i],net[i-1]->w_out/stride[i],n_out[i]); loadParams(i);
        net[i] = Layer_init(CONV,net[i-1],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
                RunLayer(net[i]);freeLayerParams(net[i]);i++;

          map[i] = Cat_NewLayer(net[6],n_out[i]); loadParams(i);
        net[i] = Layer_init(DCONV,net[i-2],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
                RunLayer(net[i]);freeLayerParams(net[i]);i++;

          map[i] = Cat_NewLayer(net[6],n_out[i]); loadParams(i);
        net[i] = Layer_init(DCONV,net[i-2],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
                RunLayer(net[i]);freeLayerParams(net[i]);i++;
          freeLayer(net[i-3]);

          net[i] = Layer_init(CAT,net[6], k[i], stride[i],actf[i], n_out[i], weight[i], bias[i], map[i], 2,net[i-2],net[i-1]);
                RunLayer(net[i]);freeLayerParams(net[i]);i++;
          freeLayer(net[i-5]);

          map[i] = map_allocate(net[i-1]->h_out/stride[i],net[i-1]->w_out/stride[i],n_out[i]); loadParams(i);
        net[i] = Layer_init(CONV,net[i-1],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
                RunLayer(net[i]);freeLayerParams(net[i]);i++;

          map[i] = Cat_NewLayer(net[4],n_out[i]); loadParams(i);
        net[i] = Layer_init(DCONV,net[i-2],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
                RunLayer(net[i]);freeLayerParams(net[i]);i++;

          map[i] = Cat_NewLayer(net[4],n_out[i]); loadParams(i);
        net[i] = Layer_init(DCONV,net[i-2],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0); //21
                RunLayer(net[i]);freeLayerParams(net[i]);i++;
                freeLayer(net[i-3]);

        net[i] = Layer_init(CAT,net[4], k[i], stride[i],actf[i], n_out[i], weight[i], bias[i], map[i], 2,net[i-2],net[i-1]);
                RunLayer(net[i]);freeLayerParams(net[i]);i++;
          freeLayer(net[i-5]);// net[6]->save=0;freeLayer(net[6]);

          map[i] = map_allocate(net[i-1]->h_out/stride[i],net[i-1]->w_out/stride[i],n_out[i]); loadParams(i);
        net[i] = Layer_init(CONV,net[i-1],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
                RunLayer(net[i]);freeLayerParams(net[i]);i++;

          map[i] = Cat_NewLayer(net[2],n_out[i]); loadParams(i);
        net[i] = Layer_init(DCONV,net[i-2],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
                RunLayer(net[i]);freeLayerParams(net[i]);i++;

          map[i] = Cat_NewLayer(net[2],n_out[i]); loadParams(i);
        net[i] = Layer_init(DCONV,net[i-2],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
                RunLayer(net[i]);freeLayerParams(net[i]);i++;
                freeLayer(net[i-3]);

          net[i] = Layer_init(CAT,net[2], k[i], stride[i],actf[i], n_out[i], weight[i], bias[i], map[i], 2,net[i-2],net[i-1]);
                RunLayer(net[i]);freeLayerParams(net[i]);i++;
          freeLayer(net[i-5]);// net[4]->save=0;freeLayer(net[4]);

          map[i] = map_allocate(net[i-1]->h_out/stride[i],net[i-1]->w_out/stride[i],n_out[i]); loadParams(i);
        net[i] = Layer_init(CONV,net[i-1],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
                RunLayer(net[i]);freeLayerParams(net[i]);

          freeLayer(net[i-1]);// net[2]->save=0;freeLayer(net[2]);


          out_map = map[i];

          //chksum(net[i]->out,net[i]->h_out,net[i]->w_out,net[i]->n_out);

  return 0;
  }


  uint8_t FlowSimple_deinit(){
    uint8_t err;

    return err;
  }

 #endif


// #ifndef OPT_IDX
//
//   float**** map;
//   float***  in_map;
//   float***  out_map;
//
//   float**** weight;
//   float**   bias;
//
//   Layer* img_in;
//   Layer** net;
//
//   Layer  null;
//
//       #ifdef FLOWS
//
//       uint8_t loadParams(int i){
//
//           weight[i] = weight_allocate(k[i],n_in[i]*n_out[i]);
//           loadWeights(list_w[i],weight[i],k[i],n_in[i]*n_out[i]);
//           bias[i] = bias_allocate(n_out[i]);
//           loadBiases(list_b[i],bias[i],n_out[i]);
//         return 0;
//       }
//
//   uint8_t FlowSimple_init_run(){
//           int i;
//           //--> get lists
//           n_el = loadListW("w_list.txt",&list_w,k,n_in,n_out);
//           loadListB("b_list.txt",&list_b,n_out);
//
//           //--> allocate maps and layers
//           map = (float****) malloc(n_el*sizeof(float***));
//           map[0] = map_allocate(IMG_H, IMG_W,6);
//           in_map = map[0];
//
//           net = (Layer**) malloc(n_el*sizeof(Layer*));
//
//           //--> allocate and load weights
//           weight = (float****) malloc(n_el*sizeof(float***));
//           bias   = (float**) malloc(n_el*sizeof(float*));
//
//         //   for(i=0;i<n_el;i++){
//         //    weight[i] = weight_allocate(k[i],n_in[i]*n_out[i]);
//         //    loadWeights(list_w[i],weight[i],k[i],n_in[i]*n_out[i]);
//         //    bias[i] = bias_allocate(n_out[i]);
//         //    loadBiases(list_b[i],bias[i],n_out[i]);
//         //  }
//
//           //chksum(weight[n_el-1], 0, 0,3, 3);
//
//           //--> load Inputs
//           loadMatrix("testFrames/frame10_r.txt", in_map[0], IMG_H, IMG_W,'t');
//           loadMatrix("testFrames/frame10_g.txt", in_map[1], IMG_H, IMG_W,'t');
//           loadMatrix("testFrames/frame10_b.txt", in_map[2], IMG_H, IMG_W,'t');
//           loadMatrix("testFrames/frame11_r.txt", in_map[3], IMG_H, IMG_W,'t');
//           loadMatrix("testFrames/frame11_g.txt", in_map[4], IMG_H, IMG_W,'t');
//           loadMatrix("testFrames/frame11_b.txt", in_map[5], IMG_H, IMG_W,'t');
//
//           //--> init Layers
//           net[0] = Input_Layer_init("input",in_map,6,IMG_H,IMG_W);
//           img_in = net[0];
//
//           i = 1;
//           #ifdef VERBOSE
//             printf("\nallocate run & free\n\n");
//           #endif
//
//           //contracting section
//             map[i] = map_allocate(img_in->h_out/stride[i],img_in->w_out/stride[i],n_out[i]); loadParams(i);
//           net[i] = Layer_init(CONV,img_in,k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
//                   RunLayer(net[i]);freeLayerParams(net[i]);i++;
//             freeLayer(net[i-2]);
//             map[i] = map_allocate(net[i-1]->h_out/stride[i],net[i-1]->w_out/stride[i],n_out[i]); loadParams(i);
//           net[i] = Layer_init(CONV,net[i-1],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0); net[i]->save= 1; //2
//                   RunLayer(net[i]);freeLayerParams(net[i]);i++;
//             freeLayer(net[i-2]);
//             map[i] = map_allocate(net[i-1]->h_out/stride[i],net[i-1]->w_out/stride[i],n_out[i]); loadParams(i);
//           net[i] = Layer_init(CONV,net[i-1],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
//                   RunLayer(net[i]);freeLayerParams(net[i]);i++;
//             freeLayer(net[i-2]);
//             map[i] = map_allocate(net[i-1]->h_out/stride[i],net[i-1]->w_out/stride[i],n_out[i]); loadParams(i);
//           net[i] = Layer_init(CONV,net[i-1],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);net[i]->save= 1; //3_1
//                   RunLayer(net[i]);freeLayerParams(net[i]);i++;
//             freeLayer(net[i-2]);
//             map[i] = map_allocate(net[i-1]->h_out/stride[i],net[i-1]->w_out/stride[i],n_out[i]); loadParams(i);
//           net[i] = Layer_init(CONV,net[i-1],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
//                   RunLayer(net[i]);freeLayerParams(net[i]);i++;
//             freeLayer(net[i-2]);
//             map[i] = map_allocate(net[i-1]->h_out/stride[i],net[i-1]->w_out/stride[i],n_out[i]); loadParams(i);
//           net[i] = Layer_init(CONV,net[i-1],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);net[i]->save= 1; //4_1
//                   RunLayer(net[i]);freeLayerParams(net[i]);i++;
//             freeLayer(net[i-2]);
//             map[i] = map_allocate(net[i-1]->h_out/stride[i],net[i-1]->w_out/stride[i],n_out[i]); loadParams(i);
//           net[i] = Layer_init(CONV,net[i-1],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
//                   RunLayer(net[i]);freeLayerParams(net[i]);i++;
//             freeLayer(net[i-2]);
//             map[i] = map_allocate(net[i-1]->h_out/stride[i],net[i-1]->w_out/stride[i],n_out[i]); loadParams(i);
//           net[i] = Layer_init(CONV,net[i-1],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);net[i]->save= 1; //5_1
//                   RunLayer(net[i]);freeLayerParams(net[i]);i++;
//             freeLayer(net[i-2]);
//             map[i] = map_allocate(net[i-1]->h_out/stride[i],net[i-1]->w_out/stride[i],n_out[i]); loadParams(i);
//           net[i] = Layer_init(CONV,net[i-1],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
//                   RunLayer(net[i]);freeLayerParams(net[i]);i++;
//             freeLayer(net[i-2]);
//             map[i] = map_allocate(net[i-1]->h_out/stride[i],net[i-1]->w_out/stride[i],n_out[i]); loadParams(i);
//           net[i] = Layer_init(CONV,net[i-1],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);  //6_1
//                   RunLayer(net[i]);freeLayerParams(net[i]);i++;
//
//           //expanding section
//             freeLayer(net[i-2]);
//             map[i] = map_allocate(net[i-1]->h_out/stride[i],net[i-1]->w_out/stride[i],n_out[i]); loadParams(i);
//           net[i] = Layer_init(CONV,net[i-1],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
//                   RunLayer(net[i]);freeLayerParams(net[i]);i++;
//
//             map[i] = map_allocate(net[i-2]->h_out*stride[i],net[i-2]->w_out*stride[i],n_out[i]); loadParams(i);
//           net[i] = Layer_init(DCONV,net[i-2],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
//                   RunLayer(net[i]);freeLayerParams(net[i]);i++;
//             freeLayer(net[i-3]);
//             map[i] = map_allocate(net[i-2]->h_out*stride[i],net[i-2]->w_out*stride[i],n_out[i]); loadParams(i);
//           net[i] = Layer_init(DCONV,net[i-2],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
//                   RunLayer(net[i]);freeLayerParams(net[i]);i++;
//             freeLayer(net[i-3]);
//             map[i] =(float***) malloc(n_out[i]*sizeof(float***));// printf("  N:%d map allocated ... \n",n_out[i]);
//           net[i] = Layer_init(CAT,net[8], k[i], stride[i],actf[i], n_out[i], weight[i], bias[i], map[i], 2,net[i-2],net[i-1]);
//                   RunLayer(net[i]);freeLayerParams(net[i]);i++;
//
//             map[i] = map_allocate(net[i-1]->h_out/stride[i],net[i-1]->w_out/stride[i],n_out[i]); loadParams(i);
//           net[i] = Layer_init(CONV,net[i-1],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
//                   RunLayer(net[i]);freeLayerParams(net[i]);i++;
//
//             map[i] = map_allocate(net[i-2]->h_out*stride[i],net[i-2]->w_out*stride[i],n_out[i]); loadParams(i);
//           net[i] = Layer_init(DCONV,net[i-2],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
//                   RunLayer(net[i]);freeLayerParams(net[i]);i++;
//
//             map[i] = map_allocate(net[i-2]->h_out*stride[i],net[i-2]->w_out*stride[i],n_out[i]); loadParams(i);
//           net[i] = Layer_init(DCONV,net[i-2],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
//                   RunLayer(net[i]);freeLayerParams(net[i]);i++;
//
//             freeLayer(net[i-3]);
//             map[i] =(float***) malloc(n_out[i]*sizeof(float***));// printf("  N:%d map allocated ... \n",n_out[i]);
//             net[i] = Layer_init(CAT,net[6], k[i], stride[i],actf[i], n_out[i], weight[i], bias[i], map[i], 2,net[i-2],net[i-1]);
//                   RunLayer(net[i]);freeLayerParams(net[i]);i++;
//             freeLayer(net[i-5]); //net[8]->save=0;freeLayer(net[8]);
//
//             map[i] = map_allocate(net[i-1]->h_out/stride[i],net[i-1]->w_out/stride[i],n_out[i]); loadParams(i);
//           net[i] = Layer_init(CONV,net[i-1],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
//                   RunLayer(net[i]);freeLayerParams(net[i]);i++;
//
//             map[i] = map_allocate(net[i-2]->h_out*stride[i],net[i-2]->w_out*stride[i],n_out[i]); loadParams(i);
//           net[i] = Layer_init(DCONV,net[i-2],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
//                   RunLayer(net[i]);freeLayerParams(net[i]);i++;
//
//             map[i] = map_allocate(net[i-2]->h_out*stride[i],net[i-2]->w_out*stride[i],n_out[i]); loadParams(i);
//             net[i] = Layer_init(DCONV,net[i-2],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0); //21
//                   RunLayer(net[i]);freeLayerParams(net[i]);i++;
//
//             freeLayer(net[i-3]);
//             map[i] =(float***) malloc(n_out[i]*sizeof(float***));// printf("  N:%d map allocated ... \n",n_out[i]);
//             net[i] = Layer_init(CAT,net[4], k[i], stride[i],actf[i], n_out[i], weight[i], bias[i], map[i], 2,net[i-2],net[i-1]);
//                   RunLayer(net[i]);freeLayerParams(net[i]);i++;
//             freeLayer(net[i-5]);// net[6]->save=0;freeLayer(net[6]);
//
//             map[i] = map_allocate(net[i-1]->h_out/stride[i],net[i-1]->w_out/stride[i],n_out[i]); loadParams(i);
//           net[i] = Layer_init(CONV,net[i-1],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
//                   RunLayer(net[i]);freeLayerParams(net[i]);i++;
//
//             map[i] = map_allocate(net[i-2]->h_out*stride[i],net[i-2]->w_out*stride[i],n_out[i]); loadParams(i);
//           net[i] = Layer_init(DCONV,net[i-2],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
//                   RunLayer(net[i]);freeLayerParams(net[i]);i++;
//
//             map[i] = map_allocate(net[i-2]->h_out*stride[i],net[i-2]->w_out*stride[i],n_out[i]); loadParams(i);
//           net[i] = Layer_init(DCONV,net[i-2],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
//                   RunLayer(net[i]);freeLayerParams(net[i]);i++;
//
//             freeLayer(net[i-3]);
//             map[i] =(float***) malloc(n_out[i]*sizeof(float***));// printf("  N:%d map allocated ... \n",n_out[i]);
//             net[i] = Layer_init(CAT,net[2], k[i], stride[i],actf[i], n_out[i], weight[i], bias[i], map[i], 2,net[i-2],net[i-1]);
//                   RunLayer(net[i]);freeLayerParams(net[i]);i++;
//             freeLayer(net[i-5]);// net[4]->save=0;freeLayer(net[4]);
//
//             map[i] = map_allocate(net[i-1]->h_out/stride[i],net[i-1]->w_out/stride[i],n_out[i]); loadParams(i);
//           net[i] = Layer_init(CONV,net[i-1],k[i],stride[i],actf[i],n_out[i],weight[i],bias[i],map[i],0);
//                   RunLayer(net[i]);freeLayerParams(net[i]);
//
//             freeLayer(net[i-2]);// net[2]->save=0;freeLayer(net[2]);
//
//
//             //printf("%d\n",net[i-1]->ltype);
//             out_map = map[i];
//
//             //TODO do sth
//
//
//           return 0;
//         }
//
//   uint8_t FlowSimple_deinit(){
//           uint8_t err =0;
//           int i;
//
//         #ifdef VERBOSE
//             printf("freeing arrays...");
//          #endif
//
//         free(net);
//         free(weight);
//         free(bias);
//         free(map);
//
//         #ifdef VERBOSE
//             printf("Deinit complete\n");
//           #endif
//
//           return err;
//         }
//
//     #endif
//
//  #endif

#endif
