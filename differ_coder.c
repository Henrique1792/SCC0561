#include "differ_coder.h"



void diffEncoding(PIXEL_T *img, int imgSize){


	if(img == NULL || imgSize <= 0) return ;

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
