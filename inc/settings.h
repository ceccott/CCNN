#ifndef SETTINGS
#define SETTINGS

#define OPT_IDX

#include "layers.h"
#include "defines.h"

//#define DEBUG
#define DEBUG_TOOLS
#define VERBOSE
//#define VERBOSE2
#define CHKSUM
#define M_TIME

//#define FLOWNET_C
#define FLOWNET_S
//#define S_CONV
#define FLOWS

//#define RND_weights_init

#define IN_IMG_H            384 //388
#define IN_IMG_W            512 //584

#define IMG_H               384//360//64//384 //388
#define IMG_W               512//480//64//512 //584

#define IMG_EXP_H           (IMG_H >> 6)    //map size at the end of the contractive section
#define IMG_EXP_W           (IMG_W >> 6)

#define MAP_H               (IMG_H >> stage)
#define MAP_W               (IMG_W >> stage)

#define pad(k)              (k-1)/2
#define out_size(w,k,p,s)   (w-f+2*p)/s +1

#define actfunc(x)          relu(x)        //tanhyp(x)
#define poolfunc(x,y)       maxpool(x,y)   //meanpool(x,y)

#define RELU_NS             0.1

#endif
