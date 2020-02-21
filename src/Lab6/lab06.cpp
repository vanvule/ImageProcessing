#include "stdafx.h"

#include <opencv2/opencv.hpp>

#include <iostream>
#include <string>

#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <list>

#define SIGN(a, b) ((b) >= 0.0 ? fabs(a) : -fabs(a))
#define MAX(x,y) ((x)>(y)?(x):(y))

using namespace cv;
using namespace std;

int writeMatUSV(fstream& outputFile, const Mat& outputMat);
int readMatUSV(fstream& inputFile, Mat& inputMat);

int main(int argc, char** argv)
{
	string command;
	command = argv[1];
	if (command=="--compress") {
		int k = atoi(argv[4]);
		Mat image = imread(argv[2], IMREAD_COLOR);
		if (image.empty())
		{
			cout << "Could not open or find the image" << endl;
			system("pause");
			return -1;
		}
		fstream fuB, fsB, fvB;
		fstream fuR, fsR, fvR;
		fstream fuG, fsG, fvG;
		fuB.open("uB.bin", ios::out | ios::binary);
		fsB.open("sB.bin", ios::out | ios::binary);
		fvB.open("vB.bin", ios::out | ios::binary);

		fuR.open("uR.bin", ios::out | ios::binary);
		fsR.open("sR.bin", ios::out | ios::binary);
		fvR.open("vR.bin", ios::out | ios::binary);

		fuG.open("uG.bin", ios::out | ios::binary);
		fsG.open("sG.bin", ios::out | ios::binary);
		fvG.open("vG.bin", ios::out | ios::binary);

		Mat R, G, B;
		Mat UB, SB, VB;
		Mat UR, SR, VR;
		Mat UG, SG, VG;
		vector<Mat> channels(3);
		split(image, channels);
		B = channels[0];
		G = channels[1];
		R = channels[2];
		B.convertTo(B, CV_32FC1, 1.0 / 255.0);
		G.convertTo(G, CV_32FC1, 1.0 / 255.0);
		R.convertTo(R, CV_32FC1, 1.0 / 255.0);

		SVD::compute(B, SB, UB, VB);
		Mat sigma_mB = Mat::zeros(SB.rows, SB.rows, CV_32FC1);
		for (int i = 0; i<SB.rows; i++) {
			sigma_mB.at<float>(i, i) = SB.at<float>(i);
		}

		writeMatUSV(fuB, UB);
		writeMatUSV(fsB, sigma_mB);
		writeMatUSV(fvB, VB);

		//testing
		cout << UB.at<float>(0, 1) << "\n";
		cout << sigma_mB.at<float>(1, 1) << "\n";
		cout << VB.at<float>(0, 1) << "\n";
		Mat test1 = UB*sigma_mB*VB;
		cout << test1.at<float>(0, 1) << "\n";

		sigma_mB = sigma_mB(Range(0, k), Range(0, k));
		UB = UB(Range::all(), Range(0, k));
		VB = VB(Range(0, k), Range::all());
		sigma_mB = sigma_mB(Range(0, k), Range(0, k));
		UB = UB(Range::all(), Range(0, k));
		VB = VB(Range(0, k), Range::all());
		Mat ResultB = UB*sigma_mB*VB;
		writeMatUSV(fuB, UB);

		SVD::compute(R, SR, UR, VR);
		Mat sigma_mR = Mat::zeros(SR.rows, SR.rows, CV_32FC1);
		for (int i = 0; i<SR.rows; i++) {
			sigma_mR.at<float>(i, i) = SR.at<float>(i);
		}

		writeMatUSV(fuR, UR);
		writeMatUSV(fsR, sigma_mR);
		writeMatUSV(fvR, VR);

		sigma_mR = sigma_mR(Range(0, k), Range(0, k));
		UR = UR(Range::all(), Range(0, k));
		VR = VR(Range(0, k), Range::all());
		sigma_mR = sigma_mR(Range(0, k), Range(0, k));
		UR = UR(Range::all(), Range(0, k));
		VR = VR(Range(0, k), Range::all());
		Mat ResultR = UR*sigma_mR*VR;

		SVD::compute(G, SG, UG, VG);
		Mat sigma_mG = Mat::zeros(SG.rows, SG.rows, CV_32FC1);
		for (int i = 0; i<SG.rows; i++) {
			sigma_mG.at<float>(i, i) = SG.at<float>(i);
		}

		writeMatUSV(fuG, UG);
		writeMatUSV(fsG, sigma_mG);
		writeMatUSV(fvG, VG);

		sigma_mG = sigma_mG(Range(0, k), Range(0, k));
		UG = UG(Range::all(), Range(0, k));
		VG = VG(Range(0, k), Range::all());
		sigma_mG = sigma_mG(Range(0, k), Range(0, k));
		UG = UG(Range::all(), Range(0, k));
		VG = VG(Range(0, k), Range::all());
		Mat ResultG = UG*sigma_mG*VG;

		vector<Mat> multi_channel_image;
		multi_channel_image.push_back(ResultB);
		multi_channel_image.push_back(ResultG);
		multi_channel_image.push_back(ResultR);
		Mat Result;
		merge(multi_channel_image, Result);

		imshow("test2", Result);
		//Result.convertTo(Result, CV_8UC3, 255.0);
		imwrite(argv[3], Result);
		Mat a = imread(argv[3], IMREAD_COLOR);
		imshow("Original", image);
		imshow("Result", a);
		waitKey(0);
	}
	else if (command == "--decompress") {
		Mat image = imread(argv[2], IMREAD_COLOR);
		if (image.empty())
		{
			cout << "Could not open or find the image" << endl;
			system("pause");
			return -1;
		}

		fstream fuB, fsB, fvB;
		fstream fuR, fsR, fvR;
		fstream fuG, fsG, fvG;
		fuB.open("uB.bin", ios::in | ios::binary);
		fsB.open("sB.bin", ios::in | ios::binary);
		fvB.open("vB.bin", ios::in | ios::binary);

		fuR.open("uR.bin", ios::in | ios::binary);
		fsR.open("sR.bin", ios::in | ios::binary);
		fvR.open("vR.bin", ios::in | ios::binary);

		fuG.open("uG.bin", ios::in | ios::binary);
		fsG.open("sG.bin", ios::in | ios::binary);
		fvG.open("vG.bin", ios::in | ios::binary);

		Mat R, G, B;
		Mat UB, SB, VB;
		Mat UR, SR, VR;
		Mat UG, SG, VG;

		readMatUSV(fuB, UB);
		readMatUSV(fsB, SB);
		readMatUSV(fvB, VB);
		readMatUSV(fuR, UR);
		readMatUSV(fsR, SR);
		readMatUSV(fvR, VR);
		readMatUSV(fuG, UG);
		readMatUSV(fsG, SG);
		readMatUSV(fvG, VG);

		cout << UB.at<float>(0, 1) << "\n";
		cout << SB.at<float>(1, 1) << "\n";
		cout << VB.at<float>(0, 1) << "\n";
		Mat ResultR = UR*SR*VR;
		Mat ResultB = UB*SB*VB;
		Mat ResultG = UG*SG*VG;
		cout << ResultB.at<float>(0, 1) << "\n";

		vector<Mat> multi_channel_image;
		multi_channel_image.push_back(ResultB);
		multi_channel_image.push_back(ResultG);
		multi_channel_image.push_back(ResultR);
		Mat Result;
		merge(multi_channel_image, Result);

		Result.convertTo(Result, CV_8UC3, 255.0);
		imwrite(argv[3], Result);
		Mat a = imread(argv[3], IMREAD_COLOR);
		imshow("Compress", image);
		imshow("Original", a);
		waitKey(0);
	}
	return 0;
}
int writeMatUSV(fstream& outputFile, const Mat& outputMat) {
	if (!outputFile.is_open()) return 0;
	if (outputMat.empty()) {
		int s = 0;
		outputFile.write((const char*)(&s), sizeof(int));
		return 1;
	}
	int type = outputMat.type();
	outputFile.write((const char*)(&outputMat.rows), sizeof(int));
	outputFile.write((const char*)(&outputMat.cols), sizeof(int));
	outputFile.write((const char*)(&type), sizeof(int));
	for (int i = 0; i < outputMat.rows; i++)
		for (int j = 0; j < outputMat.cols; j++)
			outputFile.write((const char *)&outputMat.at<float>(i, j), sizeof(float));
	return 1;
}
int readMatUSV(fstream& inputFile, Mat& inputMat) {
	if (!inputFile.is_open()) return 0;
	int rows, cols, type;
	float val;
	inputFile.read((char*)(&rows), sizeof(int));
	inputFile.read((char*)(&cols), sizeof(int));
	inputFile.read((char*)(&type), sizeof(int));

	inputMat.release();
	inputMat.create(rows, cols, type);
	for (int i = 0; i< rows; i++)
		for (int j = 0; j < cols; j++)
		{
			inputFile.read((char *)&val, sizeof(val));
			inputMat.at<float>(i, j) = val;
		}
	return 1;
}