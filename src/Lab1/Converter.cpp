#include "stdafx.h"
#include "Converter.h"
#include <iostream>
using namespace std;
#define min_f(a, b, c)  (fminf(a, fminf(b, c)))
#define max_f(a, b, c)  (fmaxf(a, fmaxf(b, c)))

void rgb2hsv(const unsigned char &src_r, const unsigned char &src_g, const unsigned char &src_b, unsigned char &dst_h, unsigned char &dst_s, unsigned char &dst_v)
{
	float r = src_r / 255.0f;
	float g = src_g / 255.0f;
	float b = src_b / 255.0f;

	float h, s, v; // h:0-360.0, s:0.0-1.0, v:0.0-1.0

	float max = max_f(r, g, b);
	float min = min_f(r, g, b);

	v = max;

	if (max == 0.0f) {
		s = 0;
		h = 0;
	}
	else if (max - min == 0.0f) {
		s = 0;
		h = 0;
	}
	else {
		s = (max - min) / max;

		if (max == r) {
			h = 60 * ((g - b) / (max - min)) + 0;
		}
		else if (max == g) {
			h = 60 * ((b - r) / (max - min)) + 120;
		}
		else {
			h = 60 * ((r - g) / (max - min)) + 240;
		}
	}

	if (h < 0) h += 360.0f;

	dst_h = (unsigned char)(h / 2);   // dst_h : 0-180
	dst_s = (unsigned char)(s * 255); // dst_s : 0-255
	dst_v = (unsigned char)(v * 255); // dst_v : 0-255
}

void hsv2rgb(const unsigned char &src_h, const unsigned char &src_s, const unsigned char &src_v, unsigned char &dst_r, unsigned char &dst_g, unsigned char &dst_b)
{
	float h = src_h *   2.0f; // 0-360
	float s = src_s / 255.0f; // 0.0-1.0
	float v = src_v / 255.0f; // 0.0-1.0

	float r, g, b; // 0.0-1.0

	int   hi = (int)(h / 60.0f) % 6;
	float f = (h / 60.0f) - hi;
	float p = v * (1.0f - s);
	float q = v * (1.0f - s * f);
	float t = v * (1.0f - s * (1.0f - f));

	switch (hi) {
	case 0: r = v, g = t, b = p; break;
	case 1: r = q, g = v, b = p; break;
	case 2: r = p, g = v, b = t; break;
	case 3: r = p, g = q, b = v; break;
	case 4: r = t, g = p, b = v; break;
	case 5: r = v, g = p, b = q; break;
	}

	dst_r = (unsigned char)(r * 255); // dst_r : 0-255
	dst_g = (unsigned char)(g * 255); // dst_r : 0-255
	dst_b = (unsigned char)(b * 255); // dst_r : 0-255
}


int Converter::RGB2GrayScale(const Mat& sourceImage, Mat& destinationImage) {
	destinationImage = sourceImage;
	if (sourceImage.empty() || sourceImage.channels() != 3) {
		return 0;
	}
	else {
		for (int i = 0; i<sourceImage.cols; i++) {
			for (int j = 0; j<sourceImage.rows; j++)
			{
				Vec3b color1 = sourceImage.at<Vec3b>(Point(i, j));
				Scalar color2 = destinationImage.at<uchar>(Point(i, j));
				color2 = (0.11*color1.val[0] + 0.59*color1.val[1] + 0.3*color1.val[2]);

				destinationImage.at<uchar>(Point(i, j)) = color2.val[0];
			}
		}
		/*int nChannels = sourceImage.channels();
		int pRows = sourceImage.rows, pCols = sourceImage.cols;
		for (int y = 0; y < pRows; y++) {
		const uchar*pRow = sourceImage.ptr<uchar>(y);
		for (int x = 0; x < pCols; x++, pRows += nChannels) {
		Scalar color2 = destinationImage.at<uchar>(Point(x, y));
		color2 = (0.11*pRow[0] + 0.59*pRow[1] + 0.3*pRow[2]);
		destinationImage.at<uchar>(Point(x, y)) = color2.val[0];
		}
		}*/
	}
}

