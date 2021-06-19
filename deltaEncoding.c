#include "deltaEncoding.h"

//HUFFMAN prefixes
const unsigned char prefixTable[9] = {2,   3,   4,   0,  5,   6,   14,   30,     62};
const unsigned char prefixTableSize[9] = {3,   3,   3,   2,  3,   3,   4,    5,      6};

Table_t *deltaEncoding(char *vector, int vectorSize) {
	int i=0, j=0, tam=0;
	int old_binary[vectorSize];
	char buffer[10] = {0};
	int vectorInt[vectorSize];
	int diff_vectorInt[vectorSize];
	Table_t *rt;
	int *diff_vector;
	char **binary_vector;

	//load content as int
	for (i = 0; i < vectorSize; i++) {
		vectorInt[i] = (int) vector[i];
	}


	diff_vector[0]=0;
	for(i = 0, j=1; j < vectorSize; i++, j++){
		diff_vector[j] = vectorInt[j] - vectorInt[i];
	}

#if 0

	rt			  = (Table_t *) malloc (vectorSize * sizeof(Table_t));
	diff_vector   = (int*) malloc (vectorSize * sizeof(int));
	binary_vector = (char **)malloc (vectorSize*sizeof(char *));

	diff_vector[0] = returnVector[0];
	binary_vector[0] = decimal2Binary(diff_vector[0]);
	for (i = 0; i < (vectorSize-1); ++i) {
		diff_vector[i + 1] = (returnVector[i + 1] - returnVector[i]);
		binary_vector[i + 1] = decimal2Binary(diff_vector[i + 1]);
	}

	for (int i = 0; i < vectorSize; ++i) {
		old_binary[i] = binary_vector[i];
		sprintf(buffer, "%u", (char)old_binary[i]);
		tam = strlen(buffer);
		rt[i].unicode = decimal2Binary(tam);
		old_binary[i] = rt[i].unicode;
		sprintf(buffer, "%u", (char)old_binary[i]);
		tam = strlen(buffer);
		rt[i].unicodeSize = tam;
	}
#endif


	//

	//free_content
	free(diff_vector);
	free(binary_vector);

	return rt;
}
