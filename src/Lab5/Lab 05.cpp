// Lab 05.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdafx.h"
#include <iostream>
#include <vector>
#include "opencv2/opencv.hpp" 
#include "opencv2/highgui/highgui.hpp" 
#include <opencv2\core\core.hpp>
#include "Threshold.h"
#include "Kmean.h"
using namespace cv;
using namespace std;


int main(int argc, char* argv[])
{
	char *inputpath = argv[1];
	char *option = argv[2];
	char *par1 = argv[3];
	char *par2 = argv[4];
	
	if (strcmp(option, "static") == 0) {
		StaticThreshold T;
		T.SetLow(atoi(argv[3]));
		T.SetHigh(atoi(argv[4]));
		Mat image0 = imread(inputpath, CV_LOAD_IMAGE_GRAYSCALE);
		Mat dstimage0;
		T.Apply(image0, dstimage0);
		imshow("Show Static Threshold Image", dstimage0);
		waitKey(0);
	}
	if (strcmp(option, "mean") == 0) {
		AverageLocalThreshold AV;
		AV.SetC(atoi(argv[3]));

		Mat image1 = imread(inputpath, CV_LOAD_IMAGE_GRAYSCALE);
		Mat dstimage1;
		Size winSize(3, 3);
		AV.Apply(image1, dstimage1, winSize);
		imshow("Show Mean Threshold Image", dstimage1);
		waitKey(0);
	}
	if (strcmp(option, "median") == 0) {
		MedianLocalThreshold M;
		M.SetC(atoi(argv[3]));
		Mat image2 = imread(inputpath, CV_LOAD_IMAGE_GRAYSCALE);
		Mat dstimage2;
		Size winSize1(3, 3);
		M.Apply(image2, dstimage2, winSize1);
		imshow("Show Median Threshold Image", dstimage2);
		waitKey(0);
	}
	if (strcmp(option, "sauvola") == 0) {
		SauvolaLocalThreshold S;
		S.SetK(atoi(argv[3]));
		S.SetR(atoi(argv[4]));
		Mat image3 = imread(inputpath, CV_LOAD_IMAGE_GRAYSCALE);
		Mat dstimage3;
		Size winSize2(3, 3);
		S.Apply(image3, dstimage3, winSize2);
		
		imshow("Show Sauvola Threshold Image", dstimage3);
		waitKey(0);
	}
	if (strcmp(option, "kmeans") == 0) {
		Kmean K;
		K.SetCluster(atoi(argv[3]));
		Mat image4 = imread(inputpath, CV_LOAD_IMAGE_COLOR);
		Mat dstimage4;
		K.Apply(image4, dstimage4);

		imshow("Show Kmean Image", dstimage4);
		waitKey(0);
	}
    return 0;
}

