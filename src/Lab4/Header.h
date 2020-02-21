#pragma once

#pragma once
#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

class CannyEdgeDetector
{
	//ngưỡng dưới
	int _lowThreshold;
	//ngưỡng trên
	int _highThreshold;

public:
	CannyEdgeDetector();
	~CannyEdgeDetector();

	int getLow();
	int getHigh();
	void setLow(int low);
	void setHigh(int high);

	/*
		Hàm áp dụng thuật toán Canny để phát hiện biên cạnh
		- srcImage: ảnh input
		- dstImage: ảnh kết quả
		Hàm trả về
			1: nếu detect thành công
			0: nếu detect không thành công
	*/
	//int Apply(const Mat& srcImage, Mat& dstImage);


//Gaussian kernel
	int Gaussian_kernel(int kernel_size, int sigma, Mat& kernel);

	/*
Tính giá trị độ dốc và hướng
imageSource: hình ảnh thang độ thô
hình ảnh độ dốc hướng X của imageX
hình ảnh gradient hướng hình ảnh Y
gradXY độ lớn của độ dốc tại điểm này
góc hướng theta độ dốc theta = arctan (imageY / imageX)
*/
	int GradDirection(const Mat imageSource, Mat& imageX, Mat& imageY, Mat& gradXY, Mat& theta);

	//Dọc theo hướng của độ dốc của điểm này, so sánh biên độ của hai điểm trước và sau điểm.Nếu điểm lớn hơn hai điểm trước và sau, nó được giữ lại.
	//Nếu điểm nhỏ hơn bất kỳ điểm nào trong hai điểm trước và sau, điểm đó được đặt thành 0;
	//đầu vào imageInput để có được hình ảnh gradient
	//đầu ra hình ảnh không bị triệt tiêu tối đa bởi imageOutput
	//góc hướng theta của từng pixel
	//độ dốc hướng X của imageX
	//độ dốc hình ảnh hướng Y
	int NonLocalMaxValue(const Mat imageInput, Mat& imageOutput, const Mat& theta, const Mat& imageX, const Mat& imageY);


/*Cơ chế của ngưỡng kép là:
Chỉ định giá trị ngưỡng low A và giá trị ngưỡng high B và chọn 70% tổng biểu đồ là B và B gấp 1,5 đến 2 lần kích thước của A;
Nếu giá trị màu xám nhỏ hơn A, đặt giá trị này thành 0; nếu giá trị màu xám lớn hơn B, hãy đặt giá trị này thành 255;*/

	int DoubleThreshold(Mat& imageInput);

	/*
Nếu giá trị màu xám nằm giữa A và B, kiểm tra xem 8 pixel liền kề với pixel có giá trị xám là 255 hay không.
Nếu không có 255, nó chỉ ra rằng đây là điểm tối đa cục bộ bị cô lập, được loại trừ và được đặt thành 0;
Nếu có 255, điều đó có nghĩa đây là vật liệu có thể xây dựng có "đường viền" với các cạnh khác. Đặt nó thành 255.
Bước này được lặp lại cho đến khi pixel cuối cùng được kiểm tra.

==> Thuật toán theo dõi vùng lân cận bắt đầu từ các pixel có giá trị 255 để tìm các điểm xung quanh đáp ứng các yêu cầu
và đặt các điểm đáp ứng các yêu cầu thành 255.
=> Sau đó sửa đổi các giá trị tọa độ của i, j và các giá trị i, j được khôi phục.
Trên cơ sở thay đổi i, j, tiếp tục tìm các điểm xung quanh 255 đáp ứng yêu cầu.
=>Sau khi tất cả các điểm kết nối 255 được sửa đổi, hãy đặt tất cả các điểm tối đa cục bộ được đề cập ở trên thành 0;
(thuật toán có thể tiếp tục được tối ưu hóa).

Tham số 1, imageInput: hình ảnh độ dốc của đầu vào và đầu ra
Tham số 2, lowTh: ngưỡng thấp
Tham số 3, highTh: ngưỡng cao
	*/
	int Apply(Mat& imageInput, Mat& imageOutput);

};


