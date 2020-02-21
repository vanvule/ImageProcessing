// Lab03.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "opencv2/opencv.hpp" 
#include "opencv2/highgui/highgui.hpp" 
#include <opencv2\core\core.hpp>
#include "Convolution.h"
#include "Blur.h"
#include "EdgeDetector.h"
using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	char *inputpath = argv[1];
	char *option = argv[2];
	Blur B;
	EdgeDetector e;
	if (strcmp(option, "mean") == 0) {
		Mat image_mean = imread(inputpath, CV_LOAD_IMAGE_GRAYSCALE);
		Mat desimage_mean;
		B.BlurImage(image_mean, desimage_mean, 3, 3, 0);
		imshow("show image", image_mean);
		waitKey(0);
		imshow("show image mean fillter", desimage_mean);
		waitKey(0);
	}
	if (strcmp(option, "median") == 0) {
		Mat image_median = imread(inputpath, CV_LOAD_IMAGE_GRAYSCALE);
		Mat desimage_median;
		B.BlurImage(image_median, desimage_median, 3, 3, 1);
		imshow("show image", image_median);
		waitKey(0);
		imshow("show image median fillter", desimage_median);
		waitKey(0);
	}
	if (strcmp(option, "gauss") == 0) {
		Mat image_gauss = imread(inputpath, CV_LOAD_IMAGE_GRAYSCALE);
		Mat desimage_gauss;
		B.BlurImage(image_gauss, desimage_gauss, 3, 3, 2);
		imshow("show image", image_gauss);
		waitKey(0);
		imshow("show image gauss fillter", desimage_gauss);
		waitKey(0);
	}
	if (strcmp(option, "sobel") == 0) {
		Mat src1 = imread(inputpath, CV_LOAD_IMAGE_GRAYSCALE);
		Mat dst1;
		e.DetectEdge(src1, dst1, 3, 3, 1);
		imshow("input", src1);
		waitKey(0);
		imshow("output", dst1);
		waitKey(0);
	}
	if (strcmp(option, "prewitt") == 0) {
		Mat src2 = imread(inputpath, CV_LOAD_IMAGE_GRAYSCALE);
		Mat dst2;
		e.DetectEdge(src2, dst2, 3, 3, 2);
		imshow("input", src2);
		waitKey(0);
		imshow("output", dst2);
		waitKey(0);
	}
	if (strcmp(option, "laplace") == 0) {
		Mat src3 = imread(inputpath, CV_LOAD_IMAGE_GRAYSCALE);
		Mat dst3;
		e.DetectEdge(src3, dst3,3,3,3);
		imshow("input", src3);
		waitKey(0);
		imshow("output", dst3);
		waitKey(0);
	}
    return 0;
}