int Converter::GrayScale2RGB(const Mat& sourceImage, Mat& destinationImage) {
	if (sourceImage.empty()) return 1;
	destinationImage = sourceImage.clone();
	if (destinationImage.empty())return 1;
	int nCols = destinationImage.cols;
	int nRows = destinationImage.rows;
	uchar* pDataSource = (uchar*)sourceImage.data;
	uchar* pDataDes = (uchar*)destinationImage.data;
	int widthStep = sourceImage.step[0];
	int nChanel = sourceImage.step[1];
	int widthStepDes = destinationImage.step[0];
	int nChanelDes = destinationImage.step[1];
	for (int i = 0; i < nRows; i++) {
		uchar* pRow = pDataSource;
		uchar* pRowDes = pDataDes;
		for (int j = 0; j < nCols; j++) {
			for (int z = 0; z < nChanel; z++)
			{
				//Gán giá tr? c?a 3 kênh màu R,G,B b?ng Gray nh?m chuy?n t? m?ng 3 chi?u v? m?ng 1 chi?u
				destinationImage.at<Vec3b>(i, j)[0] = sourceImage.at<Vec3b>(i, j)[z]; // Blue = gray
				destinationImage.at<Vec3b>(i, j)[1] = sourceImage.at<Vec3b>(i, j)[z]; // Green = gray
				destinationImage.at<Vec3b>(i, j)[2] = sourceImage.at<Vec3b>(i, j)[z]; // Red = gray
			}
			pRow += nChanel;
			pRowDes += nChanelDes;
		}
		pDataSource += widthStep;
		pDataDes += widthStepDes;
	}
	return 0;

}


int Converter::HSV2RGB(const Mat& sourceImage, Mat& destinationImage) {
	destinationImage = sourceImage.clone();
	for (int y = 0; y < sourceImage.rows; y++) {
		for (int x = 0; x < sourceImage.cols; x++) {
			hsv2rgb(sourceImage.at<Vec3b>(y, x)[0], sourceImage.at<Vec3b>(y, x)[1], sourceImage.at<Vec3b>(y, x)[2],
				destinationImage.at<Vec3b>(y, x)[0], destinationImage.at<Vec3b>(y, x)[1], destinationImage.at<Vec3b>(y, x)[2]);
			/*for (int c = 0; c < sourceImage.channels(); c++) {
			destinationImage.at<Vec3b>(y, x)[c] =
			saturate_cast<uchar>(1.0*sourceImage.at<Vec3b>(y, x)[c]);*/
		}
	}
	return 1;
}

int Converter::RGB2HSV(const Mat& sourceImage, Mat& destinationImage) {
	destinationImage = sourceImage.clone();
	for (int y = 0; y < sourceImage.rows; y++) {
		for (int x = 0; x < sourceImage.cols; x++) {
			rgb2hsv(sourceImage.at<Vec3b>(y, x)[0], sourceImage.at<Vec3b>(y, x)[1], sourceImage.at<Vec3b>(y, x)[2],
				destinationImage.at<Vec3b>(y, x)[0], destinationImage.at<Vec3b>(y, x)[1], destinationImage.at<Vec3b>(y, x)[2]);
			/*for (int c = 0; c < sourceImage.channels(); c++) {
			destinationImage.at<Vec3b>(y, x)[c] =
			saturate_cast<uchar>(1.0*sourceImage.at<Vec3b>(y, x)[c]);*/
		}
	}
	return 1;
}

int Converter::Convert(Mat& sourceImage, Mat& destinationImage, int type) {
	if (type == 1) {
		RGB2HSV(sourceImage, destinationImage);
	}
	if (type == 2) {
		HSV2RGB(sourceImage, destinationImage);
	}
}
