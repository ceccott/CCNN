#ifndef PPM
#define PPM

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

typedef struct _PPMImg{
     int row;
     int col;
     int max;

     float** r;
     float** g;
     float** b;
} PPMImg;

typedef struct _PPMImg_int{
     int row;
     int col;
     int max;

     int** r;
     int** g;
     int** b;
} PPMImg_int;

uint8_t readPPM(const char *filename, PPMImg* dest);
uint8_t writePPM(const char *filename, PPMImg* img);
uint8_t loadFrames(const char *frame1,const char *frame2,float*** cat_map);
PPMImg* img_allocate(int row, int col,int maxrgb);
void img_deallocate(PPMImg* img);
uint8_t img2map(PPMImg* img,float*** map);


#endif
