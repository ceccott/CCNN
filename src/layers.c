#include "layers.h"

#ifdef VERBOSE
  #include <string.h>
#endif


// - - - General Functions - - -
Layer* Layer_init(L_type ltype,Layer* in_lyr,int k_size,int str,uint8_t act,int n_out,float* w, float* b, float* out,uint8_t n_par,...){
  Layer* lyr;
  static int cnt=1;
  va_list args;
  lyr = (Layer*) malloc(sizeof(Layer));
  lyr->ltype = ltype;
  #ifdef VERBOSE
    printf("%d-",cnt);
   #endif
  //|n_in | in* |k | n_out | g | str | w* | b* | out*|
  switch (ltype) {
    case CONV:
      lyr->conv = ConvLayer_init(in_lyr->n_out, in_lyr->out,k_size,n_out,1,str,act,w,b,out);
      lyr->out = lyr->conv->out;
      lyr->n_out = lyr->conv->n_out;
      lyr->h_out = in_lyr->h_out / lyr->conv->str;
      lyr->w_out = in_lyr->w_out / lyr->conv->str;
    break;
    case DCONV:
      lyr->dconv = DeconvLayer_init(in_lyr->n_out, in_lyr->out,k_size,n_out,str,act,w,b,out);
      lyr->out = lyr->dconv->out;
      lyr->n_out = lyr->dconv->n_out;
      lyr->h_out = in_lyr->h_out * lyr->dconv->str;
      lyr->w_out = in_lyr->w_out * lyr->dconv->str;
    break;
    case CAT:
      va_start(args, n_par);
      lyr->cat = CatLayer_init(in_lyr,n_par,args);
      lyr->out = lyr->cat->map;
      lyr->n_out = lyr->cat->n_out;
      lyr->h_out = in_lyr->h_out;
      lyr->w_out = in_lyr->w_out;
      va_end(args);
    break;
    default:
      return NULL;
  }
  lyr->save = 0;
  //lyr->n_in = in_lyr->n_out;
  #ifdef VERBOSE
    printf("  out -> H:%d W:%d N:%d\n",lyr->h_out,lyr->w_out,lyr->n_out);
   #endif
   cnt++;
  return lyr;
}

Layer* Input_Layer_init(char* name,float* map,int n_in,int h_in,int w_in){
  Layer* lyr;// = {};
    lyr = (Layer*) malloc(sizeof(Layer));
    lyr->input = (InputData*) malloc(sizeof(InputData));
    lyr->ltype = INPUT;
    lyr->input->map = map;
    lyr->input->lev = n_in;
      lyr->n_out = n_in;
    lyr->h_out = h_in;
    lyr->w_out = w_in;
    lyr->out = lyr->input->map;
    lyr->save = 0;
    #ifdef VERBOSE
      printf("InputLayer init...  %s %p\n  out -> H:%d W:%d L:%d\n",name,lyr->out,lyr->h_out,lyr->w_out,lyr->n_out);
      #endif

  return  lyr;
}

void RunLayer(Layer* lyr){
    switch (lyr->ltype) {
      case CONV:
        ConvLayer_run(lyr->conv,lyr->h_out*lyr->conv->str,lyr->w_out*lyr->conv->str,lyr->conv->act);
      break;
      case DCONV:
        DeconvLayer_run(lyr->dconv,lyr->h_out/lyr->dconv->str,lyr->w_out/lyr->dconv->str,lyr->dconv->act);
      break;
      case CAT:
        #ifdef VERBOSE2
          #ifdef CHKSUM
            printf("Cat...");
            chksum(lyr->out,lyr->h_out,lyr->w_out,lyr->n_out);
          #endif
         #endif
      break;
      default:
      break;
    }
}

uint8_t freeLayer(Layer* lyr){
  uint8_t err=0;
  if(!(lyr->save)){

    switch (lyr->ltype) {
      case CONV:
          map_deallocate(lyr->out);
          //map_deallocate(lyr->conv->w, lyr->conv->k_size, lyr->conv->k_size,lyr->conv->n_out*lyr->conv->n_in);
          //free(lyr->conv->b);
          free(lyr->conv);
      break;
      case DCONV:
        map_deallocate(lyr->out);
        //map_deallocate(lyr->dconv->w,lyr->dconv->k_size,lyr->dconv->k_size,lyr->dconv->n_out*lyr->dconv->n_in);
        //free(lyr->dconv->b);
        free(lyr->dconv);
      break;
      case CAT:
        map_deallocate(lyr->out);
        //free(lyr->cat->map);
      break;
      case INPUT:
        map_deallocate(lyr->out);
      break;
      default:
      break;
    }
    free(lyr);
  }
  return err;
}

