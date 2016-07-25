#include <opencv2/highgui.hpp> //imshow, drawKeypoints, waitKey
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>//Mat
#include <opencv2/xfeatures2d.hpp>//SiftDescriptorExtractor
#include <opencv2/features2d.hpp>
#include <vector>
#include "sp_image_proc_util.h"
#include <stdlib.h>
#include "main_aux.h"


int main() {
	setbuf(stdout, NULL);
	int i;
	int j;
	//#1
	char dir_path[1024];//TODO: might me i need to add malloc to all this fellas.
	printf("Enter images directory path:\n");
	scanf("%s", dir_path);

	//#2
	char prefix[1024];
	printf("Enter images prefix:\n");
	scanf("%s", prefix);

	//#3
	int num_pic;
	printf("Enter number of images:\n");
	scanf("%d", &num_pic);
	if (num_pic < 1){
		printf("An error occurred - invalid number of images\n");
		exit(0);
	}

	//#4
	char suffix[1024];
	printf("Enter images suffix:\n");
	scanf("%s", suffix);

	//#5
	int nBins;
	printf("Enter number of bins:\n");
	scanf("%d", &nBins);
	if (nBins < 1){
		printf("An error occurred - invalid number of bins\n");
		exit(0);
	}

	//#6
	int nFeatures;
	printf("Enter number of features:\n");
	scanf("%d", &nFeatures);
	if (nFeatures < 1){
		printf("An error occurred - invalid number of features\n");
		exit(0);
	}

	//#7 here we create the data base
	int*** pics_hists = (int***)malloc(sizeof(int**)*num_pic);
	double*** pics_feats= (double***)malloc(sizeof(double**)*num_pic);
	int* realnFeatures = (int*) malloc(num_pic*sizeof(int));
	//here we free memory allocations in case of failure.
	if(pics_hists==NULL || pics_feats==NULL || realnFeatures==NULL){
		printf("An error occurred - allocation failure\n");
		if(pics_hists!=NULL) free(pics_hists);
		if(pics_feats!=NULL) free(pics_feats);
		if(realnFeatures!=NULL) free(realnFeatures);
		return -1;
	}
	int temp;
	for (i=0; i<num_pic ; i++){
		char pic_name[1024]="";
		strcat(pic_name,dir_path);
		strcat(pic_name,prefix);
		strcat(pic_name,"%d");
		sprintf(pic_name,pic_name,i); //strcat(pic_name,itoa(i,i_str,10));
		strcat(pic_name,suffix);
		// creating the pictures histogram database + memory allocation check
		pics_hists[i]=spGetRGBHist(pic_name, nBins);
		if(pics_hists[i]==NULL){
			freeSpGetRGBHist(pics_hists,i);
			printf("An error occurred - allocation failure\n");
			free(pics_feats); free(realnFeatures);
			return -1;
		}
		// creating the pictures SIFT database + memory allocation check
		pics_feats[i]=spGetSiftDescriptors(pic_name, nFeatures, &temp);
		if(pics_feats[i]==NULL){
			freeSpGetRGBHist(pics_hists,num_pic);
			freeSpGetSiftDescriptors(pics_feats,i);
			printf("An error occurred - allocation failure\n");
			free(realnFeatures);
			return -1;
		}
		// creating the pictures nFeatures database + memory allocation check
		realnFeatures[i] = temp;
	}

	//#8+9
	while(true){
	char query[1024];//TODO malloc?
	printf("Enter a query image or # to terminate:\n");
	scanf("%s", query);
	if (strcmp(query,"#") == 0 ){
		printf("Exiting...\n");
		freeAll(pics_hists,pics_feats,num_pic);
		free(realnFeatures);
		exit(0);
		}

	//here we initialise the data for the query and check for memory allocation failure.
	int qFeatures;
	int** query_hist= spGetRGBHist(query, nBins);
	if(query_hist==NULL){
		goto FREE_DATABASE;
	}
	double** query_feats= spGetSiftDescriptors(query, nFeatures, &qFeatures);
	if(query_feats==NULL){
		free2Dint(query_hist,3);
		goto FREE_DATABASE;
	}

	//#10
	//in this part we find the best 5 according to histogram
	double** hist_distance= (double**)malloc(sizeof(double*)*num_pic);
	double* hist_distance_data= (double*)malloc(sizeof(double)*num_pic*2);

	//memory allocation failure check
	if(hist_distance==NULL||hist_distance_data==NULL){
		if(hist_distance!=NULL) free(hist_distance);
		if(hist_distance_data!=NULL) free(hist_distance_data);
		free2Dint(query_hist,3);
		free(*query_feats);
		free(query_feats);
		goto FREE_DATABASE;
	}
	//caculate the distance between querys histogram to each picutres histogram.
	for(i=0;i<num_pic;i++){
		hist_distance[i] = hist_distance_data +2*i;
		for(j=0;j<2;j++){
				hist_distance[i][0]=i;
				hist_distance[i][1] = spRGBHistL2Distance(query_hist,pics_hists[i],nBins);
		}
	}

	//here we run 5 times over the whole distances, each time picking the minimum and then take it of
	//the array by changing it to -1.
	int bestRBG[5];
	double tempMin=0;
	int tempI=0;
	for(int n=0;n<5;n++){
		for(i=0;i<num_pic;i++){
			if(i==0){
				tempMin=hist_distance[i][1];
			}
			if(hist_distance[i][1]==-1) continue;
			if(tempMin==-1 || tempMin>hist_distance[i][1]){
				tempMin=hist_distance[i][1];
				tempI=i;
			}
		}
		bestRBG[n]=tempI;
		hist_distance[tempI][1]=-1;
	}
	printf("Nearest images using global descriptors:\n");
	for(i=0;i<4;i++){
		printf("%d, ",bestRBG[i]);
	}
	printf("%d\n",bestRBG[4]);

	//now we can free hist_distance
	free(*hist_distance);
	free(hist_distance);


	//here we find the best 5 according to SIFT features
	//first we initialise all the arrays we need, SIFTBesties contains all the best 5 distances
	//between each picture features to the query features
	int** SIFTBesties=(int**) malloc(sizeof(int*)*qFeatures);
	int** mostHits = (int**) malloc(sizeof(int*)*num_pic);
	int* mostHitsData = (int*) calloc(num_pic*2,sizeof(int));
	if(SIFTBesties==NULL || mostHits==NULL||mostHitsData==NULL){
		if(SIFTBesties!=NULL) free(SIFTBesties);
		if(mostHits!=NULL) free(mostHits);
		if(mostHitsData!=NULL) free(mostHitsData);
		free2Dint(query_hist,3);
		free(*query_feats);
		free(query_feats);
		goto FREE_DATABASE;
	}
	for(i=0;i<qFeatures;i++){
		SIFTBesties[i] =  spBestSIFTL2SquaredDistance(5,query_feats[i],pics_feats,num_pic,realnFeatures);
		if(SIFTBesties[i]==NULL){
			free2Dint(SIFTBesties,i);
			free(mostHits);
			free(mostHitsData);
			free2Dint(query_hist,3);
			free(*query_feats);
			free(query_feats);
			goto FREE_DATABASE;
		}
	}
	//initialising mostHits.
	for(i=0;i<num_pic;i++){
		mostHits[i]= mostHitsData+2*i;
		mostHits[i][0]=i;
	}

	//here we count for each index how many times it appeared in SIFTBesties
	for(i=0;i<qFeatures;i++){
		for(j=0;j<5;j++){
			mostHits[SIFTBesties[i][j]][1]++;
		}
	}

	qsort(mostHits,num_pic,sizeof(int*),intArrayCompare);
	printf("Nearest images using local descriptors:\n");
	for(i=0;i<4;i++){
		printf("%d, ",mostHits[num_pic-1-i][0]);
	}
	printf("%d\n",mostHits[num_pic-1-4][0]);

	//now we will free all the irrelevant data
	free2Dint(SIFTBesties,qFeatures);
	free(mostHitsData);
	free(mostHits);
	free2Dint(query_hist,3);
	free(*query_feats);
	free(query_feats);
	}

	FREE_DATABASE:
		printf("An error occurred - allocation failure\n");
		freeAll(pics_hists,pics_feats,num_pic);
		free(realnFeatures);
		return -1;
}
