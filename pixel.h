#ifndef PIXEL_H_
#define PIXEL_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct PIXEL_T{
	unsigned char R;
	unsigned char G;
	unsigned char B;
}PIXEL_T;

PIXEL_T *createPIXEL_T();
void freePIXEL_T(PIXEL_T **tgt);
int updatePIXEL_T(PIXEL_T *tgt, unsigned char R, unsigned char G, unsigned char B);

#endif
