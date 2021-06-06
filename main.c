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
		char ***matrixR = (char ***)malloc(vectorSize*sizeof(char **));
		char ***matrixG = (char ***)malloc(vectorSize*sizeof(char **));
		char ***matrixB = (char ***)malloc(vectorSize*sizeof(char **));

		//slashing our image into a vector of 8x8 matrix
		bmpSlashSquares(img, bmpHeader->biWidth, bmpHeader->biHeight, 
				vectorSize);
		int i, j;

		for(i=0 ; i < vectorSize; i++){
				matrixR[i]=(char **)malloc((8)*sizeof(char *));
				matrixG[i]=(char **)malloc((8)*sizeof(char *));
				matrixB[i]=(char **)malloc((8)*sizeof(char *));

				for (j=0; j < 8; j++){
					matrixR[i][j] = (char *)malloc(8*sizeof(char));
					matrixG[i][j] = (char *)malloc(8*sizeof(char));
					matrixB[i][j] = (char *)malloc(8*sizeof(char));
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


		//quantization + zigzag scan!

		char qzTable[8][8];

// setting qzTable values

		qzTable[0][0] = 16;
		qzTable[0][1] = 11;
		qzTable[0][2] = 10;
		qzTable[0][3] = 16;
		qzTable[0][4] = 24;
		qzTable[0][5] = 40;
		qzTable[0][6] = 51;
		qzTable[0][7] = 61;

		qzTable[1][0] = 12;
		qzTable[1][1] = 12;
		qzTable[1][2] = 14;
		qzTable[1][3] = 19;
		qzTable[1][4] = 26;
		qzTable[1][5] = 58;
		qzTable[1][6] = 60;
		qzTable[1][7] = 55;

		qzTable[2][0] = 14;
		qzTable[2][1] = 13;
		qzTable[2][2] = 16;
		qzTable[2][3] = 24;
		qzTable[2][4] = 40;
		qzTable[2][5] = 57;
		qzTable[2][6] = 69;
		qzTable[2][7] = 56;

		qzTable[3][0] = 14;
		qzTable[3][1] = 17;
		qzTable[3][2] = 22;
		qzTable[3][3] = 29;
		qzTable[3][4] = 51;
		qzTable[3][5] = 87;
		qzTable[3][6] = 80;
		qzTable[3][7] = 62;

		qzTable[4][0] = 18;
		qzTable[4][1] = 22;
		qzTable[4][2] = 37;
		qzTable[4][3] = 56;
		qzTable[4][4] = 68;
		qzTable[4][5] = 109;
		qzTable[4][6] = 103;
		qzTable[4][7] = 77;

		qzTable[5][0] = 24;
		qzTable[5][1] = 35;
		qzTable[5][2] = 55;
		qzTable[5][3] = 64;
		qzTable[5][4] = 81;
		qzTable[5][5] = 104;
		qzTable[5][6] = 113;
		qzTable[5][7] = 92;

		qzTable[6][0] = 49;
		qzTable[6][1] = 64;
		qzTable[6][2] = 78;
		qzTable[6][3] = 87;
		qzTable[6][4] = 103;
		qzTable[6][5] = 121;
		qzTable[6][6] = 120;
		qzTable[6][7] = 101;

		qzTable[7][0] = 72;
		qzTable[7][1] = 92;
		qzTable[7][2] = 95;
		qzTable[7][3] = 98;
		qzTable[7][4] = 112;
		qzTable[7][5] = 100;
		qzTable[7][6] = 103;
		qzTable[7][7] = 99;


		for(int k=0; k < vectorSize; k++){
			for(i=0; i<8; i++){
				for(j=0; j<8; j++){
					matrixR[k][i][j] = matrixR[k][i][j] / qzTable[i][j];
					matrixG[k][i][j] = matrixG[k][i][j] / qzTable[i][j];
					matrixB[k][i][j] = matrixB[k][i][j] / qzTable[i][j];
				}
			}
		}

		char **zzScanR = (char **)malloc(vectorSize*sizeof(char *));
		char **zzScanG = (char **)malloc(vectorSize*sizeof(char *));
		char **zzScanB = (char **)malloc(vectorSize*sizeof(char *));


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
