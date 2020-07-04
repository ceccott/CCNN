#include "ppm.h"

uint8_t img2map(PPMImg* img, float*** map){
  int i,j = 0;

  for (i = 0; i < img->row; i++)
    for (j = 0; j < img->col; j++){
      map[0][i][j] = img->r[i][j];
      map[1][i][j] = img->g[i][j];
      map[2][i][j] = img->b[i][j];
    }
  return 0;
}

PPMImg* img_allocate(int row, int col, int maxrgb){
  int i=0;
  PPMImg* img;
  img = (PPMImg*) malloc(sizeof(PPMImg));

  img->row = row;
  img->col = col;
  img->max = maxrgb;

  img->r = (float**) malloc(sizeof(float*)*img->row);
  img->g = (float**) malloc(sizeof(float*)*img->row);
  img->b = (float**) malloc(sizeof(float*)*img->row);

  for(i=0;i<img->row;i++){
    img->r[i] = (float*) malloc(sizeof(float)*img->col);
    img->g[i] = (float*) malloc(sizeof(float)*img->col);
    img->b[i] = (float*) malloc(sizeof(float)*img->col);
}
  return img;
}

void img_deallocate(PPMImg* img){
int i=0;
  for(i=0;i<img->row;i++){
    free(img->r[i]);
    free(img->g[i]);
    free(img->b[i]);
  }
  free(img->r);
  free(img->g);
  free(img->b);

  free(img);
}

uint8_t readPPM(const char *filename,PPMImg* img){
         char buff[16];
        uint8_t r,g,b = 0;
         FILE *fp;
         int i,j;
         //open PPM file for reading
         fp = fopen(filename, "rb");
         if (!fp) {
              fprintf(stderr, "Unable to open file '%s'\n", filename);
              exit(1);
         }
         //read image format
         if (!fgets(buff, sizeof(buff), fp)) {
              perror(filename);
              exit(1);
         }

    //check the image format
    if (buff[0] != 'P' || buff[1] != '6') {
         fprintf(stderr, "Invalid image format (must be 'P6')\n");
         exit(1);
    }
    //read image size information
    if (fscanf(fp, "%d %d",&img->col, &img->row) != 2) {
         fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
         exit(1);
    }

    //read rgb component
    if (fscanf(fp, "%d", &img->max) != 1) {
         fprintf(stderr, "Invalid rgb component (error loading '%s')\n", filename);
         exit(1);
    }

    while (fgetc(fp) != '\n') ;

    //read pixel data from file
    for (i = 0; i < img->row; ++i){
            for (j = 0; j < img->col; ++j){
              //fscanf(fp,"%c %c %c ",&r,&g,&b);
              //printf("%u %u %u - %d %d\n",r,g,b,i,j);
              r= fgetc(fp);
              g= fgetc(fp);
              b= fgetc(fp);
              img->r[i][j] = (float) r;
              img->g[i][j] = (float) g;
              img->b[i][j] = (float) b;
            }
    }
    fclose(fp);
    return 0;
}

uint8_t loadFrames(const char *frame1,const char *frame2,float*** cat_map){
  char buff[16];
 uint8_t r,g,b = 0;
  FILE *fp;
  int i,j;
  int img_col,img_row,img_max;
  //open PPM file for reading
  fp = fopen(frame1, "rb");
  //read image format
  fgets(buff, sizeof(buff), fp);
  //read image size information
  fscanf(fp, "%d %d",&img_col,&img_row);
  //read rgb component
  fscanf(fp, "%d", &img_max);
  while (fgetc(fp) != '\n') ;
  //read pixel data from file
  for (i = 0; i < img_row; ++i){
     for (j = 0; j < img_col; ++j){
       r= fgetc(fp);
       g= fgetc(fp);
       b= fgetc(fp);
       cat_map[0][i][j] = (float) r;
       cat_map[1][i][j] = (float) g;
       cat_map[2][i][j] = (float) b;
     }
  }
  fclose(fp);

  //open PPM file for reading
  fp = fopen(frame2, "rb");
  //read image format
  fgets(buff, sizeof(buff), fp);
  //read image size information
  fscanf(fp, "%d %d",&img_col,&img_row);
  //read rgb component
  fscanf(fp, "%d", &img_max);
  while (fgetc(fp) != '\n') ;
  //read pixel data from file
  for (i = 0; i < img_row; ++i){
     for (j = 0; j < img_col; ++j){
       r= fgetc(fp);
       g= fgetc(fp);
       b= fgetc(fp);
       cat_map[3][i][j] = (float) r;
       cat_map[4][i][j] = (float) g;
       cat_map[5][i][j] = (float) b;
     }
  }
  fclose(fp);

  return 0;
}

uint8_t writePPM(const char *filename, PPMImg* img){
    FILE *fp;
    int i,j=0;
    //open file for output
    fp = fopen(filename, "wb");
    if (!fp) {
         fprintf(stderr, "Unable to open file '%s'\n", filename);
         exit(1);
    }

    //write the header file
    //image format
    fprintf(fp, "P6\n");

    //image size
    fprintf(fp, "%d %d\n",img->col,img->row);

    // rgb component depth
    fprintf(fp, "%d\n",img->max);

    // pixel data
    for (i = 0; i < img->row; ++i){
            for (j = 0; j < img->col; ++j){
              //fprintf(fp,"%c%c%c ",(int)img->r[i][j],(int)img->g[i][j],(int)img->b[i][j]);
              fputc((char)img->r[i][j],fp);
              fputc((char)img->g[i][j],fp);
              fputc((char)img->b[i][j],fp);
            }
    }
    fclose(fp);

    return 0;
}

void SkipComments(FILE *fp)
{
    int ch;
    char line[100];

    while ((ch = fgetc(fp)) != EOF && isspace(ch));

    if (ch == '#') {
        fgets(line, sizeof(line), fp);
        SkipComments(fp);
    } else
        fseek(fp, -1, SEEK_CUR);
}
