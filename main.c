#include "RLE.h"

#define RED "red.bmp"
#define GREEN "green.bmp"
#define BLUE "blue.bmp"


int main(int argc, char *argv[]){
	unsigned char *rleInputVector = NULL;
	int *rleRepetitionVector = NULL;

// rle input vector
	int i, colors;

//FILES section
	FILE *src;

//names section

	if(argc < 2){
		printf("insuficient nargs\n");
		exit(1);
	}

//BitmapHeader
	BitmapHeader *bmpHeader = createBitmapHeader();
	printf("FILE FOUND: %s\n", argv[1]);
	src=fopen(argv[1], "rb");


//Read BitmapHeader
	fillBitmapHeader(src, bmpHeader);
	printBitmapHeader(bmpHeader);

	PIXEL_T *img = (PIXEL_T *)malloc(((bmpHeader->biWidth)*(bmpHeader->biHeight))*sizeof(PIXEL_T));

//Reload img postion and loading bmp to img
	//BEGINBMP(src);
	loadBMP(bmpHeader, src, img);

	//set it in dark mode
	WImg(bmpHeader, img);
	 

	FILE *bw = fopen("bw.bmp", "wb");
	writeBitmapHeader(bmpHeader, bw);
	writeBMP(bmpHeader, img, bw);
	
	RLE_preparation(img, (bmpHeader->biWidth)*(bmpHeader->biHeight),
					&rleInputVector, &rleRepetitionVector, &colors);


//free bitmapHeader
	freeBitmapHeader(&bmpHeader);

	free(rleInputVector);
	free(rleRepetitionVector);

//deallocate img
	free(img);
	fclose(bw);
	fclose(src);

	return EXIT_SUCCESS;
}
