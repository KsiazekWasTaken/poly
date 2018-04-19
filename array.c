/** @file
   Implementacja tablicy dynamicznej.
   Przerobiona wersja tablicy dynamicznej z zadania Drzewa.
   @author Piotr Książek
   @copyright Uniwersytet Warszawski
   @date 2017-04-24, 2017-05-26
*/
//
// Created by pk371141 on 2017-03-26.
//

#include "array.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void init(ArrayH * H){
	H->maxIndex = 0;
	H->lastIndex = -1;
	H->array = NULL;
}

void append(ArrayH * H, Mono x){
	if(H->maxIndex-1 == (H->lastIndex)){
		if(H->maxIndex == 0)H->maxIndex = H->maxIndex+1;
		else H->maxIndex = H->maxIndex  << 1;
		H->array = realloc(H->array, (sizeof(Mono)*(H->maxIndex)) );
        assert(H->array!=NULL);
		(H->lastIndex)++;
		H->array[H->lastIndex] = x;
	}
	else{
		(H->lastIndex)++;
		(H->array)[H->lastIndex] = x;
	}
}
void freeArray(ArrayH *H){
	free(H->array);
	H->maxIndex = 0;
	H->lastIndex = -1;
	H->array = NULL;
}
