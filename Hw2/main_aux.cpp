/*
 * main_aux.cpp
 *
 *  Created on: 2 במאי 2016
 *      Author: Yair
 */
#include "stdlib.h"
#include <stdlib.h>
#include <stdio.h>
#include "main_aux.h"

int doubleArrayCompare(const void* x, const void* y){
	double* arrayX = *(double**) x;
	double* arrayY = *(double**) y;

	if(arrayX[1]>arrayY[1]) return 1;
	if(arrayX[1]<arrayY[1]) return -1;
	else{
		if(arrayX[0]>arrayY[0]) return 1;
		else if(arrayX[0]<arrayY[0]) return -1;
	}

	return 0;
}

int intArrayCompare(const void* x, const void* y){
	int* arrayX = *(int**) x;
	int* arrayY = *(int**) y;

	if(arrayX[1]>arrayY[1]) return 1;
	else if(arrayX[1]<arrayY[1]) return -1;
	else{
		if(arrayX[0]>arrayY[0]) return 1;
		else if(arrayX[0]<arrayY[0]) return -1;
	}

	return 0;
}

//TODO delete testers
//releasing memory allocations of histograms of n pictures.
int freeSpGetRGBHist(int*** RGBhist,int pic_num){
	//printf("got in freeSpGetRGBHist\n");
	int j;
	for(j=0; j<pic_num;j++){
		free2Dint(RGBhist[j],3);
		}
		free(RGBhist);
		//printf("freeSpGetRGBHist succeed!\n");
		return -1;
}

//releasing memory allocation of histogram of 1 picture.
void free2Dint(int** int2D,int n){
	int i;
	for(i=0;i<n;i++){
		free(int2D[i]);
		}
	free(int2D);
}

//releasing memory allocations of features of n pictures.
int freeSpGetSiftDescriptors(double*** SiftDescriptors,int pic_num){
	//printf("got in freeSpGetSiftDescriptors\n");
	int l;
	for(l=0;l<pic_num;l++){
		free(*SiftDescriptors[l]);
		free(SiftDescriptors[l]);
	}
	free(SiftDescriptors);
	//printf("freeSpGetSiftDescriptors succeed!\n");
	return -1;
}

////releasing memory allocation of two dimensional array of doubles.
void free2Ddouble(double** doubleD,int nFeatures){
	int k;
	for(k=0;k<nFeatures;k++){
		if(doubleD[k]==NULL) break;
		free(doubleD[k]);
	}
	free(doubleD);
}

int freeAll(int*** RGBhist,double*** SiftDescriptors,int pic_num){
	freeSpGetRGBHist(RGBhist,pic_num);
	freeSpGetSiftDescriptors(SiftDescriptors,pic_num);
	return -1;
}

