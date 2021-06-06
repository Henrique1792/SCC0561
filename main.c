#include "RLE.h"

#define RED "red.bmp"
#define GREEN "green.bmp"
#define BLUE "blue.bmp"


int main(int argc, char *argv[]){
	unsigned char *rleInputVector = NULL;
	int *rleRepetitionVector = NULL;

// rle input vector
	//int colors;

//FILES section
	FILE *src;

//names section

	if(argc < 3){
		printf("insuficient nargs\n");
		printf("USAGE: <imgname||compressed_filename> <compression||decompression>\n");
		exit(1);
	}



	if(!strcmp(argv[3], "compression")){
	//BitmapHeader
		BitmapHeader *bmpHeader = createBitmapHeader();
		printf("FILE FOUND: %s\n", argv[1]);
		src=fopen(argv[1], "rb");

	//Read BitmapHeader
		fillBitmapHeader(src, bmpHeader);
		printBitmapHeader(bmpHeader);

		if(bmpHeader->biHeight % 8 != 0 ||
		   bmpHeader->biWidth % 8 != 0 ||
		   (bmpHeader->biHeight < 8 || bmpHeader->biHeight > 1280) ||
		   (bmpHeader->biWidth < 8 || bmpHeader->biWidth > 800) ){
			printf("Invalid image! Image standards accepted:\nMax_size: 1280x800\tMin_Size: 8x8\n \
					width %% 8 = 0\t height %% 8 = 0\n \
					24bits/color");

			//close and deallocate stuff if falls here, please
			freeBitmapHeader(&bmpHeader);
			fclose(src);
			exit(1);
		}

//Allocate image
		PIXEL_T *img = (PIXEL_T *)malloc(((bmpHeader->biWidth)*(bmpHeader->biHeight))*sizeof(PIXEL_T));

//Reload img postion and loading bmp to img
		BEGINBMP(src, bmpHeader->bfOffBits);
		loadBMP(bmpHeader, src, img);

		int vectorSize = ((bmpHeader->biWidth/8)*(bmpHeader->biHeight/8));
		unsigned char ***matrixR = (unsigned char ***)malloc(vectorSize*sizeof(unsigned char **));
		unsigned char ***matrixG = (unsigned char ***)malloc(vectorSize*sizeof(unsigned char **));
		unsigned char ***matrixB = (unsigned char ***)malloc(vectorSize*sizeof(unsigned char **));

		//slashing our image into a vector of 8x8 matrix
		bmpSlashSquares(img, bmpHeader->biWidth, bmpHeader->biHeight, 
				vectorSize);
		int i, j;

		for(i=0 ; i < vectorSize; i++){
				matrixR[i]=(unsigned char **)malloc((8)*sizeof(unsigned char *));
				matrixG[i]=(unsigned char **)malloc((8)*sizeof(unsigned char *));
				matrixB[i]=(unsigned char **)malloc((8)*sizeof(unsigned char *));

				for (j=0; j < 8; j++){
					matrixR[i][j] = (unsigned char *)malloc(8*sizeof(unsigned char));
					matrixG[i][j] = (unsigned char *)malloc(8*sizeof(unsigned char));
					matrixB[i][j] = (unsigned char *)malloc(8*sizeof(unsigned char));
				}
		}

		int imgPosition;

		for(imgPosition = 0; 
			imgPosition < ((bmpHeader->biWidth)*(bmpHeader->biHeight)); 
			imgPosition++){

			matrixR[i][img[imgPosition].nSquareW][img[imgPosition].nSquareH] = img[imgPosition].R;
			matrixG[i][img[imgPosition].nSquareW][img[imgPosition].nSquareH] = img[imgPosition].G;
			matrixB[i][img[imgPosition].nSquareW][img[imgPosition].nSquareH] = img[imgPosition].B;

			if(imgPosition % 8 == 0){
				if(i < vectorSize)
					i++;
				if(i == vectorSize)
					i = 0;
			}
		}


		//here we'll apply DCT in each 8x8 matrix
		for(i = 0 ; i < vectorSize ; i++){
			DCT(matrixR[i]);
			DCT(matrixG[i]);
			DCT(matrixB[i]);
		}


		//quantization - zigzag scan!
		//
		unsigned char **zzScanR = (unsigned char **)malloc(vectorSize*sizeof(unsigned char *));
		unsigned char **zzScanG = (unsigned char **)malloc(vectorSize*sizeof(unsigned char *));
		unsigned char **zzScanB = (unsigned char **)malloc(vectorSize*sizeof(unsigned char *));


		for(i = 0; i < vectorSize ; i++){
			zzScanR[i] = zigzagProcedure(matrixR[i]);
			zzScanG[i] = zigzagProcedure(matrixG[i]);
			zzScanB[i] = zigzagProcedure(matrixB[i]);
		}

		//RLE+huffman
		
		//Remember: 
		//[0] positions: RLE
		//[remaining] - differ encoding



		//Write file
		//writeBitmapHeader(bmpHeader, bw);
		//writeBMP(bmpHeader, img, bw);
		
		//RLE_preparation(img, (bmpHeader->biWidth)*(bmpHeader->biHeight),
		//				jpeg compression divide file&rleInputVector, &rleRepetitionVector, &colors);


		//Table_t * output = RLE_workout(img,(bmpHeader->biWidth)*(bmpHeader->biHeight));
		//GravaBit(output, (bmpHeader->biWidth)*(bmpHeader->biHeight));

	//free content
	//bitmapHeader
		freeBitmapHeader(&bmpHeader);

	//zzScanVector
	
		for(i = 0; i < vectorSize ; i++){
			free(zzScanR[i]);
			free(zzScanG[i]);
			free(zzScanB[i]);
		}
		free(zzScanR);
		free(zzScanG);
		free(zzScanB);

		//DCT matrix

		for(i = 0; i < vectorSize ; i++){
			for(j = 0; j < vectorSize ; j++){
				free(matrixR[i][j]);
				free(matrixG[i][j]);
				free(matrixB[i][j]);

			}
			free(matrixR[i]);
			free(matrixG[i]);
			free(matrixB[i]);
		}

		free(matrixR);
		free(matrixG);
		free(matrixB);


		//free(output);
		free(rleInputVector);
		free(rleRepetitionVector);

	//deallocate img
		free(img);
		fclose(src);
	}else{
		if(!strcmp(argv[3],"decompression")){
			printf("Dummy content here\n");
		}
	}

	return EXIT_SUCCESS;
}
