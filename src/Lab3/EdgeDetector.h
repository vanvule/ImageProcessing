#pragma once
#include "stdafx.h"
#include <iostream>
#include <vector>
#include "opencv2/opencv.hpp" 
#include "opencv2/highgui/highgui.hpp" 
#include <opencv2\core\core.hpp>
using namespace cv;
using namespace std;
class EdgeDetector
{
public:
	/*
	Tính thành phần x của vectơ gradient
	tại một điểm đã cho trong ảnh.
	trả về độ dốc theo hướng x
	*/
	friend int xGradient(Mat image, int x, int y, int method);

	/*
	Tính thành phần y của vectơ gradient
	tại một điểm đã cho trong ảnh
	trả về độ dốc theo hướng y
	*/
	friend int yGradient(Mat image, int x, int y, int method);

	friend int Laplacian(const unsigned char* src_, unsigned char* dst_, int width_, int height_);

	/*
	Hàm phát hiện biên cạnh của ảnh xám
	sourceImage: ảnh input
	destinationImage: ảnh output
	method: phương pháp detect
	kWidth, kHeight: kích thước kernel
	1: Sobel
	2: Prewitt
	3: Laplace
	Hàm trả về
	0: nếu detect thành công
	1: nếu detect thất bại (không đọc được ảnh input,...)
	*/
	int DetectEdge(Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight, int method);
	EdgeDetector() {}
	~EdgeDetector() {}
};

