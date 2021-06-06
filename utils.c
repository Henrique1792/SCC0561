#include "utils.h"

void quicksortColors(unsigned char *colors, int *repetitions, int len){
  if (len < 2) return;
 
  int pivot = repetitions[len / 2];
  int tempR;
  unsigned char tempC;
 
  int i, j;
  for (i = 0, j = len - 1; ; i++, j--) {
    while (repetitions[i] < pivot) i++;
    while (repetitions[j] > pivot) j--;
 
    if (i >= j) break;
 
	//swapping repetitions vector
    tempR = repetitions[i];
	tempC = colors[i];

    repetitions[i] = repetitions[j];
    repetitions[j] = tempR;

    colors[i] = colors[j];
    colors[j] = tempC;
  }
 
  quicksortColors(colors, repetitions, i);
  quicksortColors(colors, repetitions + i, len - i);
}


int decimal_to_binary(int Num){ //função de transformacao de um decimal (negativo ou nao) para binário sem sinal

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


void GravaBit(Table_t *TabCodigos, int tam){ //Funcao para gravar os binários definidos em categorias huffman em um arquivo 'out.bin' binário
	FILE *p;
    char* str = (char*)malloc(10*sizeof(char)); //definindo uma nova variavel string para modificar o binario unsigned int e facilitar a escrita no arquivo

    p = fopen( "out.bin" , "w" );
    for (int i = 0; i < tam; ++i) {
        sprintf(str, "%d", TabCodigos[i].unicode);
        fwrite(str , 1 , sizeof(str) , p);
    }
    fclose(p);
	free(str);
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

	//don't forget to free this res matrix!

}



/*
 *
 * zigzag procedure
 *
 *
*/


char *zigzagProcedure(char **tgt){

	int i, j, k=0;

	unsigned char *ret = (unsigned char *)malloc(64*sizeof(unsigned char));

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

