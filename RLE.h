#ifndef RLE_H_
#define RLE_H_
#include "bitmap.h"

#define UNUSED -1
#define MAX_TONALITIES 256 

void RLE_preparation(PIXEL_T *img, int imgSize, 
		unsigned char * rleInputVector, int * rleRepetitionVector);

#endif
