#include "pixutils.h"
#include "optparse.h"

int main(int argc, char *argv[]){
  char *inputfile=0,*outputfile=0;
  float degrees=0;
  int option = 0, grayFlag = 0, conversionFlag = 0, sortFlag = 0;
  struct optparse options;

  optparse_init(&options, argv);

  while((option = optparse(&options, ":io::brgs")) != -1) {

    switch(option) {
      case 'b':
	conversionFlag = 1;
	break;
      case 's':
	sortFlag = 1;
	break;
      case 'i':
	inputfile = optparse_arg(&options);
	break;
      case 'o':
	outputfile = optparse_arg(&options);
	break;
      case 'r':
	degrees = atof(optparse_arg(&options));
	break;
      case 'g':
	grayFlag = 1;
	break;
      case '?':
	fprintf(stderr, "%s: %s\n", argv[0], options.errmsg);
	exit(EXIT_FAILURE);
    }
  }

  pixMap *p=pixMap_init_filename(inputfile);
  if(grayFlag)pixMap_gray(p);
  if(sortFlag)pixMap_sort(p);
  if(degrees)pixMap_rotate(p,degrees);

  if(conversionFlag){
    pixMap_write_bmp16(p,outputfile);
  } else{
  pixMap_write(p,outputfile);
  }

  pixMap_destroy(p);
  
  return 0;
}

