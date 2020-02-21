#include "stdafx.h"
#include "ColorTransformer.h"
#include <iostream>
using namespace std;
ColorTransformer::ColorTransformer()
{}
ColorTransformer::~ColorTransformer()
{}
int ColorTransformer::ChangeBrighness(const Mat& sourceImage, Mat& destinationImage, short b) {

	if (sourceImage.empty()) // Check for failure
	{
		return 0;
	}
	destinationImage = sourceImage;
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
	destinationImage = sourceImage;
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
double ColorTransformer::MaxPixel(vector<double> histMatrix)
{
	double max = 0;
	for (int i = 0; i < histMatrix.size(); i++) {
		if (max < histMatrix[i]) {
			max = histMatrix[i];
		}
	}
	return max;
}
vector<double> ColorTransformer::CalculatorHistogram(const Mat& sourceImage, vector<double> histMatrix)
{
	Mat image = sourceImage;
	int nchannel = image.channels();
	if (nchannel > 1)//Anh mau
	{
		//Tach tung kenh mau, tinh 3 histopgram cho tung mau
		int row = image.rows; // number of lines  
		int col = image.cols;
		//Khỏi tạo mang H toàn giá trị 0
		double B[256],G[256],R[256];
		for (int i = 0; i < 256; i++) {
			B[i] = 0;
			G[i] = 0;
			R[i] = 0;
		}
		//histMatrix = Mat(1, 256, CV_, Scalar(0));
		//Tính lược đồ xám xám của ảnh f luu vào tung kenh
		for (int j = 0; j < row; j++) {
			// lấy địa chỉ của dòng thứ j   
			uchar* data = image.ptr<uchar>(j);
			for (int i = 0; i < col; i++,data+=nchannel) {
				// xử lý trên mỗi pixel    
				B[data[0]] += 1;
				G[data[1]] += 1;
				R[data[2]] += 1;
			} 
		}   
		
		for (int i = 0; i < 256; i++) {
			// xử lý trên mỗi pixel    
			histMatrix.push_back(B[i]);
			histMatrix.push_back(G[i]);
			histMatrix.push_back(R[i]);
		} // end of line   }
		return histMatrix;
		
	}
	if (nchannel == 1)
	{
		int row = image.rows; // number of lines  
		int col = image.cols;
		//Khỏi tạo mang H toàn giá trị 0
		double H[256];
		for (int i = 0; i < 256; i++)
			H[i] = 0;
		//Tính lược đồ xám xám của ảnh f luu vào H
		for (int j = 0; j < row; j++) {
			// lấy địa chỉ của dòng thứ j   
			uchar* data = image.ptr<uchar>(j);
			for (int i = 0; i < col; i++) {
				// xử lý trên mỗi pixel    
				H[data[i]] += 1;
			} // end of line   }
		}
		for (int i = 0; i < 256; i++) {
			histMatrix.push_back(H[i]);
		} 
		return histMatrix;
	}
}
//mỗi pixel có giá trị 0-255, trong ảnh xám có nhiều pixel có cùng mức xám. 
int ColorTransformer::HistogramEqualization(const Mat& sourceImage, Mat& destinationImage)
{
	// Convert to grayscale
	Mat image = sourceImage;
	//cvtColor(image, image, CV_BGR2GRAY);
	int nchannel = image.channels();
	if (nchannel > 1)//Anh mau
	{
		int row = image.rows; // number of lines  
		int col = image.cols;
		destinationImage = Mat(row, col, CV_8UC3, Scalar(0, 0, 0));
		//Tinh luoc do mau
		double B[256], G[256], R[256];
		vector<double> histMatrix;
		histMatrix=ColorTransformer::CalculatorHistogram(sourceImage,histMatrix);
		
		for (int i = 0,j = 0; i < histMatrix.size() - 2; i+=3,j++) {
			B[j] = histMatrix[i];
			G[j] = histMatrix[i + 1];
			R[j] = histMatrix[i + 2];
		}
		//Tinh luoc do xam tich luy cua f luu vao T
		double TB[256], TG[256], TR[256];
		double TBR[256], TGR[256], TRR[256];
		TB[0] = B[0];TG[0] = G[0];TR[0] = R[0];
		double val = 255.0 / (col*row);
		for (int j = 1; j < 256; j++)
		{
			TB[j] = TB[j - 1] + B[j];
			TG[j] = TG[j - 1] + G[j];
			TR[j] = TR[j - 1] + R[j];
			//Chuan Hoa t ve doan [0,255]
			
			TBR[j] = round(val*TB[j]);
			TGR[j] = round(val*TG[j]);
			TRR[j] = round(val*TR[j]);
		}
		TBR[0] = round(val*TB[0]);
		TGR[0] = round(val*TG[0]);
		TRR[0] = round(val*TR[0]);
		//Tao anh ket qua
		

		for (int y = 0; y < row; y++)
		{
			uchar* data = image.ptr<uchar>(y);
			uchar* data01 = destinationImage.ptr<uchar>(y);
			for (int x = 0; x < col; x++,data+=3,data01+=3)
			{
				data01[0] = TBR[data[0]];
				data01[1] = TGR[data[1]];
				data01[2] = TRR[data[2]];
			}
		}
		return 0;

	}
	else
	{
		int row = image.rows; // number of lines  
		int col = image.cols;
		destinationImage = Mat(row, col, CV_8UC1);
		double H[256];
		vector<double> histMatrix;
		histMatrix=ColorTransformer::CalculatorHistogram(sourceImage, histMatrix);
		
		for (int i = 0; i < histMatrix.size(); i++) {
			H[i] = histMatrix[i];
		}
		//Tinh luoc do xam tich luy cua f luu vao T
		double T[256];
		double TR[256];
		T[0] = H[0];
		double val = 255.0 / (row*col);
		for (int j = 1; j < 256; j++)
		{
			T[j] = T[j - 1] + H[j];
			//Chuan Hoa t ve doan [0,255]
			
			TR[j] = round(val*T[j]);
		}
		TR[0] = round(val*T[0]);
		//Tao anh ket qua
		for (int y = 0; y < row; y++)
		{
			uchar* data = image.ptr<uchar>(y);
			uchar* data01 = destinationImage.ptr<uchar>(y);
			for (int x = 0; x < col; x++)
			{
				data01[x] = TR[data[x]];
			}
		}
		return 0;
	}
	return 1;
}

int ColorTransformer::DrawHistogram(vector<double> histMatrix, Mat& histImage)
{
	//size 256: tuong duong la 1 kenh mau
	int channel = histMatrix.size();
	if (channel > 256) {
		//anh mau
		histImage = Mat(350, 280 * 3, CV_8UC1, Scalar(255));
		double MAX = ColorTransformer::MaxPixel(histMatrix);
		MAX += 10;
		int i;
		int j;
		for (i = 0,j = 0; i < histMatrix.size() - 2; i += 3,j++) {
			
			double Point_YB = ceil(histMatrix[i] * 350.0 / MAX);
			double Point_YG = ceil(histMatrix[i + 1] * 350.0 / MAX);
			double Point_YR = ceil(histMatrix[i + 2] * 350.0 / MAX);
			for (int y = histImage.rows - 1; y > histImage.rows - Point_YB; y--)
			{
				uchar* data = histImage.ptr<uchar>(y);
				data[j] = 1;
			}
			for (int y = histImage.rows-1; y > histImage.rows - Point_YG; y--)
			{
				uchar* data = histImage.ptr<uchar>(y);
				data[j + 270] = 1;
			}
			for (int y = histImage.rows - 1; y > histImage.rows - Point_YR; y--)
			{
				uchar* data = histImage.ptr<uchar>(y);
				data[j + 270*2] = 1;
			}
			
		}
		return 1;
	}
	if (channel == 256) {//anh xam
		histImage = Mat(350, 280, CV_8UC1, Scalar(255));
		int j;
		for (j = 0; j < histMatrix.size(); j++) {

			double Point_YR = ceil(histMatrix[j] * 350.0 / 2000);
			for (int y = histImage.rows - 1; y > histImage.rows - Point_YR; y--)
			{
				uchar* data = histImage.ptr<uchar>(y);
				data[j] = 1;
			}
		}
		return 1;
	}
	return 0;
}
float ColorTransformer::CompareImage(const Mat& image1, Mat& image2)
{
	//Tinh khoang cach 2 mang gia trij histogram
	//1.Tinh histogram tung anh
	Mat image01 = image1;
	Mat image02 = image2;
	vector<double> histMatrix1 = ColorTransformer::CalculatorHistogram(image01, histMatrix1);
	vector<double> histMatrix2 = ColorTransformer::CalculatorHistogram(image02, histMatrix2);

	int channel1 = image01.channels();
	int channel2 = image02.channels();
	if (channel1 != channel2)
		return 0.0;
	if (channel1 == 3) {//anh xam. -->2 histogram la 1 mong chieu 256 phan tu
		//2. Su dung ong thuc tinh khoanf cach 2 VECTOR
		double B[256], G[256], R[256];
		double B1[256], G1[256], R1[256];
		for (int i = 0, j = 0; i < histMatrix1.size() - 2; i += 3, j++) {
			B[j] = histMatrix1[i];
			G[j] = histMatrix1[i + 1];
			R[j] = histMatrix1[i + 2];
			B1[j] = histMatrix2[i];
			G1[j] = histMatrix2[i + 1];
			R1[j] = histMatrix2[i + 2];
		}
		int count = 0;
		for (int i = 0; i < 256; i++)
		{
			if (B[i] == B1[i] && G[i] == G1[i] && R[i] == R1[i])
				count++;
		}
		return count * 100.0 / 256.0;
	}
	if (channel1 == 1)
	{
		int count = 0;
		for (int i = 0; i < 256; i++)
		{
			if (histMatrix1[i]==histMatrix2[i])
				count++;
		}
		return count * 100.0 / 256.0;
	}
}