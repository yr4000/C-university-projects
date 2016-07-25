/*
 * sp_image_proc_util.cpp
 *
 *  Created on: 26 באפר 2016
 *      Author: Yair
 */
#include "sp_image_proc_util.h"
#include "stdlib.h"
#include "main_aux.h"
#include <math.h>
#include <opencv2/highgui.hpp> //imshow, drawKeypoints, waitKey
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>//Mat
#include <opencv2/xfeatures2d.hpp>//SiftDescriptorExtractor
#include <opencv2/features2d.hpp>
#include <vector>
#include <opencv2/imgproc.hpp>//calcHist
#include <opencv2/core.hpp>//Mat
#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>
using namespace cv;

//compareDoubles recieves two doubles and return 1,-1 or zero if a is bigger, smaller or equal to b.
// this function needs to be transfered to another file as was demanded.
int compareDoubles(const void* a, const void* b){  //how does that thing works??
	if(*(double*)a-*(double*)b>0) return 1;
	else if(*(double*)a-*(double*)b<0) return -1;
	else return 0;
}

/*
 * Calculates the RGB channels histogram. The histogram will be stored in a
 * two dimensional array of dimensions 3XnBins . The first row is the
 * red channel histogram, the second row is the green channel histogram
 * and the third row is the blue channel histogram.
 *
 * @param str - The path of the image for which the histogram will be calculated
 * @param nBins - The number of subdivision for the intensity histogram
 * @return NULL if str is NULL or nBins <= 0 or allocation error occurred,
 *  otherwise a two dimensional array representing the histogram.
 */
int** spGetRGBHist(char* str, int nBins){

		Mat src;

			/// Load image
			src = imread(str, CV_LOAD_IMAGE_COLOR);

			if (src.empty()) {
				return NULL;
			}

			/// Separate the image in 3 places ( B, G and R )
			std::vector<Mat> bgr_planes;
			split(src, bgr_planes);


			/// Set the ranges ( for B,G,R) )
			float range[] = { 0, 256 };
			const float* histRange = { range };

			/// Set the other parameters:
			int nImages = 1;


			//Output
			Mat b_hist, g_hist, r_hist;

			/// Compute the histograms:
			/// The results will be store in b_hist,g_hist,r_hist.
			/// The output type of the matrices is CV_32F (float)
			calcHist(&bgr_planes[0], nImages, 0, Mat(), b_hist, 1, &nBins, &histRange);
			calcHist(&bgr_planes[1], nImages, 0, Mat(), g_hist, 1, &nBins, &histRange);
			calcHist(&bgr_planes[2], nImages, 0, Mat(), r_hist, 1, &nBins, &histRange);

			int m;
			int n;
			int** result= (int**)malloc(sizeof(int*)*3);
			if(result==NULL){
				return NULL;
			}
			for(m=0; m<3; m++){
				result[m]= (int*)malloc(sizeof(int)*nBins);
				//here we free the memory in case the allocation failed.
				if(result[m]==NULL){
					for(int i=0;i<m;i++){
						free(result[i]);
					}
					free(result);
					return NULL;
				}
				for(n=0; n< nBins; n++){
					if(m==0) result[m][n]= (int)r_hist.at<float>(n);
					else if(m==1) result[m][n]= (int)g_hist.at<float>(n);
					else result[m][n]= (int)b_hist.at<float>(n);
				}
				}
			return result;
}

/**
 * Returns the average L2-squared distance between histA and histB. The
 * histA and histB are histograms of the RGB channels. Both histograms
 * must be in the same dimension (3 X nBins).
 * @param histA - RGB histogram of image A
 * @param histB - RGB histogram of image B
 * @return -1 if nBins <= 0 or histA/histB is null, otherwise the average L@-squared distance.
 */
