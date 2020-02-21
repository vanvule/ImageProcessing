#include "stdafx.h"
#include "Threshold.h"
#include "math.h"
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
StaticThreshold::StaticThreshold()
{
	this->_lowThreshold = 0;
	
	this->_highThreshold = 0;

}
StaticThreshold::StaticThreshold(int a, int b)
{
	this->_lowThreshold = a;

	this->_highThreshold = b;

}
int StaticThreshold::Apply(const Mat& srcImage, Mat &dstImage)
{
	//StaticThreshold::StaticThreshold();
	Mat image = srcImage;
	dstImage = image;
	if (image.channels() == 1)
	{
		for (int i = 0; i < image.rows; i++)
		{
			uchar *data = image.ptr<uchar>(i);
			uchar *datadst = image.ptr<uchar>(i);
			for (int j = 0; j < image.cols; j++)
			{
				if (data[j] < _highThreshold && data[j] > _lowThreshold)
				{
					datadst[j] = 255;
				}
				else
					datadst[j] = 0;
			}
		}
		return 0;
	}
	return 1;
}
AverageLocalThreshold::AverageLocalThreshold()
{
	this->_C = 0;
}
AverageLocalThreshold::AverageLocalThreshold(int c)
{
	this->_C = c;
}
vector<int> GetPixelAround(Mat& srcImage,Size winSize)
{
	/*
	Ham lay cac gia tri pixel lan can
	*/
	Mat image = srcImage;
	int kHalfWidth = winSize.width >> 1; 
	int kHalfHeight = winSize.height >> 1; 
	int widthStep = image.step[0];
	vector<int> offsets; 
	for (int y = -kHalfHeight; y <= kHalfHeight; y++) 
		for (int x = -kHalfWidth; x <= kHalfWidth; x++) 
			offsets.push_back(y*widthStep + x);
	return offsets;
}
int AverageLocalThreshold::Apply(const Mat& srcImage, Mat &dstImage, Size winSize)
{
	Mat image = srcImage;
	
	//padding anh nguon, khong bo qua  cac vien anh -->  giu nguyen kich thuuoc anh
	Mat image_src;
	if (winSize.height == 3 && winSize.width == 3)
	{
		image_src = Mat(image.rows + 2, image.cols + 2, CV_8UC1);
		int row = image_src.rows;
		int col = image_src.cols;
		uchar *data_src0 = image_src.ptr<uchar>(0);
		for (int j = 0; j < col; j++)
			data_src0[j] = 0;
		data_src0 = image_src.ptr<uchar>(row-1);
		for (int j = 0; j < col; j++)
			data_src0[j] = 0;
		for (int i = 1; i < row - 1; i++)
		{
			uchar *data = image.ptr<uchar>(i - 1);
			uchar *data_src = image_src.ptr<uchar>(i);
			data_src[0] = 0;
			data_src[col - 1] = 0;
			for (int j = 1; j < col - 1; j++)
			{
				data_src[j] = data[j - 1];
			}
		}
		
	}
	//Thuc hien phan nguong anh sau khi padding dam bao giu nguyen kich thuoc annh
	if (image_src.channels() == 1)
	{
		dstImage = image_src;
		vector <int> Matrix;
		Matrix = GetPixelAround(image_src, winSize);
		for (int i = 1; i < image_src.rows - 1; i++)
		{
			
			uchar *data_dst = dstImage.ptr<uchar>(i);
			for (int j = 1; j < image_src.cols - 1; j++)
			{
				
				
				uchar *p = image_src.ptr<uchar>(i,j);
				float x = p[0];
				float mean;	
				float sum = 0;
				for (int k = 0; k < Matrix.size(); k++)
				{
					sum+=p[Matrix[k]];//Tong cac pixel lan can
				}
				mean = sum / Matrix.size();//Tinh trung binh cac pixel
				if (x > mean - _C)
					data_dst[j] = 255;
				else
					data_dst[j] = 0;
			}
		}
		return 0;
	}
	return 1;
}


MedianLocalThreshold::MedianLocalThreshold()
{
	this->_C = 0;
}
MedianLocalThreshold::MedianLocalThreshold(int c)
{
	this->_C = c;
}

