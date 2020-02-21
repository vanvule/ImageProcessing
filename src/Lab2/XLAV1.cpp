#include "stdafx.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include "testing.h"
#include"GeometricTransformer.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	Mat srcImage = imread("aa.jpg",CV_LOAD_IMAGE_COLOR);
	Mat dstImage;
	dstImage = Mat::zeros(srcImage.size(), srcImage.type());
	if (srcImage.empty()) cout << "0\n";
	else {
		GeometricTransformer testing;
		NearestNeighborInterpolate test1;
		ColorTransformer test2;
		test2.ChangeContrast(srcImage, dstImage, 0.5);
		imshow("Original Image", srcImage);
		imshow("New Image", dstImage);
		waitKey();
		return 0;
	}
}