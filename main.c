#include "RLE.h"
#include "deltaEncoding.h"


char qzTable[8][8];

// setting qzTable values



int main(int argc, char *argv[]){

	//initializing quantization table;
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


	//declarations
	// img/file strucuture
	BitmapHeader *bmpHeader;
	PIXEL_T *img; int imgPosition = 0;
	int vectorSize = 0;
	int fullSquares = 0;

	//Matrices for DCT/IDCT
	char ***matrixR;
	char ***matrixG;
	char ***matrixB;
	
	// vectors for zzscan
	char **zzScanR;
	char **zzScanG;
	char **zzScanB;


	//rle vectors
	char **rleVectorsB;
	char **rleVectorsG;
	char **rleVectorsR;

	char *rleVectorDecompressB;
	char *rleVectorDecompressG;
	char *rleVectorDecompressR;

	//delta required structures
	char *deltaInputR; 
	char *deltaInputG; 
	char *deltaInputB; 
	Table_t *deltaR;
	Table_t *deltaG;
	Table_t *deltaB;


	int i, j, k, l;

	FILE *src;
	FILE *outfile;
//names section

	if(argc < 3){
		printf("insuficient nargs\n");
		printf("USAGE: <compression||decompression> <imgname||compressed_filename>\n");
		exit(1);
	}

	if(!strcmp(argv[1], "compression")){
	//BitmapHeader
		bmpHeader = createBitmapHeader();
		src=fopen(argv[2], "rb");

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
		img = (PIXEL_T *)malloc(((bmpHeader->biWidth)*(bmpHeader->biHeight))*sizeof(PIXEL_T));

//Reload img postion and loading bmp to img structure
		BEGINBMP(src, bmpHeader->bfOffBits);
		loadBMP(bmpHeader, src, img);

		//slashing our image into a vector of 8x8 matrix
		bmpSlashSquares(img, bmpHeader->biWidth, bmpHeader->biHeight, 
				((bmpHeader->biWidth)*(bmpHeader->biHeight)), &fullSquares);

		vectorSize = ((bmpHeader->biWidth/8)*(bmpHeader->biHeight/8));
		matrixR = (char ***)malloc(vectorSize*sizeof(char **));
		matrixG = (char ***)malloc(vectorSize*sizeof(char **));
		matrixB = (char ***)malloc(vectorSize*sizeof(char **));


		for(i=0 ; i < vectorSize; i++){
				matrixR[i]=(char **)malloc(8*sizeof(char *));
				matrixG[i]=(char **)malloc(8*sizeof(char *));
				matrixB[i]=(char **)malloc(8*sizeof(char *));

				for (j=0; j < 8; j++){
					matrixR[i][j] = (char *)malloc(8*sizeof(char));
					matrixG[i][j] = (char *)malloc(8*sizeof(char));
					matrixB[i][j] = (char *)malloc(8*sizeof(char));
				}
		}


		i=0;
		for(imgPosition = 0; 
			imgPosition < ((bmpHeader->biWidth)*(bmpHeader->biHeight)); 
			imgPosition++){

			matrixR[i][img[imgPosition].nSquareW][img[imgPosition].nSquareH] = img[imgPosition].R;
			matrixG[i][img[imgPosition].nSquareW][img[imgPosition].nSquareH] = img[imgPosition].G;
			matrixB[i][img[imgPosition].nSquareW][img[imgPosition].nSquareH] = img[imgPosition].B;

			if(imgPosition % 64 == 0){
				if(i < 7)
					i++;
				else
					i = 0;
			}
		}


		//here we'll apply DCT in each 8x8 matrix
		for(i = 0 ; i < fullSquares ; i++){
			DCT(matrixR[i]);
			DCT(matrixG[i]);
			DCT(matrixB[i]);
		}

		//quantization + zigzag scan!
		for(k=0; k < fullSquares; k++){
			for(i=0; i<8; i++){
				for(j=0; j<8; j++){
					matrixB[k][i][j] /= qzTable[i][j];
					matrixG[k][i][j] /= qzTable[i][j];
					matrixR[k][i][j] /= qzTable[i][j];
				}
			}
		}

		zzScanR = (char **)calloc(vectorSize, sizeof(char *));
		zzScanG = (char **)calloc(vectorSize, sizeof(char *));
		zzScanB = (char **)calloc(vectorSize, sizeof(char *));


		for(i = 0; i < fullSquares ; i++){
			zzScanB[i] = zigzagProcedure(matrixB[i]);
			zzScanG[i] = zigzagProcedure(matrixG[i]);
			zzScanR[i] = zigzagProcedure(matrixR[i]);
		}

		//RLE+huffman
		
		//Remember: 
		//[0] positions: delta encoding
		//[remaining] - RLE

		//delta
		deltaInputB = (char *)calloc(fullSquares, sizeof(char));
		deltaInputG = (char *)calloc(fullSquares, sizeof(char));
		deltaInputR = (char *)calloc(fullSquares, sizeof(char));

		for(i=0 ; i<fullSquares;i++){
			deltaInputB[i] = zzScanB[i][0];
			deltaInputG[i] = zzScanG[i][0];
			deltaInputR[i] = zzScanR[i][0];
		}


		deltaB = deltaEncoding(deltaInputB, fullSquares);
		deltaG = deltaEncoding(deltaInputG, fullSquares);
		deltaR = deltaEncoding(deltaInputR, fullSquares);

		//rle
		rleVectorsB = (char **)calloc(fullSquares, sizeof(char *));
		rleVectorsG = (char **)calloc(fullSquares, sizeof(char *));
		rleVectorsR = (char **)calloc(fullSquares, sizeof(char *));

		for(i = 0; i < fullSquares ; i++){
			rleVectorsB[i] = RLE_encoding(zzScanB[i], 64);
			rleVectorsG[i] = RLE_encoding(zzScanG[i], 64);
			rleVectorsR[i] = RLE_encoding(zzScanR[i], 64);
		}

		//Write file sequence

		//standard stablishment:
		//
		//original BMPheader
		//fullSquares obtained
		//deltaB
		//deltaG
		//deltaR
		//for i in bitmap
		//rleVectorsB
		//rleVectorsG
		//rleVectorsR

		outfile = fopen("out.bin", "wb");
		writeBitmapHeader(bmpHeader, outfile);	
		fwrite(&fullSquares, sizeof(int), 1, outfile);

		BitWrite(outfile, deltaB, fullSquares);
		BitWrite(outfile, deltaG, fullSquares);
		BitWrite(outfile, deltaR, fullSquares);

		
		for(i=0; i<fullSquares; i++){
			fwrite(rleVectorsB[i],64, 1, outfile);
			fwrite(rleVectorsG[i],64, 1, outfile);
			fwrite(rleVectorsR[i],64, 1, outfile);
		}

	}else{
		if(!strcmp(argv[1],"decompression")){
			bmpHeader = createBitmapHeader();
			src=fopen(argv[2], "rb");
			//recovering header content
			fillBitmapHeader(src, bmpHeader);
			printBitmapHeader(bmpHeader);

			if(bmpHeader->biHeight % 8 != 0 ||
			   bmpHeader->biWidth % 8 != 0 ||
			   (bmpHeader->biHeight < 8 || bmpHeader->biHeight > 1280) ||
			   (bmpHeader->biWidth < 8 || bmpHeader->biWidth > 800) ){
				printf("Seems odd, but the compressed file isn't valid\n standards accepted:\nMax_size: 1280x800\tMin_Size: 8x8\n \
						width %% 8 = 0\t height %% 8 = 0\n \
						24bits/color");

				//close and deallocate stuff if falls here, please
				freeBitmapHeader(&bmpHeader);
				fclose(src);
				exit(1);
			}

			vectorSize = ((bmpHeader->biWidth/8)*(bmpHeader->biHeight/8));
			
			//We'll use fullSquares for partition
			fread(&fullSquares, sizeof(int), 1, src);

			//recovering delta vectors
			deltaInputR = (char *)calloc(fullSquares, sizeof(char));
			deltaInputG = (char *)calloc(fullSquares, sizeof(char));
			deltaInputB = (char *)calloc(fullSquares, sizeof(char));

			deltaB = BitRead(src, deltaInputB, fullSquares);
			deltaG = BitRead(src, deltaInputG, fullSquares);
			deltaR = BitRead(src, deltaInputR, fullSquares);

			//rle
			rleVectorsB = (char **)malloc(fullSquares*sizeof(char));
			rleVectorsG = (char **)malloc(fullSquares*sizeof(char));
			rleVectorsR = (char **)malloc(fullSquares*sizeof(char));

			rleVectorDecompressB = (char *)malloc((2*fullSquares)*sizeof(char));
			rleVectorDecompressG = (char *)malloc((2*fullSquares)*sizeof(char));
			rleVectorDecompressR = (char *)malloc((2*fullSquares)*sizeof(char));


			int posB, posG, posR;
			posB = posG = posR = 0;

			for(i = 0; i < fullSquares; i++){
				rleVectorsB[i] = (char *)calloc(fullSquares, sizeof(char));
				rleVectorsG[i] = (char *)calloc(fullSquares, sizeof(char));
				rleVectorsR[i] = (char *)calloc(fullSquares, sizeof(char));

				fread(&rleVectorDecompressB,sizeof(rleVectorDecompressB),1,src);
				fread(&rleVectorDecompressG,sizeof(rleVectorDecompressG),1,src);
				fread(&rleVectorDecompressR,sizeof(rleVectorDecompressR),1,src);

				RLE_decoding(rleVectorsB[i], rleVectorDecompressB, &posB);
				RLE_decoding(rleVectorsG[i], rleVectorDecompressG, &posG);
				RLE_decoding(rleVectorsR[i], rleVectorDecompressR, &posR);
			}

			//well rebuild matrix
			//delta[val], RLE
				
			zzScanR = (char **)calloc(vectorSize, sizeof(char *));
			zzScanG = (char **)calloc(vectorSize, sizeof(char *));
			zzScanB = (char **)calloc(vectorSize, sizeof(char *));


			for(i = 0; i < fullSquares ; i++){
				*zzScanB[i] = *deltaB[i].unicode;
				*zzScanG[i] = *deltaG[i].unicode;
				*zzScanR[i] = *deltaR[i].unicode;


				for(j=1;j<fullSquares;j++){
					zzScanB[j] = rleVectorsB[i];
					zzScanG[j] = rleVectorsB[i];
					zzScanR[j] = rleVectorsB[i];
				}
			}

			//recovering RLE vectors
			rleVectorsB = (char **)malloc(fullSquares*sizeof(char *));
			rleVectorsG = (char **)malloc(fullSquares*sizeof(char *));
			rleVectorsR = (char **)malloc(fullSquares*sizeof(char *));

			for(i=0; i<fullSquares; i++){
				rleVectorsB[i] = (char *)malloc((2*fullSquares)*sizeof(char));
				rleVectorsG[i] = (char *)malloc((2*fullSquares)*sizeof(char));
				rleVectorsR[i] = (char *)malloc((2*fullSquares)*sizeof(char));
			}
			

			for(i=0; i<fullSquares; i++){
				fread(&rleVectorsB[i], sizeof(rleVectorsB[i]), 1, src);
				fread(&rleVectorsG[i], sizeof(rleVectorsG[i]), 1, src);
				fread(&rleVectorsR[i], sizeof(rleVectorsR[i]), 1, src);
			}

			//recovering zigzag vectors
			zzScanB = (char **)calloc(vectorSize, sizeof(char *));
			zzScanG = (char **)calloc(vectorSize, sizeof(char *));
			zzScanR = (char **)calloc(vectorSize, sizeof(char *));


			for(i = 0; i < fullSquares; i++){
				zzScanB[i] = (char *)malloc(vectorSize*sizeof(char));
				zzScanG[i] = (char *)malloc(vectorSize*sizeof(char));
				zzScanR[i] = (char *)malloc(vectorSize*sizeof(char));
				
				zzScanB[i][0] = binary2int((char *)&deltaB[i].unicode);
				zzScanG[i][0] = binary2int((char *)&deltaG[i].unicode);
				zzScanR[i][0] = binary2int((char *)&deltaR[i].unicode);
				
				for(j = 1; j < fullSquares; j++){
					zzScanB[i][j] = rleVectorsB[i][j-1];
					zzScanG[i][j] = rleVectorsG[i][j-1];
					zzScanR[i][j] = rleVectorsR[i][j-1];
				}
			}

		//recovering matrices

			matrixR = (char ***)malloc(vectorSize*sizeof(char **));
			matrixG = (char ***)malloc(vectorSize*sizeof(char **));
			matrixB = (char ***)malloc(vectorSize*sizeof(char **));

			for(i = 0; i < vectorSize; i++){

				matrixB[i] = zigzagUndo(zzScanB[i]);
				matrixG[i] = zigzagUndo(zzScanG[i]);
				matrixR[i] = zigzagUndo(zzScanR[i]);

			}

			//quantization
			for(k=0; k < fullSquares; k++){

				for(i=0; i<8; i++){

					for(j=0; j<8; j++){
						
						matrixB[k][i][j] *= qzTable[i][j];
						matrixG[k][i][j] *= qzTable[i][j];
						matrixR[k][i][j] *= qzTable[i][j];
					}
				}
			}

			//IDCT
			for(k=0; k<fullSquares; k++){
				IDCT(matrixB[i]);
				IDCT(matrixG[i]);
				IDCT(matrixR[i]);
			}

			//allocating img
			img = (PIXEL_T *)malloc(((bmpHeader->biWidth)*(bmpHeader->biHeight))*sizeof(PIXEL_T));

			for(i=0; i<((bmpHeader->biWidth)*(bmpHeader->biHeight)); i++){

				//iteract through matrices
				for(j=0; j<fullSquares; j++){

					for(k=0; k<8; k++){

						for(l=0; l<8; l++){
							img[i].B = matrixB[j][k][l];
							img[i].G = matrixG[j][k][l];
							img[i].R = matrixR[j][k][l];
						}

					}

				}

			}

			//write new fileimg
			outfile = fopen("extracted.bin", "wb");
			writeBitmapHeader(bmpHeader, outfile);
			
			for(i=0;i<(bmpHeader->biHeight*bmpHeader->biWidth); i++){
				fwrite(&img[i].B, sizeof(unsigned char), 1, outfile);
				fwrite(&img[i].G, sizeof(unsigned char), 1, outfile);
				fwrite(&img[i].R, sizeof(unsigned char), 1, outfile);

			}

		}
	}

	//free content
	
	fclose(outfile);
	//delta
	free(deltaB);
	free(deltaG);
	free(deltaR);
		
	free(deltaInputB);
	free(deltaInputG);
	free(deltaInputR);

	//RLE
		for(i = 0; i < fullSquares ; i++){
			free(rleVectorsB[i]); 
			free(rleVectorsG[i]); 
			free(rleVectorsR[i]);
		}

		free(rleVectorsB); 
		free(rleVectorsG); 
		free(rleVectorsR);
	
	//zzScanVector
	
		for(i = 0; i < fullSquares ; i++){
			free(zzScanR[i]);
			free(zzScanG[i]);
			free(zzScanB[i]);
		}
		free(zzScanR);
		free(zzScanG);
		free(zzScanB);

		//DCT matrix

		for(i = 0; i < 8 ; i++){
			for(j = 0; j < 8 ; j++){
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


	//bitmapHeader
		freeBitmapHeader(&bmpHeader);

	//deallocate img
		free(img);
		fclose(src);
		return EXIT_SUCCESS;
}
