/* FILE: A3_solutions.c is where you will code your answers for Assignment 3.
 * 
 * Each of the functions below can be considered a start for you. They have 
 * the correct specification and are set up correctly with the header file to
 * be run by the tester programs.  
 *
 * You should leave all of the code as is, especially making sure not to change
 * any return types, function name, or argument lists, as this will break
 * the automated testing. 
 *
 * Your code should only go within the sections surrounded by
 * comments like "REPLACE EVERTHING FROM HERE... TO HERE.
 *
 * The assignment document and the header A3_solutions.h should help
 * to find out how to complete and test the functions. Good luck!
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "A3_provided_functions.h"

unsigned char*
bmp_open( char* bmp_filename,        unsigned int *width, 
          unsigned int *height,      unsigned int *bits_per_pixel, 
          unsigned int *padding,     unsigned int *data_size, 
          unsigned int *data_offset                                  )
{
  unsigned char *img_data=NULL;
  // REPLACE EVERYTHING FROM HERE
  FILE *bmpfile = fopen(bmp_filename, "rb"); 

  if(bmpfile == NULL) {
    printf("Unable to open file\n");  
    }

char b, m;
fread(&b, 1, 1, bmpfile); 
fread(&m, 1, 1, bmpfile); 

unsigned int overAllFileSize; 
overAllFileSize = *data_size; 

fread(&overAllFileSize,1, sizeof(unsigned int), bmpfile);
rewind(bmpfile);  

img_data = (unsigned char*)malloc(overAllFileSize); 

fread(img_data,1,overAllFileSize,bmpfile); 

unsigned int* wp = (unsigned int*)(img_data+18); 
*width = *wp; 

unsigned int *h = (unsigned int*)(img_data+22); 
*height = *h; 
 
unsigned short int *bpp = (short*)(img_data+28); 
*bits_per_pixel = *bpp; 

unsigned int *offset = (unsigned int*)(img_data+10); 
*data_offset = *offset;

if(((*wp)*(*bits_per_pixel)/8) % 4 != 0) {
  *padding = (4 - ((*wp)*(*bits_per_pixel)/8) % 4); }


else { 
  *padding = 0; 
}

//printf( "UNIMPLEMENTED FUNCTION: bmp_open has not yet been coded. Please complete before submitting!\n");
  // TO HERE!  
  return img_data;  
}

void 
bmp_close( unsigned char **img_data )
{
  // REPLACE EVERYTHING FROM HERE
  free(*img_data); 
  *img_data = NULL; 
  //printf( "UNIMPLEMENTED FUNCTION: bmp_close has not yet been coded. Please complete before submitting!\n");
  // TO HERE!  
}

unsigned char***  
bmp_scale( unsigned char*** pixel_array, unsigned char* header_data, unsigned int header_size,
           unsigned int* width, unsigned int* height, unsigned int num_colors,
           float scale )
{
  
  unsigned char*** new_pixel_array = NULL; 
  

  unsigned int new_width = *width * scale;
    unsigned int new_height = *height * scale;
    *height = new_height;
    *width = new_width;



    new_pixel_array = (unsigned char***)malloc( sizeof(unsigned char**) * (*height));
    if( new_pixel_array == NULL ) {
      printf("Error: image failed to allocate memory");
      return NULL;
    }

    for( int row=0; row<*height; row++ ) {
      new_pixel_array[row] = (unsigned char**)malloc( sizeof(unsigned char*) * (*width) );
      for( int col=0; col<*width; col++ ) {
        new_pixel_array[row][col] = (unsigned char*)malloc( sizeof(unsigned char) * (num_colors) );
      }
    }
    for (int row=0; row<*height; row++) {
      for (int col=0; col<*width; col++) {
        for (int colour=0; colour<num_colors; colour++) {
          new_pixel_array[row][col][colour] = pixel_array[(int)(row/scale)][(int)(col/scale)][colour];

        }
      }
    }

    unsigned short int* bpp =(unsigned short int*)(header_data+28);
   
    unsigned int padd;

    int i = ((*width)*(*bpp)/8);
    if (i % 4  == 0) {
      padd = 0;
    }
    else if (i % 4  == 1) {
      padd = 3;
    }
    else if (i % 4  == 2) {
      padd = 2;
    }
    else if (i % 4  == 3) {
      padd = 1;
    }

      *(unsigned int*)(header_data+18) = *width;
      *(unsigned int*)(header_data+22) = *height;

      int pad = (4 - (*width * num_colors) % 4) % 4;
      int row_size = *width * num_colors + pad;
      int new_size = row_size * *height;

      *(unsigned int*)(header_data+34) = new_size;
      *(unsigned int*)(header_data+2) = new_size + header_size;



  //printf( "UNIMPLEMENTED FUNCTION: bmp_scale has not yet been coded. Please complete before submitting!\n");
  // TO HERE! 
  return new_pixel_array;
}         

int 
bmp_collage( char* background_image_filename,     char* foreground_image_filename, 
             char* output_collage_image_filename, int row_offset,                  
             int col_offset,                      float scale )

{
 
  unsigned char*   headerdataF;
  unsigned int     headersizeF, widthF, heightF, numcolorsF;
  unsigned char*** foreground_pixel_array = NULL;
  foreground_pixel_array = bmp_to_3D_array(foreground_image_filename, &headerdataF, &headersizeF, &widthF, &heightF, &numcolorsF);

  unsigned char*   headerdataB;
  unsigned int     headersizeB, widthB, heightB, numcolorsB;
  unsigned char*** background_pixel_array = NULL;
  background_pixel_array = bmp_to_3D_array(background_image_filename, &headerdataB, &headersizeB, &widthB, &heightB, &numcolorsB);



  if(numcolorsB != 4 || numcolorsF != 4){
    printf( "Error: Image sent to bmp_collage has wrong number of bits_per_pixel. %d.\n", (heightB) );
    return -1;
  }


  unsigned char*** collage_pixel_array = NULL;

//mallocing memory for the collage pixel array 
  collage_pixel_array = (unsigned char***)malloc(sizeof(unsigned char**) * (heightB));
  if( collage_pixel_array == NULL ){
    printf( "Error: The functon failed to allocate memory for the collage image %d.\n", (heightB) );
    return -1;
  }
  for( int row=0; row<heightB; row++ ){
    collage_pixel_array[row] = (unsigned char**)malloc( sizeof(unsigned char*) * widthB );
    for( int col=0; col<widthB; col++ ){
      collage_pixel_array[row][col] = (unsigned char*)malloc( sizeof(unsigned char) * numcolorsB );
    }
  } 


//taking the background image to the collage pixel array 
  for( int row=0; row<heightB; row++ ){
    for( int col=0; col<widthB; col++ )
        for( int color=0; color<numcolorsB; color++ ){
          collage_pixel_array[row][col][color] = background_pixel_array[row][col][color];
        }
  }

  unsigned char*** scaled_foreground_array = bmp_scale( foreground_pixel_array, headerdataF, headersizeF,
                                                   &widthF, &heightF, numcolorsF,  scale   );



  if(heightF > heightB || widthF > widthB){
    printf( "Error: The foreground image is too big, scale carefully! %d.\n", (heightB) );
    return -1;
  }

  for( int row=row_offset; row < (row_offset+heightF); row++ ){
    for( int col=col_offset; col< (col_offset+widthF); col++ )
        for( int color=0; color<numcolorsB; color++ ){
          if(scaled_foreground_array[row-row_offset][col-col_offset][0] != 0){
            collage_pixel_array[row][col][color] = scaled_foreground_array[row-row_offset][col-col_offset][color];
          } else{
            continue;
          }
        }
  }

  bmp_from_3D_array(output_collage_image_filename, headerdataB, headersizeB, collage_pixel_array, widthB, heightB, numcolorsB );

  return 0;
}

