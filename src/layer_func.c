#include "layer_func.h"

  float* softmax(float* in, int n_in){
    int i = 0;
    int sum = 0;
    float* out;
    out =(float*) calloc(n_in,sizeof(float));

    for(i=0; i< n_in; i++)
      sum+= expf(in[i]);

    for(i=0; i< n_in; i++)
      out[i] = expf(in[i])/sum;

    return out;
  }

  float meanpool(float* pool, uint8_t n){
    uint8_t i=0;
    float acc = pool[0];

    for (i = 1; i < n; i++)
      acc+=pool[i];

    return (acc/n);
  }

  float maxpool(float* pool, uint8_t n){
    uint8_t i=0;
    float max = pool[0];

    for (i = 1; i < n; i++)
      if(pool[i] > max)
        max = pool[i];

    return max;
  }

