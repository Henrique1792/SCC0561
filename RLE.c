#include "RLE.h"

/*
 * our problem scope uses
	0~255 tonalities
	We'll sort those vectors together;
	where color values have the color and  Repetitions has each ocurrence
*/
static unsigned char colorValues[256];
static int colorRepetitions[256];


/*
 *
 *	@param: img to be compressed
 *	@param: img_size to be compressed
 *
 *	@return none.
*/

void RLE_preparation(PIXEL_T *img, int imgSize, 
		unsigned char * rleInputVector, int * rleRepetitionVector){

	int i;
	int colorsUsed = 0;

	if(img == NULL || 
	   imgSize <=0 ||
	   rleInputVector != NULL || 
	   rleRepetitionVector != NULL) return;

	//initialize vectors
	

	for(i=0 ; i<MAX_TONALITIES; i++){
		colorValues[i] = i;
		colorRepetitions[i] = 0;
	}


// cover entire image
// Remember! This is working for an WB image!!!
// We need to consider other channels when dealing with other tones
	for(i = 0; i < imgSize ; i++){
		colorRepetitions[(int)img->R]++;
	}
	
//dump colors with no ocurrence 
	for(i = 0 ; i < imgSize ; i++){
		if(colorRepetitions[i]==0)
			colorValues[i] = colorRepetitions[i] = UNUSED;
	}


//we'll allocate a new vector with used colors
	for(i = 0 ; i < MAX_TONALITIES ; i++){
		if(colorRepetitions[i]!=UNUSED)	
			colorsUsed++;
	}

	rleInputVector = (unsigned char *)malloc(colorsUsed*sizeof(unsigned char));
	rleRepetitionVector = (int *)malloc(colorsUsed*sizeof(int));

	int j=0;	
	for(i = 0 ; i < MAX_TONALITIES ; i++){
		if(colorRepetitions[i]!=UNUSED){
			rleInputVector[j]=colorValues[i];
			rleRepetitionVector[j]=colorRepetitions[j];
			++j;
		}
	}

	//We need to sort them and send it back to our callee
	quicksortColors(rleInputVector, rleRepetitionVector, colorsUsed);

}

void RLE_workout(){
}