int MedianLocalThreshold::Apply(const Mat& srcImage, Mat &dstImage, Size winSize)
{
	Mat image = srcImage;

	//padding anh nguon
	Mat image_src;
	if (winSize.height == 3 && winSize.width == 3)
	{
		image_src = Mat(image.rows + 2, image.cols + 2, CV_8UC1);
		int row = image_src.rows;
		int col = image_src.cols;
		uchar *data_src0 = image_src.ptr<uchar>(0);
		for (int j = 0; j < col; j++)
			data_src0[j] = 0;
		data_src0 = image_src.ptr<uchar>(row - 1);
		for (int j = 0; j < col; j++)
			data_src0[j] = 0;
		for (int i = 1; i < row - 1; i++)
		{
			uchar *data = image.ptr<uchar>(i - 1);
			uchar *data_src = image_src.ptr<uchar>(i);
			data_src[0] = 0;
			data_src[col - 1] = 0;
			for (int j = 1; j < col - 1; j++)
			{
				data_src[j] = data[j - 1];
			}
		}

	}
	if (image_src.channels() == 1)
	{
		dstImage = image_src;
		vector <int> Matrix;
		Matrix = GetPixelAround(image_src, winSize);
		for (int i = 1; i < image_src.rows - 1; i++)
		{
			uchar *data_dst = dstImage.ptr<uchar>(i);
			for (int j = 1; j < image_src.cols - 1; j++)
			{


				uchar *p = image_src.ptr<uchar>(i, j);
				float median;
				vector<float> GetPixel;
				for (int k = 0; k < Matrix.size(); k++)
				{
					GetPixel.push_back(p[Matrix[k]]);
				}
				//Sap xep cac pixel lan can

				GetPixel = Selection_Sort(GetPixel);
				float index_median = ceilf(GetPixel.size() / 2.0);
				median = GetPixel[index_median];
				if (p[0] > median - _C)
					data_dst[j] = 255;
				else
					data_dst[j] = 0;
			}
		}
		return 0;
	}
	return 1;
}
SauvolaLocalThreshold::SauvolaLocalThreshold()
{
	this->_k = 0.5;

	this->_r = 128;
}
SauvolaLocalThreshold::SauvolaLocalThreshold(int k, int r)
{
	this->_k = k;
	this->_r = r;
}

int SauvolaLocalThreshold::Apply(const Mat& srcImage, Mat &dstImage, Size winSize)
{
	//Độ lệch chuẩn cho biết độ phân tán của các giá trị trong bộ số liệu
	//Phương sai là một giá trị đại diện cho độ phân tán của các số liệu so với giá trị trung bình
	/*
	1. Tinh mean cac gia tri torng bo du lieu
	2. Tinh phuong sai: Lấy giá trị trung bình tính được ở bước trên trừ đi từng giá trị trong bộ số liệu
	   Bình phương tất cả các giá trị có được ở các phép trừ trên
	   Tong binh phuong: Cộng tổng các giá trị sau khi đã bình phương ở trên, giá trị này còn được gọi là tổng bình phương
	   Chia tổng bình phương cho (n-1) với n là kích cỡ của bộ số liệu.
	3. Tinh do lech chuan:Lấy căn bậc hai của phương sai, ta sẽ được giá trị của độ lệch chuẩn
	*/
	Mat image = srcImage;
	//link do lech chuan  day so: https://www.wikihow.vn/T%C3%ADnh-%C4%90%E1%BB%99-l%E1%BB%87ch-Chu%E1%BA%A9n
	//padding anh nguon
	Mat image_src = srcImage;
	/*if (winSize.height == 3 && winSize.width == 3)
	{
		image_src = Mat(image.rows + 2, image.cols + 2, CV_8UC1);
		int row = image_src.rows;
		int col = image_src.cols;
		uchar *data_src0 = image_src.ptr<uchar>(0);
		for (int j = 0; j < col; j++)
			data_src0[j] = 0;
		data_src0 = image_src.ptr<uchar>(row - 1);
		for (int j = 0; j < col; j++)
			data_src0[j] = 0;
		for (int i = 1; i < row - 1; i++)
		{
			uchar *data = image.ptr<uchar>(i - 1);
			uchar *data_src = image_src.ptr<uchar>(i);
			data_src[0] = 0;
			data_src[col - 1] = 0;
			for (int j = 1; j < col - 1; j++)
			{
				data_src[j] = data[j - 1];
			}
		}

	}*/
	if (image_src.channels() == 1)
	{
		dstImage = image_src;
		vector <int> Matrix;
		Matrix = GetPixelAround(image_src, winSize);
		for (int i = 1; i < image_src.rows - 1; i++)
		{

			uchar *data_dst = dstImage.ptr<uchar>(i);
			for (int j = 1; j < image_src.cols - 1; j++)
			{

				//Tim do lech chuan cho bo du lieu la cac pixel lan can va chinh diem dang xet
				uchar *p = image_src.ptr<uchar>(i, j);
				float mean;
				float sum = 0;
				for (int k = 0; k < Matrix.size(); k++)
				{
					sum += p[Matrix[k]];
				}
				mean = sum / Matrix.size();
				vector<float> Sigma;
				for (int k = 0; k < Matrix.size(); k++)
				{
					Sigma.push_back((Matrix[k] - mean)*(Matrix[k] - mean));
				}
				float sum_sigma = 0;
				for (int k = 0; k < Sigma.size(); k++)
				{
					sum_sigma += Sigma[k];
				}
				uchar *q = image_src.ptr<uchar>(i, j);
				float standard_deviation = sqrt(sum_sigma / (Sigma.size() - 1));
				if (q[0] > (mean * (1 + _k * (standard_deviation / (_r - 1)))))
					data_dst[j] = 255;
				else
					data_dst[j] = 0;
			}
		}
		return 0;
	}
	return 1;
}