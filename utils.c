#include "utils.h"

void quicksortColors(unsigned char *colors, int *repetitions, int len){
  if (len < 2) return;
 
  int pivot = repetitions[len / 2];
  int tempR;
  unsigned char tempC;
 
  int i, j;
  for (i = 0, j = len - 1; ; i++, j--) {
    while (repetitions[i] < pivot) i++;
    while (repetitions[j] > pivot) j--;
 
    if (i >= j) break;
 
	//swapping repetitions vector
    tempR = repetitions[i];
	tempC = colors[i];

    repetitions[i] = repetitions[j];
    repetitions[j] = tempR;

    colors[i] = colors[j];
    colors[j] = tempC;
  }
 
  quicksortColors(colors, repetitions, i);
  quicksortColors(colors, repetitions + i, len - i);
}
