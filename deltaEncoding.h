#ifndef DELTA_ENCODING_H_
#define DELTA_ENCODING_H_
#include "utils.h"


Table_t *deltaEncoding(char *vector, int vectorSize);
void deltaDecoding(signed char *buf, int vec);

#endif