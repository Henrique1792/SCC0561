#include "RLE.h"

/*
 * our problem scope uses
	0~255 tonalities
	We'll sort those vectors together;
	where color values have the color and  Repetitions has each ocurrence
*/

static unsigned char colorValues[MAX_TONALITIES];
static int colorRepetitions[MAX_TONALITIES];

//HUFFMAN prefixes

const unsigned char TabPrefixos[9]={2,   3,   4,   0,  5,   6,   14,   30,     62};
const unsigned char TamPrefixos[9]={3,   3,   3,   2,  3,   3,   4,    5,      6};

/*
 *
 *	@param: img to be compressed
 *	@param: img_size to be compressed
 *	@param: vector to serve callee
 *	@param: repetition vector to serve callee
 *	@param: colors efectively used returning to callee
 *
 *	@return none.
*/

void RLE_preparation(PIXEL_T *img, int imgSize, 
		unsigned char **rleInputVector, int ** rleRepetitionVector,
		int *colors){

	int i,j;
	int colorsUsed = 0;

	if(img == NULL || 
	   imgSize <=0 ||
	   *rleInputVector != NULL || 
	   *rleRepetitionVector != NULL) return;

	//initialize vectors

	for(i=0 ; i<MAX_TONALITIES; i++){
		colorValues[i] = i;
		colorRepetitions[i] = 0;
	}


// cover entire image

	for(i = 0; i < imgSize ; i++){
		//add for other colors comparison here!
		colorRepetitions[(int)img->R]++;
	}
	


//we'll allocate a new vector with used colors
	for(i = 0 ; i < MAX_TONALITIES ; i++){

		if(colorRepetitions[i] != 0){
			colorsUsed++;
		}

	}


	*rleInputVector = (unsigned char *)malloc(colorsUsed*sizeof(unsigned char));
	*rleRepetitionVector = (int *)malloc(colorsUsed*sizeof(int));

	for(i=0;i<MAX_TONALITIES;i++){
		j=0;
		if(colorRepetitions[i] != 0){
			*rleInputVector[j] = i;
			*rleRepetitionVector[j] = colorRepetitions[i];
			j++;
		}
	}

	//We need to sort them and send it back to our callee
	quicksortColors(*rleInputVector, *rleRepetitionVector, colorsUsed);

	*colors = colorsUsed;
}

Table_t *RLE_workout(PIXEL_T *img, int imgSize){

	if(img == NULL || imgSize <= 0) return NULL;

	int i = 0, tam;
	unsigned int old_binary[imgSize];
	char buffer[10];
	int returnVector[imgSize];

	//load content as int
	for(i=0;i<imgSize;i++)
		returnVector[i] = (int)img[i].R;


    Table_t *tabela = (Table_t *)malloc(imgSize*sizeof(Table_t));
    int *diff_vector = (int*)malloc(imgSize*sizeof(int));
    int *binary_vector = (int*)malloc(imgSize*sizeof(int));

    diff_vector[0] = returnVector[0];
    binary_vector[0] = decimal_to_binary(diff_vector[0]);
    for (i = 0; i < (imgSize-1); ++i) {
        diff_vector[i+1] = (returnVector[i+1] - returnVector[i]);
        binary_vector[i+1] = decimal_to_binary(diff_vector[i+1]);
    }

    for (int i = 0; i < imgSize; ++i) {
		old_binary[i] = binary_vector[i]; //usa o primeiro binario convertido da diferencial
		sprintf(buffer, "%u", (char)old_binary[i]); //modifica para string
		tam = strlen(buffer); //encontra seu tamanho para definir tabela de prefixos huffman
		tabela[i].unicode = decimal_to_binary(tam); //modifica o valor encontrado para binario para setar o prefixo
		old_binary[i] = tabela[i].unicode;
		sprintf(buffer, "%u", (char)old_binary[i]);
		tam = strlen(buffer); //pega o novo binário encontrado para encontrar seu tamanho e setar o 
        tabela[i].unicodeSize = tam;

    }
	//free_content
	free(diff_vector);
	free(binary_vector);

    return tabela;

}
