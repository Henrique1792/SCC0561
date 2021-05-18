#ifndef RLE_H_
#define RLE_H_
#include "bitmap.h"
#include "utils.h"

#define MAX_TONALITIES 256 

void RLE_preparation(PIXEL_T *img, int imgSize, 
		unsigned char **rleInputVector, int ** rleRepetitionVector,
		int *colors);
Table_t *RLE_workout(PIXEL_T *img, int imgSize);

#endif
