#include <stdio.h>
#include <stdlib.h>
#include "settings.h"
#include "defines.h"
#include "layers.h"
#include "ppm.h"
//#include "simpleConv.h"
#include "flowSimple.h"

#ifdef DEBUG_TOOLS
 #include"debugTools.h"
 #include <time.h>
#endif

#ifndef DEBUG
  float test_array[10] = {100, 9, 0.3,3.4, 3.4, 8 , 0, -1, 4.456, 22};
  float*** test_weights;
  float*** read_weights;
  float*  test_bias;
  float*  read_bias;
  int size=0;
  int lev=0;
  time_t t;

#endif


float**** w;

int main(){

printf("START\n");
#ifdef M_TIME
 clock_t begin = clock();
 #endif

#ifdef DEBUG
  int i,j,k=0;
  inImg = img_allocate(388,584,255);
  readPPM("frame10.ppm", inImg);

  testMap = map_allocate(inImg->row,inImg->col,3);

  test_weights = weight_allocate(3,2);
  // read_weights = weight_allocate(3,2);
  test_bias    = bias_allocate(2);
  // read_bias    = bias_allocate(2);

  srand((unsigned) time(&t));
  for(k=0;k<2;k++)
    for(i=0;i<3;i++)        //load test weights
      for(j=0;j<3;j++){
        test_weights[k][i][j] = rand() %10; //filling with random 0-99
        test_bias[k] = -rand() %40;}
  img2map(inImg,testMap);

  #ifdef VERBOSE
    printf("storing weights and biasesI/O...\n");
  #endif
  //storeWeights("test_w",test_weights,3,2);
  //loadWeights("test_weights", read_weights,&size,&lev);
  //storeBiases("test_b",test_bias,2);
  //loadBiases("test_biases",test_bias, &size,&lev);

  #define Nin   3
  #define wlev  2
  #define inrow 16
  #define incol 16
  #define k_size 5
  #define str   1
  #define grp   1

  map1 = map_allocate(inrow,incol,Nin);
  test_weights = map_allocate(k_size,k_size,wlev);
  test_bias    = bias_allocate(wlev);
  mapOut = map_allocate(inrow,incol,wlev);
  for(i=0;i<Nin;i++)
    init2rnd(map1[i],inrow,incol,22+i);
  for(i=0;i<wlev;i++){
    init2rnd(test_weights[i],k_size,k_size,2+i);
    test_bias[i] = -rand() % 10;
  }
  Conv1 = ConvLayer_init(Nin,map1,k_size,wlev,grp,str,test_weights, test_bias, mapOut);
  ConvLayer_run(Conv1,inrow,incol,1);


  printf("\nOut Debug\n");

  mapPrint("test_weight", test_weights, 0, 0, 3, 0, 3);
  mapPrint("test_weight", test_weights, 1, 0, 3, 0, 3);
  for (i = 0; i < wlev; i++) {
    printf("%.2f\n",test_bias[i] );
  }
  printf("\n");

  mapPrint("inmap_0", map1, 0, 0, 5, 0, 5);
  mapPrint("inmap_1", map1, 1, 0, 5, 0, 5);
  mapPrint("inmap_2", map1, 2, 0, 5, 0, 5);

  mapPrint("Conv1_out_0", mapOut,1,0,16,0,16);

 #endif

#ifdef FLOWNET_C

 #endif

#ifdef S_CONV

  s_conv_init_run();

  #ifdef CONV_TEST_RGB
    weightPrint("conv1_w",conv1_w,0,0,5,0,5);
    printf("%f\n",conv1_b[2]);
    mapPrint("cat_r.txt",In_map,0,0,10,0,10);
    mapPrint("conv1_out",c1_out,0,0,10,0,10);
    printf("checksum: %.7e\n",chksum(c1_out,0,1,360,480));
  #endif

  #ifdef DCONV_TEST
    weightPrint("conv1_w",conv1_w,0,0,4,0,4);
    printf("bias:\n%f %f\n",conv1_b[0],conv1_b[1]);
    mapPrint("cat_gray.txt",In_map,0,0,10,0,10);
    mapPrint("conv1_out",c1_out,0,0,10,0,10);
    printf("checksum: %.7e\n",chksum(c1_out,0,1,720,960));
  #endif

  #ifdef DCONV_TEST_RGB
    weightPrint("conv1_w",conv1_w,0,0,4,0,4);
    printf("bias:\n%f %f\n",conv1_b[0],conv1_b[1]);
    mapPrint("cat_r.txt",In_map,0,0,10,0,10);
    mapPrint("conv1_out",c1_out,0,0,10,0,10);
    printf("checksum: %.7e\n",chksum(c1_out,0,1,720,960));
  #endif

  #ifdef CAT_ELTW_TEST_RGB
    mapPrint("cat_r.txt",In_map,0,0,2,0,10);
    mapPrint("cat_g.txt",In_map,1,0,2,0,10);
    mapPrint("cat_b.txt",In_map,2,0,2,0,10);
    mapPrint("cat_inv_r.txt",In_map2,0,0,2,0,10);
    mapPrint("cat_inv_g.txt",In_map2,1,0,2,0,10);
    mapPrint("cat_inv_b.txt",In_map2,2,0,2,0,10);
    mapPrint("conv1_out",cat_out,0,0,10,0,10);
    mapPrint("conv1_out",cat_out,3,0,10,0,10);

    EltwLayer_run(eltw, 360, 480);
    mapPrint("eltw_out",eltw_out,0,0,10,0,10);

    printf("checksum: %.10g\n",chksum(cat_out,0,5,360,480));

    // FILE* fp;
    // SaveToText(cat_out[0],"cat_r_out.txt",360,480,fp);

  #endif

 #endif

#ifdef FLOWNET_S
  FlowSimple_init_run();
  FlowSimple_deinit();
 #endif

#ifdef M_TIME
 clock_t end = clock();
 double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
 printf("time spent: %g s\n",time_spent);
 #endif
printf("STOP\n");
return 0;
}
