#include "stdafx.h"
#include "Blur.h"
#include "opencv2/opencv.hpp" 
#include "opencv2/highgui/highgui.hpp" 
#include <opencv2\core\core.hpp>
void Swap(float &a, float &b)
{
	float tempt;
	tempt = a;
	a = b;
	b = tempt;
}
vector<float> Selection_Sort(vector<float> data)
{
	int n = data.size();
	for (int i = 0; i < n - 1; i++)
	{
		int min = i;
		for (int j = i + 1; j < n; j++)
		{
			if (data[j] < data[min])
				min = j;
		}
		Swap(data[i], data[min]);
	}
	return data;
}
int  Blur::BlurImage(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight, int method)
{
	Convolution C;
	Mat image = sourceImage;
	vector<float> kernel;
	if (method == 0)//loc trung binh
	{
		
		kernel.push_back(1.0 / 9);
		kernel.push_back(1.0 / 9);
		kernel.push_back(1.0 / 9);
		kernel.push_back(1.0 / 9);
		kernel.push_back(1.0 / 9);
		kernel.push_back(1.0 / 9);
		kernel.push_back(1.0 / 9);
		kernel.push_back(1.0 / 9);
		kernel.push_back(1.0 / 9);
		C.SetKernel(kernel, 3, 3);
		C.DoConvolution(image, destinationImage);
		return 0;
	}
	if (method == 2)//Gauss
	{
		//link: https://www.stdio.vn/articles/loc-gaussian-giao-dien-don-gian-qt-opencv-447
		float sigma = 0;
		float pi = 3.14;
		cout << "Input sigma: ";
		cin >> sigma;
		//B1: Set kernel Gaussian
		float Hesonhan = 1 / (sqrt(2 * pi) * sigma);
		float HesonhanMu= 1 / (2 * sigma * sigma);

		float sumker = 0;
		for (int i = -kHeight / 2; i < ((kHeight - 1) / 2) + 1; i++)
		{
			for (int j = -kWidth / 2; j < ((kWidth - 1) / 2) + 1; j++)
			{
				float hi = Hesonhan*exp(-(i*i + j*j)*HesonhanMu);
				kernel.push_back(hi);
				sumker += hi;
			}
		}
		//sumker;
		if (sumker == 0)
			sumker = 1;
		//B2: Chuan hoa: Chia m?i phan tu trong kernel cho sumker
		for (int i = 0; i < kernel.size(); i++) {
			kernel[i] = (kernel[i] / sumker);
		}
		C.SetKernel(kernel, kHeight, kWidth);
		//B3: Tich chap kernel voi anh ban nguon
		C.DoConvolution(image, destinationImage);
		return 0;
	}
	if (method == 1)//Trung vi
	{
		//destinationImage.create(Size(image.cols, image.rows), CV_8UC1);
		int col = image.cols;
		int row = image.rows;
		destinationImage = Mat( row,col, CV_8UC1);
		vector<float>Matrix;
		//vi tri bien thi bo qua khong loc
		
		for (int i = 1; i < row - 1; i++) {
			
			for (int j = 1; j < col - 1; j++){
				
				Matrix.push_back(image.at<uchar>(i,j));//0,0
				Matrix.push_back(image.at<uchar>(i, j+1));//0,1
				Matrix.push_back(image.at<uchar>(i, j-1));//0,-1
				Matrix.push_back(image.at<uchar>(i-1, j));//-1,0
				Matrix.push_back(image.at<uchar>(i+1, j));//1,0
				Matrix.push_back(image.at<uchar>(i-1, j-1));//-1,-1
				Matrix.push_back(image.at<uchar>(i+1, j+1));//1,1
				Matrix.push_back(image.at<uchar>(i-1, j+1));//-1,1
				Matrix.push_back(image.at<uchar>(i+1, j-1));//1,-1
				Matrix=Selection_Sort(Matrix);
				destinationImage.at<uchar>(i,j)= Matrix[4];
				Matrix.clear();
			}
		}
		return 0;
	}
	return 1;
}