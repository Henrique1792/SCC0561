#include "RLE.h"

char *RLE_encoding(char *vector, int vectorSize) {
	int i, j, count;
	char *rt = (char *) malloc((2 * vectorSize) * sizeof(char));
	i = 1; j = 0;


	for (i = 1; i < vectorSize; i++) {
		count = 1;
		while (i < vectorSize &&
			vector[i] == vector[i+1]) {
			count++;
			i++;
		}
		rt[j++] = vector[i];
		rt[j++] = (char)i;
	}

	return rt;
}

void RLE_decoding(char *tgt, char *src, int *position) {

	int repetitions, i, j;
	int p = 0;


	for(j=0;(long unsigned)j<sizeof(tgt);j+=2){
		repetitions = binary2int(&src[j]);

		for (i = 0; i < repetitions; i++) {
			tgt[p + i] = src[repetitions+i];
		}
		*position += repetitions;
		p+=repetitions;
	}
}
