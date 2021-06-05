#include "bitmap.h"

BitmapHeader *createBitmapHeader(){
	BitmapHeader *rt = (BitmapHeader *)malloc(sizeof(BitmapHeader));
	return (rt == NULL ?  NULL : rt);
}

void freeBitmapHeader(BitmapHeader **tgt){
	if(tgt == NULL || *tgt==NULL) return;
	free(*tgt);
}


void fillBitmapHeader(FILE *src, BitmapHeader *tgt){
	//use file already opened bro
	if(src== NULL || tgt == NULL) return;

	rewind(src);
	// reading BMPFILEHEADER
	fread(&tgt->bfType, sizeof(tgt->bfType), 1, src);
	fread(&tgt->bfSize, sizeof(tgt->bfSize), 1, src);
	fread(&tgt->bfReserved1, sizeof(tgt->bfReserved1), 1, src);
	fread(&tgt->bfReserved2, sizeof(tgt->bfReserved2), 1, src);
	fread(&tgt->bfOffBits, sizeof(tgt->bfOffBits), 1, src);


	// reading BMPINFOHEADER
	fread(&tgt->biSize, sizeof(tgt->biSize), 1, src);
	fread(&tgt->biWidth, sizeof(tgt->biWidth), 1, src);
	fread(&tgt->biHeight, sizeof(tgt->biHeight), 1, src);
	fread(&tgt->biPlanes, sizeof(tgt->biPlanes), 1, src);
	fread(&tgt->biBitCount, sizeof(tgt->biBitCount), 1, src);
	fread(&tgt->biCompression, sizeof(tgt->biCompression), 1, src);
	fread(&tgt->biSizeImage, sizeof(tgt->biSizeImage), 1, src);
	fread(&tgt->biXPelsPerMeter, sizeof(tgt->biXPelsPerMeter), 1, src);
	fread(&tgt->biYPelsPerMeter, sizeof(tgt->biYPelsPerMeter), 1, src);
	fread(&tgt->biClrUsed, sizeof(tgt->biClrUsed), 1, src);
	fread(&tgt->biClrImportant, sizeof(tgt->biClrImportant), 1, src);
}

void printBitmapHeader(BitmapHeader *tgt){

	printf("*************** File Header ***************\n\n");

	printf("Magic number for file: %x\n", tgt->bfType);   
	printf("File's size: %d\n",tgt->bfSize);           
	printf("Offset to bitmap data: %d\n", tgt->bfOffBits);

	printf("\n\n");
	printf("*************** Info Header ***************\n\n");
	printf("Info header's size: %d\n", tgt->biSize);
	printf("Width: %d\n", tgt->biWidth);          
	printf("Height: %d\n",tgt->biHeight);
	printf("Color planes: %d\n", tgt->biPlanes);
	printf("Bits per pixel: %d\n", tgt->biBitCount);
	printf("Compression type (0 = no compression): %d\n", tgt->biCompression);
	printf("Image's data size: %d\n", tgt->biSizeImage);
	printf("X Pixels per meter: %d\n", tgt->biXPelsPerMeter);
	printf("Y Pixels per meter: %d\n", tgt->biYPelsPerMeter);
	printf("Number of colors: %d\n", tgt->biClrUsed);
	printf("Numberof important colors: %d\n", tgt->biClrImportant); 

}

void writeBitmapHeader(BitmapHeader *tgt, FILE *src){
	if(src == NULL || tgt == NULL) return;
	
// writing BMPFILEHEADER
	fwrite(&tgt->bfType, sizeof(tgt->bfType), 1, src);
	fwrite(&tgt->bfSize, sizeof(tgt->bfSize), 1, src);
	fwrite(&tgt->bfReserved1, sizeof(tgt->bfReserved1), 1, src);
	fwrite(&tgt->bfReserved2, sizeof(tgt->bfReserved2), 1, src);
	fwrite(&tgt->bfOffBits, sizeof(tgt->bfOffBits), 1, src);


// writing BMPINFOHEADER
	fwrite(&tgt->biSize, sizeof(tgt->biSize), 1, src);
	fwrite(&tgt->biWidth, sizeof(tgt->biWidth), 1, src);
	fwrite(&tgt->biHeight, sizeof(tgt->biHeight), 1, src);
	fwrite(&tgt->biPlanes, sizeof(tgt->biPlanes), 1, src);
	fwrite(&tgt->biBitCount, sizeof(tgt->biBitCount), 1, src);
	fwrite(&tgt->biCompression, sizeof(tgt->biCompression), 1, src);
	fwrite(&tgt->biSizeImage, sizeof(tgt->biSizeImage), 1, src);
	fwrite(&tgt->biXPelsPerMeter, sizeof(tgt->biXPelsPerMeter), 1, src);
	fwrite(&tgt->biYPelsPerMeter, sizeof(tgt->biYPelsPerMeter), 1, src);
	fwrite(&tgt->biClrUsed, sizeof(tgt->biClrUsed), 1, src);
	fwrite(&tgt->biClrImportant, sizeof(tgt->biClrImportant), 1, src);
}

void loadBMP(BitmapHeader *src, FILE *raw, PIXEL_T *img){
	if(src == NULL || raw == NULL || img == NULL) return;

	int content = (src->biWidth * src->biHeight);
	unsigned char tmp[3];


	while(content>=0){
		fread(tmp, 3, sizeof(unsigned char), raw);
		img[content].B = tmp[0];
		img[content].G = tmp[1];
		img[content--].R = tmp[2];
	}
}


//we're writing img with src header on raw file
void writeBMP(BitmapHeader *src, PIXEL_T *img, FILE *raw){
	if(src == NULL || raw == NULL || img == NULL) return;

	int content = (src->biWidth * src->biHeight);
	int i;
	unsigned char tmp[3];
	rewind(raw);
	BEGINBMP(raw, src->bfOffBits);

	for(i = content ; i >= 0; i--){
		tmp[0] = img[i].B; 
		tmp[1] = img[i].G;
		tmp[2] = img[i].R; 
		fwrite(tmp, 3, sizeof(unsigned char), raw);
	}

}


void bmpSlashSquares(PIXEL_T *tgt, int width, int height){

	int nSquareW, nSquareH, k;
	nSquareW = nSquareH = 0;

	for(k = 0; k < width*height; k++){
		if(k % 8 == 0){
			nSquareW++;

		}else{

			if(k % width == 0){
				nSquareW = 0;
				nSquareH++;
			}

		}

		tgt[k].nSquareW = nSquareW;
		tgt[k].nSquareH = nSquareH;
	}
}