double spRGBHistL2Distance(int** histA, int** histB, int nBins){
	if (nBins <= 0 || histA== NULL || histB == NULL){
			return -1;
		}
		double red_sum=0;
		double green_sum=0;
		double blue_sum=0;
		double result=0;

		for(int i=0; i<nBins; i++){
			red_sum += pow((double)histA[0][i] - (double)histB[0][i], 2);
			green_sum += pow((double)histA[1][i] - (double)histB[1][i], 2);
			blue_sum += pow((double)histA[2][i] - (double)histB[2][i], 2);
		}
		result = red_sum* 0.33 + green_sum* 0.33 + blue_sum* 0.33;
		return result;
}

/**
 * Extracts AT MOST maxNFeatures SIFT descriptors from the image given by str.
 * The result will be stored in two dimensional matrix with nFeatures
 * rows and 128 columns. Each row represents a SIFT feature corresponding
 * to some keypoint.
 *
 * @param str - A string representing the path of the image
 * @param maxNFeautres - The max number of features features to retain
 * @param nFeatures - A pointer in which the actual number of features retained
 * will be stored.
 * @return NULL in case of:
 * 		   - str is NULL
 * 		   - the image given by str didn't open
 * 		   - nFeatures is NULL
 * 		   - maxNFeatures <= 0
 * 		   - Memory allocation failure
 * 		   otherwise, the total number of features retained will be stored in
 * 		   nFeatures, and the actual features will be returned.
 */
double** spGetSiftDescriptors(char* str, int maxNFeatures, int *nFeatures){
	int ONE_HUNDRED_TWENTY_EIGHT = 128;
	double **Arr2D = NULL;
	//this is the code from TIRGUL
	cv::Mat src;
	src = cv::imread(str, CV_LOAD_IMAGE_GRAYSCALE);
	if(src.empty()){
		return NULL;
	}
	//Key points will be stored in kp1;
		std::vector<cv::KeyPoint> kp1;
		//Feature values will be stored in ds1;
		cv::Mat ds1;
		//Creating  a Sift Descriptor extractor
		cv::Ptr<cv::xfeatures2d::SiftDescriptorExtractor> detect =
				cv::xfeatures2d::SIFT::create(maxNFeatures);
		//Extracting features
		//The features will be stored in ds1
		//The output type of ds1 is CV_32F (float)
		detect->detect(src, kp1, cv::Mat());
		detect->compute(src, kp1, ds1);
		//ds1.convertTo(ds1, CV_64F);

		//here we start build the wanted matrix
		int i;
		int j;
		*nFeatures = ds1.rows;
		Arr2D = (double **)malloc(sizeof(double*)*(*nFeatures));
		if(Arr2D==NULL){
			return NULL;
		}
		double* Arr = (double *) malloc(sizeof(double)*ONE_HUNDRED_TWENTY_EIGHT*(*nFeatures));
		if(Arr==NULL){
			free(Arr2D);
			return NULL;
		}
		for(i=0;i<*nFeatures;i++){
			Arr2D[i]= Arr+i*ONE_HUNDRED_TWENTY_EIGHT;
			for(j=0;j<ONE_HUNDRED_TWENTY_EIGHT;j++){
				Arr2D[i][j]= (double)ds1.at<float>(i,j);
			}
		}
		return Arr2D;
}

/**
 * Calculates the L2-Square distance of the two features: featureA & featureB
 *
 * @param featureA - The first SIFT feature
 * @param featureB - The second SIFT feature
 * @return -1 in case featureA or featureB is NULL, otherwise the L2-Squared distance
 * as given in the assignment instructions
 */
double spL2SquaredDistance(double* featureA, double* featureB){
	double sum=0;
	if (featureA == NULL ||featureB == NULL )
		return -1;
	for(int i=0;i<128;i++){
		sum += pow(featureA[i]-featureB[i],2);
	}
	return sum;
}