uint8_t freeLayerParams(Layer* lyr){
  uint8_t err=0;
  switch (lyr->ltype) {
    case CONV:
        free(lyr->conv->w);
        free(lyr->conv->b);
    break;
    case DCONV:
        free(lyr->dconv->w);
        free(lyr->dconv->b);
    break;
    default:
      //do nothing
    break;
  }
  return err;
}

CatLayer* CatLayer_init(Layer* in_net1,uint8_t n_par,va_list valist){
    int j;
    CatLayer* cat;
    Layer* in_net;
    int ksin;
    #ifdef VERBOSE
      printf(MAG"CatLayer init... "CRST);
    #endif

    cat = (CatLayer*) malloc(sizeof(CatLayer));

    ksin = in_net1->h_out*in_net1->w_out;

    cat->map = in_net1->out;
    cat->n_out = in_net1->n_out;

    for (j = 0; j < n_par; j++) {
      in_net = va_arg(valist, Layer*);
      //printf("cnout: %d cmap:%p in_out_ptr:%p in_n_out:%d \n",cat->n_out,cat->map,in_net->out,in_net->n_out);
      cat->map = in_net->out-(cat->n_out*ksin);
      cat->n_out += in_net->n_out;
      //printf("cnout:%d cmap2:%p\n",cat->n_out,cat->map);
    }

    cat->h_out = in_net1->h_out;
    cat->w_out = in_net1->w_out;

     #ifdef VERBOSE
       printf(" out:%p n_out:%d ...",cat->map,cat->n_out);
       printf(MAG"ok\n"CRST);
     #endif

     Cat_NewLayer(NULL,-1); //reset cat chain

     return cat;
}

float* Cat_NewLayer(Layer* base, int in_n){
    static int stack_size=0;
    static int idx=0;
    static uint8_t first=1;
    static float* sptr= NULL;
    float* ptr = NULL;
    int ksin = base->h_out*base->w_out;

    if(in_n == -1){    //RESET
      first = 1;
      #ifdef VERBOSE
        printf("CatLayer Reset...%d\n",first);
       #endif
    }else{

    if(!first){
        ptr = (float*) realloc(sptr, stack_size + ksin*in_n*sizeof(float));
      sptr = ptr;
      ptr = ptr+idx;
      idx += ksin* in_n;
      stack_size = idx*sizeof(float);
    }
    if(first){
      idx = ksin*base->n_out;
      stack_size = idx*sizeof(float);
        ptr = (float*) realloc(base->out, stack_size + ksin*in_n*sizeof(float));
      sptr = ptr;
      ptr = ptr+idx;
      idx += ksin* in_n;
      stack_size = idx*sizeof(float);
      first = !first;
    }

    #ifdef VERBOSE
      printf("ptr:%p sptr:%p ssize:%d idx:%d\n",ptr,sptr,stack_size,idx);
     #endif
   }

    return ptr;
}

// - - - alloc and dealloc - - -

  float*   map_allocate(int row, int col, int lev){
    int n_el = row*col*lev;
    float* map = NULL;

    map = (float *) calloc(n_el,sizeof(float));

    #ifdef VERBOSE
      printf("  %d x %d x %d map allocated... \n",row,col,lev);
    #endif

      return map;
  }

  void     map_deallocate(float* map){
      free(map);

      #ifdef VERBOSE
        static int md_cnt = 0;
        printf("map #%d deallocated...\n",md_cnt);
        md_cnt++;
       #endif

  }

  float*   weight_allocate(int size, int lev){
    int n_el = size*size*lev;
    float* weight = NULL;

    weight = (float *) calloc(n_el,sizeof(float));

    // #ifdef VERBOSE
    //   printf("  %d x %d x %d kernel allocated... \n",size,size,lev);
    // #endif

    return weight;
  }
