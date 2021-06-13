#include "deltaEncoding.h"

//HUFFMAN prefixes
const unsigned char prefixTable[9]={2,   3,   4,   0,  5,   6,   14,   30,     62};
const unsigned char prefixTableSize[9]={3,   3,   3,   2,  3,   3,   4,    5,      6};

Table_t *deltaEncoding(char *vector, int vectorSize){

	int i = 0, tam;
	unsigned int old_binary[vectorSize];
	char buffer[10] = {0};
	int returnVector[vectorSize];

	//load content as int
	for(i=0;i<vectorSize;i++)
		returnVector[i] = (int)vector[i];


	Table_t *rt = (Table_t *)malloc(vectorSize*sizeof(Table_t));
	int *diff_vector = (int*)malloc(vectorSize*sizeof(int));
	int *binary_vector = (int*)malloc(vectorSize*sizeof(int));

	diff_vector[0] = returnVector[0];
	binary_vector[0] = decimal_to_binary(diff_vector[0]);
	for (i = 0; i < (vectorSize-1); ++i) {
		diff_vector[i+1] = (returnVector[i+1] - returnVector[i]);
		binary_vector[i+1] = decimal_to_binary(diff_vector[i+1]);
	}

	for (int i = 0; i < vectorSize; ++i) {
		old_binary[i] = binary_vector[i]; 
		sprintf(buffer, "%u", (char)old_binary[i]); 
		tam = strlen(buffer);
		rt[i].unicode = decimal_to_binary(tam); 
		old_binary[i] = rt[i].unicode;
		sprintf(buffer, "%u", (char)old_binary[i]);
		tam = strlen(buffer); 
		rt[i].unicodeSize = tam;

	}
	//free_content
	free(diff_vector);
	free(binary_vector);

	return rt;
}
