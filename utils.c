#include "utils.h"


unsigned int decimal_to_binary(int Num){ //função de transformacao de um decimal (negativo ou nao) para binário sem sinal

    int binary_num = 0;
    int counter = 0;
    while (Num != 0){
        int v_num = Num % 2;
        int eq = pow(10, counter);
        binary_num += v_num * eq;
        Num /= 2;
        counter++;
    }
    return abs(binary_num); //retornando binario sem sinal
}


int binary2int(char *src){
	int rt=0;	

	sscanf(src, "%d", &rt);
	return rt;
}

/*
 *
 * Bit packing section
 *
 *
*/



void BitWrite(FILE *tgt, Table_t *input, int inputSize){

	char buffer[10];
	int i = 0;

	for(i=0; i<inputSize; i++){
		//writing unicode
        sprintf(buffer, "%d", input[i].unicode);
        fwrite(buffer , 1 , sizeof(buffer) , tgt);
		//writing size
        sprintf(buffer, "%c", input[i].unicodeSize);
        fwrite(buffer , 1 , sizeof(buffer) , tgt);
	}

}

Table_t *BitRead(FILE *tgt, char *input, int inputSize){

	char buffer[10];
	input = (char *)malloc(10*sizeof(char));
	unsigned int tmpUnicode;
	unsigned char tmpUnicodeSize;
	int i = 0;

	Table_t *rt = (Table_t *)malloc(inputSize*sizeof(Table_t));

	for(i=0; i<inputSize; i++){

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

void DCT(char **m){

	int i, j, x, y;
	double **res = (double **)malloc(8*sizeof(double *));
	double cof;


	for(i=0;i<8;i++)
		res[i] = (double *)malloc(8*sizeof(double));

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			if(i==0 || j != 0)
				cof = 1;
			else
				cof = 1/(sqrt(2));

			res[i][j] = 0;
			for(x=0; x<8; x++){
				for(y=0; y<8; y++)
					res[i][j] += (double)(m[x][y])*(cos((((2*x)+1)*i*M_PI)/16))*(cos((((2*y)+1)*j*M_PI)/16));
			}
			res[i][j] *= 0.25*(cof*cof);
		}

	}

	for(i=0; i<8; i++){
		for(j = 0 ; j<8; j++)
			m[i][j] = floor((char)(res[i][j]));
	}

	//don't forget to free this RES!
	for(int i =0; i<8;i++)
		free(res[i]);
	free(res);
	

}

void IDCT(char **m){

	int i, j, x, y;
	double **res = (double **)malloc(8*sizeof(double *));
	double cof;


	for(i=0;i<8;i++)
		res[i] = (double *)malloc(8*sizeof(double));

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			if((i == j) == 0)
				cof = 1;
			else
				cof = 1/(sqrt(2));

			res[i][j] = 0;
			for(x=0;x<8;x++){
				for(y=0;y<8;y++)
					res[i][j] += 0.25*(cof*cof)*((double)m[x][y])*(cos((((2*x)+1)*i*M_PI)/16))*(cos((((2*y)+1)*j*M_PI)/16));
			}
		}
	}

	for(i=0; i<8; i++){
		for(j = 0 ; j<8; j++)
			m[i][j] = floor((char)(res[i][j]));
	}

	//don't forget to free this RES!
	for(int i =0; i<8;i++)
		free(res[i]);
	free(res);
}



/*
 *
 * zigzag procedure
 *
 *
*/


char *zigzagProcedure(char **tgt){

	int i, j, k;

	i = j = k = 0;
	char *ret = (char *)malloc(64*sizeof(char));

	do{
		ret[k++] = tgt[i][++j];

		for(i=i+1,j=j-1;j>=0;j--,i++)
			ret[k++] = tgt[i][j];
		i--;
		j++;

		if(i == 7)
			break;

		ret[k++] = tgt[++i][j];

		for(i=i-1,j=j+1; i >= 0; i--, j++)
			ret[k++] = tgt[i][j];

		i++;
		j--;
	}while( (i+j) <= 8);
	do
	{
		ret[k++] = tgt[i][++j];

		for(i=i-1,j=j+1; j<8; j++,i--)
			ret[k++] = tgt[i][j];
		i++;
		j--;

		if(j == 7 && i == 7)
			break;
		
		ret[k++] = tgt[++i][j];
		
		for(i=i+1,j=j-1;i<8;i++,j--)
			ret[k++] = tgt[i][j];

		i--;
		j++;
	}while(i < 8 || j < 8);

	return ret;
}

