#include "conv_layer.h"



ConvLayer* ConvLayer_init(int n_in, float* in, int k_size,int n_out, int group,int stride,uint8_t act,float* w,float* b,float* out ){
  ConvLayer* cl;

  #ifdef VERBOSE
    //char name[10]; strcpy(name,wname); strtok(name,"_");
    printf(GRN"ConvLayer init... "CRST);
  #endif
  cl = (ConvLayer*) malloc(sizeof(ConvLayer));
  cl->state = 1;
  cl->n_in   = n_in;
  cl->in     = in;
  cl->k_size = k_size;
  cl->n_out  = n_out;
  cl->group = group;
  cl->str   = stride;
  cl->w      = w;
  cl->b      = b;
  cl->out    = out;
  cl->act    = act;

  #ifdef VERBOSE
    printf("in:%p out:%p w:%p b:%p n_in:%d n_out:%d k:%d g:%d s:%d af:%d-> %s ...",cl->in,cl->out,cl->w,cl->b,
                                      cl->n_in,cl->n_out,cl->k_size,cl->group,cl->str,cl->act,cl->state ? "active":"silent" );
    printf(GRN"ok\n"CRST);
  #endif

  return cl;
}

uint8_t ConvLayer_run(ConvLayer* cl, int in_row, int in_col, uint8_t actfunc){
    int i,j,k,t,s,lev,wcnt,gcnt;
    int ksin = in_row*in_col;
    int rsin = in_col;
    int ksout= ksin/(cl->str*cl->str);
    int rsout= rsin/(cl->str);
    int ksw = cl->k_size*cl->k_size;

    wcnt = 0;
    gcnt = 0;

    // for(i=0;i<ksout*cl->n_out;i++)
    //   cl->out[i] = 0;

    if(cl->state == 1){
      if(cl->group == 1) {
      if(actfunc == 0){


        for(lev=0;lev<cl->n_out;lev++){
          #ifdef VERBOSE
              printf("  lev: %d  \r",lev);
              fflush(stdout);
           #endif
        for (s = 0; s <(in_row); s+=cl->str)
        for (t = 0; t <(in_col); t+=cl->str) {
          for(k=0;k<cl->n_in;k++){
            for(i=s-cl->k_size/2;i<= s+ cl->k_size/2;i++)
            for(j=t-cl->k_size/2;j<= t+ cl->k_size/2;j++){
               //printf("lev%d s%d t%d k%d i%d j%d wcnt%d\n",lev,s,t,k,i,j,wcnt);
                 cl->out[lev*ksout+(s/cl->str)*rsout+(t/cl->str)] = cl->out[lev*ksout+(s/cl->str)*rsout+(t/cl->str)]
                 + cl->w[ksw*(lev*cl->n_in+k)+wcnt] * (isInside(i,j) ? cl->in[k*ksin+i*rsin+j]:0);
                wcnt++;
                }
            wcnt = 0;
          }
          cl->out[lev*ksout+(s/cl->str)*rsout+(t/cl->str)] = cl->out[lev*ksout+(s/cl->str)*rsout+(t/cl->str)] + cl->b[lev];
          }
       }

        //endof actf = 1
      }
    else if(actfunc == 1){

      for(lev=0;lev<cl->n_out;lev++){
        #ifdef VERBOSE
            printf("  lev: %d  \r",lev);
            fflush(stdout);
         #endif
       for (s = 0; s <(in_row); s+=cl->str)
       for (t = 0; t <(in_col); t+=cl->str) {
         for(k=0;k<cl->n_in;k++){
              //printf("lev%d s%d t%d k%d i%d j%d wcnt%d\n",lev,s,t,k,i,j,wcnt);
           for(i=s-cl->k_size/2;i<= s+ cl->k_size/2;i++)
           for(j=t-cl->k_size/2;j<= t+ cl->k_size/2;j++){
             cl->out[lev*ksout+(s/cl->str)*rsout+(t/cl->str)] = cl->out[lev*ksout+(s/cl->str)*rsout+(t/cl->str)]
             + cl->w[ksw*(lev*cl->n_in+k)+wcnt] * (isInside(i,j) ? cl->in[k*ksin+i*rsin+j]:0);
               wcnt++;
               }
           wcnt = 0;
         }
         cl->out[lev*ksout+(s/cl->str)*rsout+(t/cl->str)] = actfunc(cl->out[lev*ksout+(s/cl->str)*rsout+(t/cl->str)] + cl->b[lev]);
       }
      }


     }
    }else if(cl->group > 1){
      //TODO -check ConvLayer_run for group > 1
      for(gcnt = 0; gcnt<cl->group;gcnt++)
      for(lev=0;lev<cl->n_out;lev++){
       for (s = 0; s <(in_row); s+=cl->str)
       for (t = 0; t <(in_col); t+=cl->str) {
         for(k=0;k<cl->n_in;k++){
           for(i=s-cl->k_size/2;i<= s+ cl->k_size/2;i++)
           for(j=t-cl->k_size/2;j<= t+ cl->k_size/2;j++){
              //printf("lev%d s%d t%d k%d i%d j%d wcnt%d\n",lev,s,t,k,i,j,wcnt);
              //  cl->out[lev][s/cl->str][t/cl->str] += cl->w[lev+gcnt*cl->n_out][wcnt/cl->k_size][wcnt%cl->k_size]
              //                                             * (isInside(i,j) ? cl->in[k+(gcnt*cl->n_in/cl->group)][i][j]:0);
               wcnt++;
               }
           wcnt = 0;
         }
  //       cl->out[lev][s/cl->str][t/cl->str] += cl->b[lev];
       }
      }


    }

    } else if(cl->state == 0){
      cl->out = cl->in;
    }

    #ifdef VERBOSE2
      static int cnt;
      cnt++;
      printf("Conv_%d... lev:%d ",cnt,lev );
      #ifdef CHKSUM
        chksum(cl->out,in_row/cl->str,in_col/cl->str,cl->n_out);
      #endif
    #endif

  return 0;
}
