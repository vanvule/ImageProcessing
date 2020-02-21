#pragma once
#include "stdafx.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "math.h"
#include <vector>
#include "opencv2/opencv.hpp" 
#include "opencv2/highgui/highgui.hpp" 
#include <opencv2\core\core.hpp>
using namespace cv;
using namespace std;
class Kmean
{
	//số cụm K
	int _numClusters;
public:
	/*
	Hàm áp dụng thuật toán Kmeans để phân đoạn ảnh
	- srcImage: ảnh input
	- dstImage: ảnh kết quả
	Hàm trả về
	1: nếu phân đoạn thành công
	0: nếu phân đoạn không thành công
	*/

	int Apply(const Mat& srcImage, Mat &dstImage);
	Kmean();
	void SetCluster(int k) { _numClusters = k; }
	~Kmean() {}
};

