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

void RLE_preparation(PIXEL_T *img, int imgSize){
	int i;
	if(img == NULL || imgSize) return;
	//initialize vectors
	

	for(i=0 ; i<MAX_TONALITIES; i++){
		colorValues[i] = i;
		colorRepetitions[i] = 0;
	}


// cover entire image
// Remember! This is working for an WB image!!!
// We need to consider other channels when dealing with other tones
	for(i = 0; i < imgSize ; i++){
		colorRepetitions[img->R]++;
	}
	
//dump colors with no ocurrence in the end of the vector
	for(i = 0 ; i <imgSize ; i++){
		if(colorRepetitions[i]==0)
			colorValues[i] = colorRepetitions[i] = UNUSED;
	}
}

