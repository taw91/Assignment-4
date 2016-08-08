#include "pixutils.h"
#include "lodepng.h"
#include "bmp.h"

//private methods
static int pixMap_cmp(const void *a, const void *b);
static pixMap *pixMap_init(); //allocate memory for pixMap and set variables to zero
static void pixMap_reset();  //free the allocated memoray and set to zero but do not free memory for pixMap
static void pixMap_copy(pixMap *dest,pixMap *source);
static int pixMap_read(pixMap *p,char *filename);

static pixMap* pixMap_init(){
	pixMap *p;
	if(!(p=malloc(sizeof(pixMap))))exit(EXIT_FAILURE);
	p->image=0;
	p->pixArray=0;
	p->width=0;
	p->height=0;
	return p;
}

pixMap* pixMap_init_filename(char *filename){
	pixMap *p=pixMap_init();
 pixMap_read(p,filename);
 if(!(p->pixArray=malloc(p->height*sizeof(rgba*))))exit(EXIT_FAILURE);
 p->pixArray[0]=(rgba*)p->image;
 for (int i=1;i<p->height;i++)
  p->pixArray[i]=p->pixArray[i-1]+p->width;
 return p;
}
static int pixMap_read(pixMap *p,char *filename){
	int error;
 if((error=lodepng_decode32_file(&(p->image), &(p->width), &(p->height),filename))){
  fprintf(stderr,"error %u: %s\n", error, lodepng_error_text(error));
  return 1;
	}
	return 0;
}	
static void pixMap_copy(pixMap *dest,pixMap *source){
	//if source image is zero then reset dest and copy width and height
	//if source image is not zero 
	  //if width or height are different 
	    //if width*height is different
	      //if dest->image is zero use malloc for dest->image
	      //else use realloc for dest->image
	      //if dest->height is different malloc or realloc dest->pixArray depending on whether it is zero 
	      //set dest->pixArray[0] to dest->image and dest->pixArray[i]=dest->pixArray[i-1]+source->width
	    //
	  //
	 //do a memcpy from source to dest
	 //set dest->width and dest->height to source values
	//
 if(!source->image){
	 pixMap_reset(dest);
	 dest->width=source->width;
	 dest->height=source->height;
	}
	if(dest->width != source->width || dest->height != source->height){
		if(dest->width * dest->height != source->width * source->height){
			if(dest->image){
	   if(!(dest->image=realloc(dest->image,source->height*source->width*sizeof(rgba))))exit(EXIT_FAILURE);
			}
			else if(!(dest->image=malloc(source->height*source->width*sizeof(rgba))))exit(EXIT_FAILURE);
		}
	 if(dest->height != source->height){
	  if(dest->pixArray){
	   if(!(dest->pixArray=realloc(dest->pixArray,source->height*sizeof(rgba*))))exit(EXIT_FAILURE);
			}
			else if(!(dest->pixArray=malloc(source->height*sizeof(rgba*))))exit(EXIT_FAILURE);
		}
		dest->pixArray[0]=(rgba*)dest->image;
		for(int i=1;i<source->height;i++){
			dest->pixArray[i]=dest->pixArray[i-1]+source->width;
		}	
	}		
	memcpy(dest->image,source->image,source->height*source->width*sizeof(rgba));
 dest->height=source->height;
 dest->width=source->width;
}

static void pixMap_reset(pixMap *p){
	if (p && p->image){
		free(p->image);
	 p->image=0;
	} 
 if (p && p->pixArray){
		free(p->pixArray);
		p->pixArray=0;
	}
}	


void pixMap_destroy(pixMap *p){
 if(p) pixMap_reset(p);
 if(p) free(p);
}


