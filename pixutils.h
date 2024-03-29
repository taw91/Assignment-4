#ifndef __pixutils_h
#define __pixutils_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
 #define M_PI 3.14159265
#endif

#define degreesToRadians(degrees) (degrees * M_PI / 180.0)

#define FIVE_MOST_SIG_BITS 5
#define SIX_MOST_SIG_BITS 6
#define FIVE_BIT_MASK 0b11111000
#define SIX_BIT_MASK 0b11111100

typedef struct rgba{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
}rgba;

typedef struct{
	unsigned char *image;
 rgba **pixArray;
	unsigned int width;
	unsigned int height;
}pixMap;
#endif

//public constructors and destructor
pixMap *pixMap_init_filename(char *filename); //call pixMap_init and set the pixMap fields using lodepng
void pixMap_destroy (pixMap *p); //call pixMap_reset, free p and set pointer to zero

//public methods
int pixMap_write(pixMap *p,char *filename); //use lodepng to write to a file
void pixMap_rotate (pixMap *p, float theta); //rotate the pixMap
void pixMap_gray(pixMap *p); //gray out the colors of the pixMap

//ASSIGNMENT 4 FUNCTIONS
void pixMap_write_bmp16(pixMap *p,char *filename); //write 16 bit bmp to output file
void pixMap_sort(pixMap *p); //sort colors of the pixMap by total of r,g,b values