/**
 * Given sift descriptors of the images in the database (databaseFeatures), finds the
 * closest bestNFeatures to a given SIFT feature (featureA). The function returns the
 * INDEXES of the images to which the closest features belong, stored in ascending order
 * (Closest feature image index is first, second closest feature image index is second, etc...).
 * Assumptions:
 *   - Tie break - In case featureA has the same distance (L2Squared distance) from two features,
 *     then the feature that corresponds to the smallest image
 *     index in the database is closer.
 *
 *   - The returned result may contain duplicates in case two features belongs to the same image.
 *
 *   - databaseFeatures is an array of two dimensional arrays, the number of elements
 *     in databaseFeatures is numberOfImages.
 *
 *   - Each entry in databaseFeatures corresponds to the features of some image in the database.
 *     The ith entry corresponds to the features of image_i in the database, and it is a two dimensional
 *     array of dimension nFeaturesPerImage[i]X128.
 *
 *   - The number of descriptors for the ith image is nFeaturesPerImage[i]
 *
 * @param bestNFeatures     - The number of indexes to return.
 * @param featureA          - A sift descriptor which will be compared with the other descriptor
 * 							  (Assumption dimension(bestNFeatures) = 128)
 * @param databaseFeatures  - An array of two dimensional array, in which the descriptors of the images are stored.
 * 							  The ith entry of the array corresponds to the features of the ith image in the database
 * @param numberOfImages    - The number of images in the database. (Number of entries in databaseFeatures)
 * @param nFeaturesPerImage - The number of features for each image. (i.e databaseFeatures[i] is two dimensional
 * 							  array with the dimension nFeaturesPerImage[i]X128
 * @return - NULL if either the following:
 * 			 * featureA is NULL
 * 			 * databaseFeatures is NULL
 * 			 * numberOfImages <= 1
 * 			 * nFeaturesPerImage is NULL
 * 			 * allocation error occurred
 * 			 otherwise, an array of size bestNFeatures is returned such that:
 * 			 * Given that f1, f2, ... the closest features to featureA (i.e d(featureA,f1) <= d(featureA,f2) <= ...)
 * 			 * i1, i2, .... are the indexes of the images to which fi belongs (i.e f1 is a SIFT descriptor of image i1,
 * 			   f2 is a SIFT descriptor of image i2 etc..)
 * 			 Then the array returned is {i1,i2,...,i_bestNFeatures}
 */
int* spBestSIFTL2SquaredDistance(int bestNFeatures, double* featureA,
		double*** databaseFeatures, int numberOfImages,
		int* nFeaturesPerImage){
	//here we initialise all the arrays we need
	int i;
	int j;
	int sumFeatures=0;
	for(i=0;i<numberOfImages;i++){
		sumFeatures+=nFeaturesPerImage[i];
	}
	//initialise the arrays we need and checks for allocation failure;
	double** Collection =(double**) malloc(numberOfImages*sumFeatures*sizeof(double*));
	if(Collection==NULL) return NULL;
	int* Besties = (int*) calloc(bestNFeatures,sizeof(int));
	if(Besties==NULL){
		free(Collection);
		return NULL;
	}
	//here we prepare the Matrix Collection, which contains for every image an array
	//with the spL2SquaredDistansce between featureA to every feature of that image
	int k=0;
	for(i=0;i<numberOfImages;i++){
		for(j=0;j<nFeaturesPerImage[i];j++){
			Collection[k] = (double*) malloc(2*sizeof(double));
			//checks for memory allocation failure
			if(Collection[k]==NULL){
				int l;
				for(l=0;l<k;l++){
					free(Collection[l]);
				}
				free(Collection);
				free(Besties);
				return NULL;
			}
			Collection[k][0] = (double)i;
			Collection[k][1] = spL2SquaredDistance(featureA,databaseFeatures[i][j]); //O(nFeaturesPerImage*numberOfImages*128)
			k++;
		}
	}
	//here we sort the array of features for every image,
	//and insert the closest ones into Besties.
	qsort(Collection,numberOfImages*(*nFeaturesPerImage),sizeof(double*),doubleArrayCompare); //O(numberOfImages*nFeaturesPerImage*log(nFeaturesPerImage) is it good? this code works
	for(i=0;i<5;i++){
		Besties[i] = (int)Collection[i][0];
	}
	//we don't need Collection anymore so we let it go let it go.
	for(i=0;i<numberOfImages*(*nFeaturesPerImage);i++){
		free(Collection[i]);
	}
	free(Collection);
	//in the end Besties should contain the number of the images with the closest features.
	return Besties;
}