void pixMap_rotate (pixMap *p, float theta){
	//correct rotation that uses inverse rotation to determine which pixels to copy from the original
	//the previous method would leave holes in the image
	
	pixMap *temp=pixMap_init();//make a new temp blank pixMap structure
	pixMap_copy(temp,p); //copy the p to temp
	//set the values in the image to zero using memset - that the 3rd argument of memset is the size in BYTES
 memset(temp->image,0,temp->width*temp->height*sizeof(rgba));
 
 const float ox=p->width/2.0f;
 const float oy=p->height/2.0f;
 const float s=sin(degreesToRadians(-theta));
	const float c=cos(degreesToRadians(-theta));
	
	//for()   //double for loop to loop through each pixel in the original
	  //for()
	    //calculate the old coordinates rotx roty to copy from using the formula from 
	    //http://stackoverflow.com/questions/2259476/rotating-a-point-about-another-point-2d
	    //use the answer from stackoverflowery
	    
 	   //round the coordinates to the nearest integer in your calculations (add 0.5 and cast to integer)	
	    //if rotated coordinates are still inside the image
	      //copy the pixel at the old coords to the pixel to the temporary copy using memcpy
	      //i.e. 	memcpy(temp->pixArray[y]+x,p->pixArray[roty]+rotx,sizeof(rgba))
	    //
	  //
	//  
	//copy the temp pixMap to the original
	//destroy the temp

	for(int y=0;y<p->height;y++){
		for(int x=0;x<p->width;x++){ 
   float rotx = c*(x-ox) - s * (oy-y) + ox;
   float roty = -(s*(x-ox) + c * (oy-y) - oy);
		 int rotj=rotx+.5;
		 int roti=roty+.5; 
		 if(roti >=0 && roti < temp->height && rotj >=0 && rotj < temp->width){
				memcpy(temp->pixArray[y]+x,p->pixArray[roti]+rotj,sizeof(rgba));
			}	
		}	
	}
	pixMap_copy(p,temp);
	pixMap_destroy(temp);
	return;
}	
void pixMap_gray (pixMap *p){

	//loop through pixels using double for loop
	   //calculate average value of r and g and b values (assign to a float variable)
	   //round float variable to integer (add 0.5 before casting to integer)
	   //assign the rounded value to r,g and b values
	for(int y=0;y<p->height;y++){
	 for(int x=0;x<p->width;x++){
			const rgba pixel=p->pixArray[y][x]; 
			const float average= (pixel.r+pixel.g+pixel.b)/3.0f;
			const unsigned char iaverage=average+.5;
			p->pixArray[y][x].r=iaverage;
		 p->pixArray[y][x].g=iaverage;			
		 p->pixArray[y][x].b=iaverage;
		}	
	}
	return;
}
int pixMap_write(pixMap *p,char *filename){
 int error=0;
 if(lodepng_encode32_file(filename, p->image, p->width, p->height)){
  fprintf(stderr,"error %u: %s\n", error, lodepng_error_text(error));
  return 1;
	}
	return 0;
}

void pixMap_write_bmp16(pixMap *p,char *filename){
  BMP16_map *b = BMP16_map_init(p->height,p->width, 0, FIVE_MOST_SIG_BITS, SIX_MOST_SIG_BITS, FIVE_MOST_SIG_BITS);

  if(b){
    for(int y = 0; y < p->height; y++)
      for(int x = 0; x< p->width; x++){
	uint16_t convertedRGB = 0;

	uint16_t r4 = (p->pixArray[y][x].r & FIVE_BIT_MASK);
	uint16_t g4 = (p->pixArray[y][x].g & SIX_BIT_MASK);
	uint16_t b4 = (p->pixArray[y][x].b & FIVE_BIT_MASK);

	r4 >>= 3;
	g4 >>= 2;
	b4 >>= 3;

	convertedRGB = r4;

	convertedRGB <<= 6;
	convertedRGB |= g4;

	convertedRGB <<= 5;
	convertedRGB |= b4;

	b->pixArray[p->height-1-y][x] = convertedRGB;

      }
  BMP16_write(b,filename);
  BMP16_map_destroy(b);
  }
}

void pixMap_sort(pixMap *p){
  qsort(p->image, p->width*p->height, sizeof(rgba),pixMap_cmp);
}

static int pixMap_cmp(const void *a, const void *b){
  const rgba *ra = (rgba*)a;
  const rgba *rb = (rgba*)b;

  return (ra->r + ra->g + ra->b) - (rb->r + rb->g + rb->b);
}
