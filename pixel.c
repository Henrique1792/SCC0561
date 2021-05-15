#include "pixel.h"



PIXEL_T *createPIXEL_T(){
	PIXEL_T *rt = (PIXEL_T *)malloc(sizeof(PIXEL_T));
	return (rt!=NULL ? rt : NULL);

}

void freePIXEL_T(PIXEL_T **tgt){
	if(tgt == NULL || *tgt == NULL)
		return;
	free(*tgt);
}

