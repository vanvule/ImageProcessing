#include "stdafx.h"
#include "testing.h"


int ColorTransformer::ChangeBrighness(const Mat& sourceImage, Mat& destinationImage, short b) {

	if (sourceImage.empty()) // Check for failure
	{
		return 0;
	}
	for (int y = 0; y < sourceImage.rows; y++) {
		for (int x = 0; x < sourceImage.cols; x++) {
			for (int c = 0; c < sourceImage.channels(); c++) {
				destinationImage.at<Vec3b>(y, x)[c] =
					saturate_cast<uchar>(sourceImage.at<Vec3b>(y, x)[c] + b);
			}
		}
	}
	return 1;
}

int ColorTransformer::ChangeContrast(const Mat& sourceImage, Mat& destinationImage, float c) {
	if (sourceImage.empty()) // Check for failure
	{
		return 0;
	}
	for (int y = 0; y < sourceImage.rows; y++) {
		for (int x = 0; x < sourceImage.cols; x++) {
			for (int c = 0; c < sourceImage.channels(); c++) {
				destinationImage.at<Vec3b>(y, x)[c] =
					saturate_cast<uchar>(c*sourceImage.at<Vec3b>(y, x)[c]);
			}
		}
	}
	return 1;
}

ColorTransformer::ColorTransformer(){}
ColorTransformer::~ColorTransformer(){}