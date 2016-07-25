/*
 * main_aux.h
 *
 *  Created on: 2 במאי 2016
 *      Author: Yair
 */

#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_

// compare function for qsort.
int doubleArrayCompare(const void* x, const void* y);


int intArrayCompare(const void* x, const void* y);

int freeSpGetRGBHist(int*** RGBhist,int pic_num);

void free2Dint(int** int2D,int n);

int freeSpGetSiftDescriptors(double*** SiftDescriptors,int pic_num);

void free2Ddouble(double** doubleD,int nFeatures);

int freeAll(int*** RGBhist,double*** SiftDescriptors,int pic_num);


#endif /* MAIN_AUX_H_ */
