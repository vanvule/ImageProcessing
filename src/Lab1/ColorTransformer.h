#pragma once
#include "stdafx.h"
#include <iostream>
#include <vector>
#include "opencv2/opencv.hpp" 
#include "opencv2/highgui/highgui.hpp" 
#include <opencv2\core\core.hpp>
using namespace cv;
using namespace std;
class ColorTransformer
{
public:
	ColorTransformer();
	~ColorTransformer();
	
public:
	/*
	Hàm nhận vào một ảnh, thay đổi độ sáng của ảnh này và lưu kết quả vào ảnh mới
	Tham so:
	sourceImage	: ảnh ban đầu
	destinationImage: ảnh kết quả
	b	: giá trị số nguyên dùng để thay đổi độ sáng của ảnh
	Hàm trả về:
	1: Nếu thành công thì trả về ảnh kết quả (ảnh gốc vẫn giữ nguyên giá trị)
	0: Nếu không tạo được ảnh kết quả hoặc ảnh input không tồn tại
	*/
	int ChangeBrighness(const Mat& sourceImage, Mat& destinationImage, short b);

	/*
	Hàm nhận vào một ảnh, thay đổi độ tương phản của ảnh này và lưu kết quả vào ảnh mới
	Tham so :
	sourceImage : ảnh ban đầu
	destinationImage : ảnh kết quả
	c	: giá trị số thực dùng để thay đổi độ tương phản của ảnh
	Hàm trả về:
	1: Nếu thành công thì trả về ảnh kết quả (ảnh gốc vẫn giữ nguyên giá trị)
	0: Nếu không tạo được ảnh kết quả hoặc ảnh input không tồn tại
	*/
	int ChangeContrast(const Mat& sourceImage, Mat& destinationImage, float c);


	/*Hàm tính histogram
	Tham số là ảnh
	Giá trị trả về: Mảng histogram ảnh xám
	Tính số giá trị cùng mức xám của các pixel
	*/
	vector<double> CalculatorHistogram(const Mat& sourceImage,vector<double>histMatrix);


	/*
	Hàm cân bằng lược đồ màu tổng quát cho ảnh bất kỳ
	Tham so :
	sourceImage : ảnh ban đầu
	destinationImage : ảnh kết quả
	Hàm trả về:
	1: Nếu thành công thì trả về ảnh kết quả (ảnh gốc vẫn giữ nguyên giá trị)
	0: Nếu không tạo được ảnh kết quả hoặc ảnh input không tồn tại
	*/
	int HistogramEqualization(const Mat& sourceImage, Mat& destinationImage);

	
	

	/*
	Hàm so sánh hai ảnh
	Tham so :
	image1 : ảnh thứ nhất
	image2 : ảnh thứ hai
	Hàm trả về:
	độ đo sự tương đồng giữa hai ảnh
	*/
	float CompareImage(const Mat& image1, Mat& image2);
	//double Sum(double H[], int n, int x);
	double MaxPixel(vector<double> histMatrix);
	/*Vẽ histogram anh
	Hàm cân bằng lược đồ màu tổng quát cho ảnh bất kỳ
	Tham so :
	histMatrix : vector histogram ảnh ban đầu
	histImage : ảnh histogrma
	Hàm trả về:
	1: Nếu thành công 
	0: Nếu không tạo được ảnh kết quả hoặc ảnh input không tồn tại
	*/
	int DrawHistogram(vector<double>histMatrix, Mat& histImage);
	
};

#pragma once
