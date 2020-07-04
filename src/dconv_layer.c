#include "dconv_layer.h"

DeconvLayer* DeconvLayer_init(int n_in, float* in, int k_size,int n_out,int stride,uint8_t act, float* w, float* b,float* out ){
  DeconvLayer* dcl;

  #ifdef VERBOSE
    printf(YEL"DeconvLayer init... "CRST);
  #endif

  dcl = (DeconvLayer*) malloc(sizeof(DeconvLayer));
  dcl->state  = 1;
  dcl->n_in   = n_in;
  dcl->in     = in;
  dcl->k_size = k_size;
  dcl->str = stride;
  dcl->w      = w;
  dcl->b      = b;
  dcl->n_out  = n_out;
  dcl->out    = out;
  dcl->act    = act;

  #ifdef VERBOSE
    printf("in:%p out:%p w:%p b:%p n_in:%d n_out:%d k:%d g:%d s:%d af:%d-> %s ...",dcl->in,dcl->out,dcl->w,dcl->b,
                                    dcl->n_in,dcl->n_out,dcl->k_size,1,dcl->str,dcl->act,dcl->state ? "active":"silent" );
    printf(YEL"ok\n"CRST);
  #endif

  return dcl;
}

uint8_t DeconvLayer_run(DeconvLayer* dcl, int in_row, int in_col, uint8_t actfunc){
  int i,j,k,s,t,wcnt,lev;
  int out_row = in_row * dcl->str;
  int out_col = in_col * dcl->str;
  int ksin = in_row*in_col;
  int isin = in_col;
  int isout = isin*dcl->str;
  int ksout = ksin*dcl->str*dcl->str;
  int ksw   = dcl->k_size*dcl->k_size;

  lev = 0;
  wcnt = 0;

  for(i=0;i<ksout*dcl->n_out;i++)
    dcl->out[i] = 0;

  if(dcl->state == 1){

    for(k=0;k<dcl->n_in;k++){
      for (s = 0; s <(in_row); s++)
      for (t = 0; t <(in_col); t++)
        for(lev = 0; lev < dcl->n_out;lev++){
          for(i=s*dcl->str-dcl->k_size/2+1;i<=s*dcl->str+ dcl->k_size/2;i++)
          for(j=t*dcl->str-dcl->k_size/2+1;j<= t*dcl->str+ dcl->k_size/2;j++){
          if(isInsideBox(i,j,out_row,out_col)){
            //printf("s%d t%d i%d j%d wcnt%d isIn%d %d %d\n",s,t,i,j,wcnt,isInsideBox(i,j,out_row,out_col),i/dcl->str,j/dcl->str);
           dcl->out[lev*ksout+i*isout+j] = dcl->out[lev*ksout+i*isout+j]+ dcl->w[(lev+k*dcl->n_out)*ksw+wcnt] * dcl->in[k*ksin+s*isin+t];
            }
            wcnt++;
        }
      wcnt = 0;
    }

  }
    if(actfunc==0){
      for(lev = 0; lev < dcl->n_out ;lev++){
        #ifdef VERBOSE
            printf("  lev: %d  \r",lev);
            fflush(stdout);
         #endif
      for(i=0;i<out_row;i++)
        for(j=0;j<out_col;j++)
        dcl->out[ksout*lev+isout*i+j]+= dcl->b[lev];
      }
    }
    if(actfunc==1){
      for(lev = 0; lev < dcl->n_out ;lev++){
        #ifdef VERBOSE
            printf("  lev: %d  \r",lev);
            fflush(stdout);
         #endif
      for(i=0;i<out_row;i++)
        for(j=0;j<out_col;j++)
        dcl->out[ksout*lev+isout*i+j]= actfunc(dcl->out[ksout*lev+isout*i+j]+dcl->b[lev]);
      }
    }

  }else{

    //TODO deconv state == 0
  }

  #ifdef VERBOSE2
    static int cnt_d;
    cnt_d++;
    printf("Deconv_%d... lev:%d ",cnt_d,lev );
    #ifdef CHKSUM
      chksum(dcl->out,in_row*dcl->str,in_col*dcl->str,dcl->n_out);
    #endif
  #endif

  return 0;
}
