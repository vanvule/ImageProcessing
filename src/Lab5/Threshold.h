#pragma once
//phân ngưỡng tĩnh
#include "stdafx.h"
#include <iostream>
#include "math.h"
#include <vector>
#include "opencv2/opencv.hpp" 
#include "opencv2/highgui/highgui.hpp" 
#include <opencv2\core\core.hpp>

using namespace cv;
using namespace std;
vector<int> GetPixelAround(Mat& srcImage, Size winSize);
vector<float> Selection_Sort(vector<float> data);
void Swap(float &a, float &b);
class StaticThreshold
{
	//ngưỡng dưới
	int _lowThreshold;
	//ngưỡng trên
	int _highThreshold;
public:
	/*
	Hàm áp dụng phân ngưỡng tĩnh
	- srcImage: ảnh input
	- dstImage: ảnh kết quả
	Hàm trả về
	1: nếu phân ngưỡng thành công
	0: nếu phân ngưỡng không thành công
	*/

	int Apply(const Mat& srcImage, Mat &dstImage);
	void SetLow(int a) { _lowThreshold = a; }
	void SetHigh(int a){ _highThreshold = a; }
	StaticThreshold();
	~StaticThreshold(){}
public:
	StaticThreshold(int, int);
};


//phân ngưỡng cục bộ dựa vào trung bình
class AverageLocalThreshold
{
	//hệ số C
	int _C;

public:
	/*
	Hàm áp dụng phân ngưỡng cục bộ theo trung bình
	- srcImage: ảnh input
	- dstImage: ảnh kết quả
	- winSize: kích thước lân cận
	Hàm trả về
	1: nếu phân ngưỡng thành công
	0: nếu phân ngưỡng không thành công
	*/

	int Apply(const Mat& srcImage, Mat &dstImage, Size winSize);


	AverageLocalThreshold();
	AverageLocalThreshold(int);
	void SetC(int value) { _C = value; }
	~AverageLocalThreshold() {}
};

//phân ngưỡng cục bộ dựa vào trung vị
class MedianLocalThreshold
{
	//hệ số C
	int _C;

public:
	/*
	Hàm áp dụng phân ngưỡng cục bộ dựa vào trung vị
	- srcImage: ảnh input
	- dstImage: ảnh kết quả
	- winSize: kích thước lân cận
	Hàm trả về
	1: nếu phân ngưỡng thành công
	0: nếu phân ngưỡng không thành công
	*/

	int Apply(const Mat& srcImage, Mat &dstImage, Size winSize);


	MedianLocalThreshold();
	MedianLocalThreshold(int);
	void SetC(int value) { _C = value; }
	~MedianLocalThreshold(){}
};

//phân ngưỡng cục bộ dựa vào thuật toán Sauvola
class SauvolaLocalThreshold
{
	//hệ số r
	int _r;
	//hệ số k
	float _k;
public:
	/*
	Hàm áp dụng thuật toán Sauvola để phân ngưỡng
	- srcImage: ảnh input
	- dstImage: ảnh kết quả
	- winSize: kích thước lân cận
	Hàm trả về
	1: nếu phân ngưỡng thành công
	0: nếu phân ngưỡng không thành công
	*/

	int Apply(const Mat& srcImage, Mat &dstImage, Size winSize);


	SauvolaLocalThreshold();
	SauvolaLocalThreshold(int, int);
	void SetK(int value){_k = value;}
	void SetR(int value) { _r = value; }
	~SauvolaLocalThreshold() {}
};
