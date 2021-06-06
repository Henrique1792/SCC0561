#ifndef BITMAP_H_
#define BITMAP_H_

#include "utils.h"

BitmapHeader *createBitmapHeader();
void freeBitmapHeader(BitmapHeader **tgt);

void fillBitmapHeader(FILE *src, BitmapHeader *tgt);
void printBitmapHeader(BitmapHeader *tgt);
void writeBitmapHeader(BitmapHeader *tgt, FILE *src);
void loadBMP(BitmapHeader *src, FILE *raw, PIXEL_T *img);
void writeBMP(BitmapHeader *src, PIXEL_T *img, FILE *raw);
void bmpSlashSquares(PIXEL_T *tgt, int width, int height,
					int imgSize, int *fullSquares);

#endif
