#ifndef RLE_H_
#define RLE_H_
#include "bitmap.h"
#include "utils.h"


char *RLE_encoding(char *vector, int vectorSize);
void RLE_decoding(char *tgt, char *src, int *position);

#endif
