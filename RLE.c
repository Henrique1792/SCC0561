#include "RLE.h"

/*
 * our problem scope uses
	0~255 tonalities
	We'll sort those vectors together;
	where color values have the color and  Repetitions has each ocurrence
*/

static unsigned char colorValues[MAX_TONALITIES];
static int colorRepetitions[MAX_TONALITIES];

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

void RLE_workout(){
}
