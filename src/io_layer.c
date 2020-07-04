#include "io_layer.h"

// - - - I/O binary functions
uint8_t loadBiases(const char* fname,float* biases,int lev){
  FILE *fp;
  if(!strcmp(fname,"NONE"))
    return 1;
  fp = fopen (fname,"rb");
  if (fp!=NULL)
  {
    fread(biases,lev,sizeof(float),fp);
    fclose (fp);
  }else{
    #ifdef VERBOSE
      printf("Error reading .bin file\n");
    #endif
  }
    // for(k=0;k<*lev;k++)
    //   for(i=0;i<*size;i++)
    //       printf("%f\n",biases[k][i]);

  return 0;
}
uint8_t loadWeights(const char* fname,float* weights, int size,int lev){
  FILE *fp;
  int cnt = 0;
  int n_el = size*size*lev;


  if(!strcmp(fname,"NONE"))
    return 1;

  fp = fopen (fname,"rb");
  if (fp!=NULL)
  {
    cnt = fread(weights,n_el,sizeof(float),fp);
    fclose (fp);
  }else
  {
  #ifdef VERBOSE
    printf("Error reading .bin file\n");
  #endif
  }

  #ifdef VERBOSE
    printf("%d x %d x %d = %d weights loaded... \n",size,size,lev,n_el);
  #endif
  //   for(i=0;i<size;i++)
  // for(k=0;k<lev;k++)
  //     for(j=0;j<size;j++)
  //       printf("%f\n",weights[k][i][j]);
  return 0;
}

uint8_t loadMatrix(const char* fname, float* img, int row, int col,int lev,const char ftype){
  FILE *fp;
  int i,j;
  int cnt=0;

  if(ftype == 't'){
    fp = fopen (fname,"r");
    if (fp!=NULL)
    {
        for (i = 0; i < row; i++)
          for (j = 0; j < col; j++){
          if(j == col-1)
            fscanf(fp,"%f\n",&img[cnt]);
          else
            fscanf(fp,"%f ",&img[cnt]);
            cnt++;
          }
      fclose (fp);
    }

  }else if(ftype == 'b'){

    fp = fopen (fname,"r");
    if (fp!=NULL)
    {

    int n_el = row*col*lev;

    fread(img,n_el,sizeof(float),fp);

    }
  }else return 1;


  return 0;
}

uint8_t  loadListW(const char* listfile,char*** list,int* k, int* n_in, int* n_out){
    FILE *fp;
    char* line = NULL;
    int i=0;
    size_t len;
    int n_el = 0;

    line = (char*) calloc(40,sizeof(char));

    fp = fopen (listfile,"r");
      if (fp==NULL)
        printf("error opening the file\n");

    while ((getline(&line, &len, fp)) != -1)
      n_el++;

    *list = (char**) malloc((n_el)*sizeof(char*));

    for (i = 0; i < n_el; i++)
      (*list)[i] = (char*) malloc(50*sizeof(char));


      rewind(fp);
    //fseek(fp,0,SEEK_SET);
    i=0;
    if (fp!=NULL)
    {
      while ((getline(&line, &len, fp)) != -1){
        sscanf(line,"%s %d %d %d",(*list)[i],&k[i],&n_in[i],&n_out[i]);
        i++;
      }
      fclose (fp);
    }

  return i;
}
uint8_t  loadListB(const char* listfile,char*** list,int* n_out){
    FILE *fp;
    char* line = NULL;
    int i=0;
    size_t len;
    int n_el = 0;

    line = (char*) malloc(sizeof(char)*40);

    fp = fopen (listfile,"r");
      if (fp==NULL)
        printf("error opening the file\n");

    while ((getline(&line, &len, fp)) != -1)
      n_el++;

    *list = (char**) malloc((n_el)*sizeof(char*));

    for (i = 0; i < n_el; i++)
      (*list)[i] = (char*) malloc(50*sizeof(char));


      rewind(fp);
    //fseek(fp,0,SEEK_SET);
    i=0;
    if (fp!=NULL)
    {
      while ((getline(&line, &len, fp)) != -1){
        sscanf(line,"%s %d",(*list)[i],&n_out[i]);
        i++;
      }
      fclose (fp);
    }

  return i;
}
