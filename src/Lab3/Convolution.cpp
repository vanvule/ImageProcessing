#include "stdafx.h"
#include "Convolution.h"

Convolution::Convolution()
{
	this->_kernel;
	this->_kernelHeight = 0;
	this->_kernelWidth = 0;
}
Convolution::~Convolution()
{
	this->_kernel.clear();
	this->_kernelHeight = 0;
	this->_kernelWidth = 0;
}
vector<Kernel> Convolution::GetKernel()
{
	return this->_kernel;
}
void Convolution::SetKernel(vector<float> kernel, int kWidth, int kHeight)
{//
	
	int indexX[9] = { -1,-1,-1,0,0,0,1,1,1 };
	int indexY[9] = { -1,0,1,-1,0,1,-1,0,1 };
	for (int i = 0; i < kernel.size(); i++) {
		Kernel K;
		K.x = indexX[i];
		K.y = indexY[i];
		K.data = kernel[i];
		this->_kernel.push_back(K);
	}
	this->_kernelHeight = kHeight;
	this->_kernelWidth = kWidth;
	
}

int Convolution::DoConvolution(const Mat& sourceImage, Mat& destinationImage)
{
	//link: https://www.stdio.vn/articles/phep-tich-chap-trong-xu-ly-anh-convolution-386
	Mat Image = sourceImage;
	if (Image.channels() == 1) {
		int row = Image.rows;
		int col = Image.cols;
		// Tạo matrix để lưu giá trị pixel sau khi thực hiện tích chập.
		destinationImage.create(Size(col, row), CV_8UC1);
		for (int i = 0; i < row; i++)
		{
			// Lấy địa chỉ dòng của ảnh đích, để lưu kết quả vào.
			uchar* data = destinationImage.ptr<uchar>(i);
			for (int j = 0; j < col; j++)
			{
				// Lưu tổng giá trị độ xám của vùng ảnh tương ứng với kernel
				int g_val = 0;
				// Duyệt mask, giá trị pixel đích là tổ hợp tuyến tính của mask với ảnh nguồn.
				for (int ii = 0; ii < _kernel.size(); ii++)
				{
					//_kernelIndex: mảng chỉ số truy cập nhanh
					int index_r = i - _kernel[ii].x;

					// Với pixel nằm ngoài biên, bỏ qua.
					if (index_r < 0 || index_r > row - 1)
						continue;

					int index_c = j - _kernel[ii].y;
					if (index_c < 0 || index_c > col - 1)
						continue;

					g_val += _kernel[ii].data * sourceImage.at<uchar>(index_r, index_c);
				}

				// Gán giá trị cho matrix đích.
				data[j] = g_val;

			}
		}
		return 1;
	}
}