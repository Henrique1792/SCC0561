#include "utils.h"

// função de transformacao de um decimal (negativo ou nao) para binário sem sinal
char *decimal2Binary(int Num) {
	int tmp[8];
	int i=0;
	char *rt=(char *)malloc(8*sizeof(char));


	int mask = 0x80; /* 10000000 */
	while(mask > 0){
		tmp[i++]= ((Num & mask) > 0);
		mask >>= 1;
	}
	for(i=0;i<8;i++)
		sprintf(&rt[i], "%d", tmp[i]);
	return rt;

#if 0
    int binaryNum = 0;
    int counter = 0;
    while (Num != 0) {
        int vNum = Num % 2;
        int eq = pow(10, counter);
        binaryNum += vNum * eq;
        Num /= 2;
        counter++;
    }

	// retornando binario sem sinal
	return abs(binaryNum);
#endif
}

int binary2int(char *src) {
	int root;
	int rt=0;
	int i=0;

	root = atoi(src);

	for(i=0; i<8; i++){
		if(root%10)
			rt+=pow(2,i);
	}
	
	
	return root;
}

/*
 *
 * Bit packing section
 *
 *
*/

void BitWrite(FILE *tgt, Table_t *input, int inputSize) {
	char buffer[10];
	int i = 0;

	for (i = 0; i < inputSize; i++) {

		//writing unicode
        sprintf(buffer, "%d", input[i].unicode);
		printf("buffer: %s\n", buffer);
        fwrite(buffer , sizeof(buffer), 1, tgt);

		//writing size
        sprintf(buffer, "%c", input[i].unicodeSize);
		printf("bufferSize: %s\n", buffer);
        fwrite(buffer , sizeof(buffer), 1 , tgt);
	}
}

Table_t *BitRead(FILE *tgt, char *input, int inputSize) {
	char buffer[8];
	input = (char *) malloc(8 * sizeof(char));
	int tmpUnicode;
	char tmpUnicodeSize;
	int i = 0;

	Table_t *rt = (Table_t *) malloc(inputSize * sizeof(Table_t));

	for (i = 0; i < inputSize; i++) {
        fread(buffer , 1 , sizeof(buffer) , tgt);
        sscanf(buffer, "%s", input);
		tmpUnicode = binary2int(buffer);
		rt[i].unicode = tmpUnicode;

		//reading unicodeSize
        fread(buffer , 1 , sizeof(buffer) , tgt);
        sscanf(buffer, "%s", input);
		tmpUnicodeSize = binary2int(buffer);
		rt[i].unicodeSize = tmpUnicodeSize;
	}

	free(input);

	return rt;
}

/*
 *
 * DCT section
 *
 *
*/

void DCT(char **m) {
	int i, j, x, y;
	double **res = (double **) malloc(8 * sizeof(double *));
	double cof;

	for (i = 0; i < 8; i++) {
		res[i] = (double *) malloc(8 * sizeof(double));
	}

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (i == 0 || j != 0) {
				cof = 1;
			} else {
				cof = 1 / (sqrt(2));
			}

			res[i][j] = 0;
			for (x = 0; x < 8; x++) {
				for (y = 0; y < 8; y++) {
					res[i][j] += (double) (m[x][y])
						* (cos((((2 * x) + 1) * i * M_PI) / 16))
						* (cos((((2 * y) + 1) * j * M_PI) / 16));
				}
			}
			res[i][j] *= 0.25 * (cof * cof);
		}

	}

	for (i = 0; i < 8; i++) {
		for(j = 0 ; j < 8; j++) {
			m[i][j] = (char)(ceil(res[i][j]));
		}
	}

	for (int i = 0; i < 8; i++) {
		free(res[i]);
	}
	free(res);
}

void IDCT(char **m) {
	int i, j, x, y;
	double **res = (double **) malloc(8 * sizeof(double *));
	double cof;

	for (i = 0; i < 8; i++) {
		res[i] = (double *) malloc(8 * sizeof(double));
	}

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if ((i == j) == 0) {
				cof = 1;
			}
			else {
				cof = 1/(sqrt(2));
			}

			res[i][j] = 0;
			for (x = 0; x < 8; x++) {
				for (y = 0; y < 8; y++) {
					res[i][j] += 0.25 * (cof * cof)
						* ((double) m[x][y])
						* (cos((((2 * x) + 1) * i * M_PI) / 16))
						* (cos((((2 * y) + 1) * j * M_PI) /16));
				}
			}
		}
	}

	for (i = 0; i < 8; i++) {
		for (j = 0 ; j<8; j++) {
			m[i][j] = floor((char) (res[i][j]));
		}
	}

	//don't forget to free this RES!
	for (int i = 0; i < 8; i++) {
		free(res[i]);
	}
	free(res);
}

/*
 *
 * zigzag procedure
 *
 *
*/

char *zigzagProcedure(char **tgt) {
	int i, j, k;
	int count, col, index;
	i = j = k = count = col = index = 0;

	char *ret = (char *) malloc(64 * sizeof(char));
	char tempStore[8];

//matrix first half
	for(i=0; i<7; i++){
		if(i == 0)
			ret[count++] = tgt[i][i]; 

		if(i > 0){
			index=0;
			for(k=i,col=0; k>=0; k--,col++)
				tempStore[index++] = tgt[k][col];

			
			if(i%2==0){
				for(k=0; k<index; k++)
					ret[count++] = tempStore[k]; 
			}
			else{
				for(k=index-1; k >= 0; k--)
					ret[count++] = tempStore[k]; 
			}

		}
	}

//matrix second half
	for(i=0;i<8;i++){

		index=0;
		for(k=i,col=7;k<8;k++,col--)
			tempStore[index++] = tgt[k][col];

		
		if(i%2==0){
			for(k=0;k<index;k++)
				ret[count++] = tempStore[k]; 
		}
		else{
			for(k=index-1;k >=0;k--)
				ret[count++] = tempStore[k]; 
		}

	}
	return ret;
}


char **zigzagUndo(char *zigzagOrder){
	int j=0,count=0,col=0,i,index=0,k=0,m=0;
	double tempStore[8];

	char **ret = (char **)malloc(8*sizeof(char *));

	for(i=0; i<8; i++)
		ret[i] = (char *)malloc(8*sizeof(char));

//get matrix first half
	for(i=0;i<7;i++){

		if(i==0)
			 ret[i][i] = zigzagOrder[count++]; 

		if(i > 0){
			
			index=0;
			if(i%2==0){
				for(k=0,m=i;m>=0;k++,m--){
					tempStore[k] = zigzagOrder[count++];
					index++;
				}
			}
			else{
				for(k=0,m=i;m>=0;k++,m--){
					tempStore[i-k] = zigzagOrder[count++];
					index++;
				}
			}

			for(k=i,col=0;k>=0;k--,col++)
				  ret[k][col] = tempStore[col];
		}
	}

//get matrix second half
	for(i=0;i<8;i++){		

			index=0;
			if(i%2==0){

				for(m=i;m<8;m++){
					 tempStore[index++] = zigzagOrder[count++];
				}
			}
			else{
				for(j=0;j<8;j++)
					tempStore[j]=0;

				for(m=i;m<8;m++){
					tempStore[7-m] = zigzagOrder[count++];
					index++;
				}
			}

			index=0;
			for(k=i,col=7;k<8;k++,col--)
				  ret[k][col] = tempStore[index++];
	}

	return ret;
}
