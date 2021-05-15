#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pixel.h"

#define BEGINBMP(src)  fseek(src, 53, SEEK_SET);
#define ENDBMP(src)	fseek(src, 0, SEEK_END);

//Bitmap Content
typedef struct BitmapHeader{
/*BMPFILEHEADER structure*/
unsigned short bfType;           /* Magic number for file */
unsigned int   bfSize;              /* Size of file */
unsigned short bfReserved1;    /* Reserved */
unsigned short bfReserved2;    /* ... */
unsigned int   bfOffBits;           /* Offset to bitmap data */

/*BMPINFOHEADER structure*/
unsigned int   biSize;             /* Size of info header */
int            biWidth;               /* Width of image */
int            biHeight;              /* Height of image */
unsigned short biPlanes;        /* Number of color planes */
unsigned short biBitCount;     /* Number of bits per pixel */
unsigned int   biCompression; /* Type of compression to use */
unsigned int   biSizeImage;     /* Size of image data */
int            biXPelsPerMeter;    /* X pixels per meter */
int            biYPelsPerMeter;    /* Y pixels per meter */
unsigned int   biClrUsed;         /* Number of colors used */
unsigned int   biClrImportant;  /* Number of important colors */

}BitmapHeader;

void WImg(BitmapHeader *bmpHeader, PIXEL_T *src);



#endif
