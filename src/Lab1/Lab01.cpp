// Demo1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "opencv2/opencv.hpp" 
#include "opencv2/highgui/highgui.hpp" 
#include <opencv2\core\core.hpp>
#include "ColorTransformer.h"
#include "Converter.h"
#include<string>
using namespace cv;
using namespace std;
int main(int argc, char* argv[])
{
	char *inputpath = argv[1];
	char *option = argv[2];
	ColorTransformer G;
	
	Converter C;
	if (strcmp(option, "hsv2rgb") == 0) {
		Mat imagehsv = imread(inputpath, IMREAD_UNCHANGED);//anh mau
		Mat deshsv;
		C.HSV2RGB(imagehsv, deshsv);
		imshow("HSV", imagehsv);
		waitKey(0);
		imshow("RGB", deshsv);
		waitKey(0);
	}
	if (strcmp(option, "rgb2hsv") == 0) {
		Mat imgargb2 = imread(inputpath, IMREAD_UNCHANGED);//anh mau
		Mat deshsv2;
		C.RGB2HSV(imgargb2, deshsv2);
		imshow("RGB", imgargb2);
		waitKey(0);
		imshow("RGB", deshsv2);
		waitKey(0);
	}
	if (strcmp(option, "rgb2gray") == 0) {
		Mat imagergb = imread(inputpath, IMREAD_UNCHANGED);//anh mau
		Mat destinationImagegray = Mat::zeros(imagergb.size(),CV_8UC1);
		C.RGB2GrayScale(imagergb, destinationImagegray);
		imshow("Show Image RGB2Gray", destinationImagegray);
		waitKey(0);
	}
	if (strcmp(option, "gray2rgb") == 0) {
		Mat imagegray = imread(inputpath, CV_LOAD_IMAGE_GRAYSCALE);//anh mau
		Mat destinationImagergb;
		C.GrayScale2RGB(imagegray, destinationImagergb);
		imshow("Show Image Gray@RGB", destinationImagergb);
		waitKey(0);
	}

	if (strcmp(option, "bright")==0) {
		//Do sang cua anh
		short b = atoi(argv[3]);
		Mat image0 = imread(inputpath, IMREAD_UNCHANGED);//anh mau
		Mat destinationImage0;
		G.ChangeBrighness(image0, destinationImage0, b);
		imshow("Show Image Brighness", destinationImage0);
		waitKey(0);
	}
	
	if (strcmp(option, "contrast") == 0) {
		//Do tuong phan cua anh
		float a = atoi(argv[3]);
		Mat image1 = imread(inputpath, IMREAD_UNCHANGED);//anh mau
		Mat destinationImage1;
		G.ChangeContrast(image1, destinationImage1, a);
		imshow("Show Image Contrast", destinationImage1);
		waitKey(0);
	}
	//Tinh histogram cua anh
	if (strcmp(option, "hist") == 0) {
		Mat image00 = imread(inputpath, IMREAD_UNCHANGED);//anh mau
		Mat image000 = imread(inputpath, CV_LOAD_IMAGE_GRAYSCALE);//anh xam
		vector<double> histMatrix00;
		histMatrix00 = G.CalculatorHistogram(image000, histMatrix00);
		vector<double> histMatrix000;
		histMatrix000 = G.CalculatorHistogram(image000, histMatrix000);
		cout << "Image Color" << endl;
		for (int i = 0; i < histMatrix00.size(); i++)
			cout << histMatrix00[i] << " ";
		cout << endl << "Image Gray" << endl;
		for (int i = 0; i < histMatrix000.size(); i++)
			cout << histMatrix000[i] << " ";
	}
	if (strcmp(option, "equalhist") == 0) {
		//Can bang histogram
		Mat image11 = imread(inputpath, IMREAD_UNCHANGED);//anh mau
		Mat image111 = imread(inputpath, CV_LOAD_IMAGE_GRAYSCALE);//anh xam
		Mat destinationImageCL;
		Mat destinationImage;
		G.HistogramEqualization(image11, destinationImageCL);
		G.HistogramEqualization(image111, destinationImage);
		imshow("Show Image equalhist Color", destinationImageCL);
		waitKey(0);
		imshow("Show Image equalhist Gray", destinationImage);
		waitKey(0);
	}
	if (strcmp(option, "drawhist") == 0) {
		//Draw histogram
		Mat image22 = imread(inputpath, IMREAD_UNCHANGED);//anh mau
		Mat image222 = imread(inputpath, CV_LOAD_IMAGE_GRAYSCALE);//anh xam
		Mat HistImage22;
		Mat HistImage222;
		vector<double> histMatrix22;
		vector<double> histMatrix222;
		histMatrix22 = G.CalculatorHistogram(image22, histMatrix22);
		histMatrix222 = G.CalculatorHistogram(image222, histMatrix222);
		G.DrawHistogram(histMatrix22, HistImage22);
		imshow("Show Hist Image color", HistImage22);
		waitKey(0);
		G.DrawHistogram(histMatrix222, HistImage222);
		imshow("Show Hist Image GrayScale", HistImage222);
		waitKey(0);
	}
	if (strcmp(option, "compare") == 0) {
		Mat image3 = imread(inputpath, IMREAD_UNCHANGED);
		char *inputpath3 = argv[3];
		Mat image33 = imread(inputpath3, IMREAD_UNCHANGED);
		float compare = G.CompareImage(image3, image33);
		cout << "Do giong nhau: " << compare << " %" << endl;
	}

	return 0;
}
