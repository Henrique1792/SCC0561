#include "RLE.h"



char *RLE_encoding(char *vector, int vectorSize){
	int i,j, count;
	char *rt = (char *)malloc((2*vectorSize)*sizeof(char));
	i=1; j=0;
	for(i = 1; i < vectorSize; i++){
		count = 1;
		while(i < vectorSize &&
			  vector[i] == vector[i+1]){
			  count++;
			  i++;
		}
		rt[j++] = vector[i];
		rt[j] = (char)i;
	}
	return rt;
}
