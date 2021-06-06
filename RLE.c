#include "RLE.h"



char *RLE_encoding(char *vector, int vectorSize){
	int i,j, count;
	char *rt = (char *)malloc(sizeof(char));
	j=0;
	for(i = 1; i < vectorSize; i++){
		count = 1;
		while(i < vectorSize &&
			  vector[i] == vector[i+1]){
			  count++;
			  i++;
		}
		rt = (char *)realloc(rt,(2*sizeof(char)+sizeof(rt)));
		rt[j++] = vector[i];
		rt[j] = (char)i;
	}
	return rt;
}

//Table_t *RLE_workout(PIXEL_T *img, int imgSize){
//}
