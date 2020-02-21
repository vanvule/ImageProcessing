#include"Header.h"


CannyEdgeDetector::CannyEdgeDetector() {}
CannyEdgeDetector::~CannyEdgeDetector() {}

int CannyEdgeDetector::getLow() {
	return _lowThreshold;
}

int CannyEdgeDetector::getHigh() {
	return _highThreshold;
}

void CannyEdgeDetector::setLow(int low) {
	_lowThreshold = low;
}

void CannyEdgeDetector::setHigh(int high) {
	_highThreshold = high;
}

//Guassian kernel
int CannyEdgeDetector::Gaussian_kernel(int kernel_size, int sigma, Mat& kernel)
{
	//const double PI = 3.1415926;
	const double PI = 4.0 * atan(1.0);
	int m = kernel_size / 2;

	kernel = Mat(kernel_size, kernel_size, CV_32FC1);
	float s = 2 * sigma * sigma;
	//hàm Gaussian hai chiều bên dưới là 2*pi*sigma^2, không có dấu gốc.
	for (int i = 0; i < kernel_size; i++)
	{
		for (int j = 0; j < kernel_size; j++)
		{
			int x = i - m;
			int y = j - m;

			kernel.at<float>(i, j) = exp(-(x * x + y * y) / s) / (PI * s);
		}
	}


	return 1;
}

/*
Tính giá trị gradient và hướng
imageSource: hình ảnh thang độ thô
hình ảnh gradient hướng X của imageX
hình ảnh gradient hướng Y của imageY
gradXY độ lớn của độ dốc tại điểm này
góc hướng theta độ dốc theta = arctan (imageY / imageX)
*/
int CannyEdgeDetector::GradDirection(const Mat imageSource, Mat& imageX, Mat& imageY, Mat& gradXY, Mat& theta)
{

	imageX = Mat::zeros(imageSource.size(), CV_32SC1);
	imageY = Mat::zeros(imageSource.size(), CV_32SC1);
	gradXY = Mat::zeros(imageSource.size(), CV_32SC1);
	theta = Mat::zeros(imageSource.size(), CV_32SC1);

	int rows = imageSource.rows;
	int cols = imageSource.cols;

	/*
Tham số Mat.step đề cập đến độ dài thực tế của bộ nhớ bị chiếm bởi một dòng hình ảnh, tính bằng byte.
==> Bởi vì hình ảnh trong opencv sẽ tự động điền vào độ dài của mỗi dòng (bội số của 8),
Cố gắng sử dụng con trỏ khi lập trình vì nó là cách nhanh nhất để đọc và ghi pixel và chức năng là chậm nhất.
	*/
	int stepXY = imageX.step;
	int step = imageSource.step;

	/*
Kiểu mặc định của Mat :: data là uchar*, nhưng nhiều lần cần phải xử lý các loại khác, chẳng hạn như float, int,
Ta cần truyền dữ liệu tại thời điểm này, chẳng hạn như:
Mat src (1000,1000, CV_32F);
float * myptr = (float *) src.data;
Bất kể loại Mat, Mat :: dữ liệu là uchar*
	*/
	uchar* PX = imageX.data;
	uchar* PY = imageY.data;
	uchar* P = imageSource.data;
	uchar* XY = gradXY.data;
	for (int i = 1; i < rows - 1; i++)
	{
		for (int j = 1; j < cols - 1; j++)
		{
			int a00 = P[(i - 1) * step + j - 1];
			int a01 = P[(i - 1) * step + j];
			int a02 = P[(i - 1) * step + j + 1];

			int a10 = P[i * step + j - 1];
			int a11 = P[i * step + j];
			int a12 = P[i * step + j + 1];

			int a20 = P[(i + 1) * step + j - 1];
			int a21 = P[(i + 1) * step + j];
			int a22 = P[(i + 1) * step + j + 1];

			double gradX = double(a02 + 2 * a12 + a22 - a00 - 2 * a10 - a20);
			double gradY = double(a00 + 2 * a01 + a02 - a20 - 2 * a21 - a22);

// bắt đầu từ i, j = 1 đến hàng, cols-2 và các pixel hình ảnh được khởi tạo thành 0,
// Vì vậy, vòng tròn ngoài cùng của pixel là 0 là màu đen
// PX [i * stepXY + j * (stepXY / step)] = abs (gradX);
// PY [i * stepXY + j * (stepXY / step)] = abs (gradY);
			imageX.at<int>(i, j) = abs(gradX);
			imageY.at<int>(i, j) = abs(gradY);
			if (gradX == 0)
			{
				gradX = 0.000000000001;
			}
			//Thay đổi radian thành độ (chi cho 180)
			theta.at<int>(i, j) = atan(gradY / gradX) * 57.3;
			theta.at<int>(i, j) = (theta.at<int>(i, j) + 360) % 360;
			gradXY.at<int>(i, j) = sqrt(gradX * gradX + gradY * gradY);
			//XY[i * stepXY + j * (stepXY / step)] = sqrt(gradX * gradX + gradY * gradY);
		}
	}
	/*
==> Sau khi xử lý, nó cần được chuyển đổi trở lại dạng uint8 ban đầu với hàm convertScaleAbs().
Nếu không, hình ảnh sẽ không được hiển thị mà chỉ có một cửa sổ màu xám.
Nguyên mẫu chức năng là: void convertScaleAbs (InputArray src, OutputArray dst, double alpha = 1, double beta = 0);
Tham số tùy chọn alpha là hệ số mở rộng, beta là giá trị được thêm vào kết quả và kết quả trả về một hình ảnh loại uint8
Chức năng: Thực hiện chuyển đổi hình ảnh gốc sang loại uint8
	*/
	convertScaleAbs(imageX, imageX);
	convertScaleAbs(imageY, imageY);
	convertScaleAbs(gradXY, gradXY);

	return 1;
}


//Dọc theo hướng của gradient của điểm này, so sánh biên độ của hai điểm trước và sau điểm.Nếu điểm lớn hơn hai điểm trước và sau, nó được giữ lại.
//Nếu điểm nhỏ hơn bất kỳ điểm nào trong hai điểm trước và sau, điểm đó được đặt thành 0;
//đầu vào imageInput để có được hình ảnh gradient
//đầu ra hình ảnh không bị triệt tiêu tối đa bởi imageOutput
//góc hướng theta của từng pixel
//độ dốc hướng X của imageX
//độ dốc hình ảnh hướng Y
int CannyEdgeDetector::NonLocalMaxValue(const Mat imageInput, Mat& imageOutput, const Mat& theta, const Mat& imageX, const Mat& imageY)
{
	imageOutput = imageInput.clone();


	int cols = imageInput.cols;
	int rows = imageInput.rows;

	for (int i = 1; i < rows - 1; i++)
	{
		for (int j = 1; j < cols - 1; j++)
		{
// Khi continue được thực thi, phần còn lại của lần lặp này được thực thi và lần lặp tiếp theo được bắt đầu
// Khi break được thực thi, vòng lặp chứa nó được thực thi và giai đoạn tiếp theo được thực thi
			if (0 == imageInput.at<uchar>(i, j))continue;

			int g00 = imageInput.at<uchar>(i - 1, j - 1);
			int g01 = imageInput.at<uchar>(i - 1, j);
			int g02 = imageInput.at<uchar>(i - 1, j + 1);

			int g10 = imageInput.at<uchar>(i, j - 1);
			int g11 = imageInput.at<uchar>(i, j);
			int g12 = imageInput.at<uchar>(i, j + 1);

			int g20 = imageInput.at<uchar>(i + 1, j - 1);
			int g21 = imageInput.at<uchar>(i + 1, j);
			int g22 = imageInput.at<uchar>(i + 1, j + 1);

			int direction = theta.at<int>(i, j); //Giá trị góc gradient của pixel này
			int g1 = 0;
			int g2 = 0;
			int g3 = 0;
			int g4 = 0;
			double tmp1 = 0.0; //Lưu số lượng mức xám thu được bằng phép nội suy pixel phụ
			double tmp2 = 0.0;
// Trong C, abs (), labs () và fabs () tính giá trị tuyệt đối của các kiểu int, long và double tương ứng;
// Và trong C ++, abs () đã bị quá tải và có thể được áp dụng cho nhiều loại khác nhau
			double weight = fabs((double)imageY.at<uchar>(i, j) / (double)imageX.at<uchar>(i, j));

			//Nó cũng có thể được loại bỏ.
			//Nếu weight = 0, thì direction = 0 hoặc 180 sẽ không sử dụng chênh lệch và sử dụng trực tiếp giá trị ban đầu.
			if (weight == 0)
				weight = 0.0000001;
			/*
Về ý nghĩa của các công thức này: https://www.cnbloss.com/love6tao/p/5152020.html 
Giải thích:
Hình dạng của g10 * (1-weight + g20 * (weight) là tất cả các pixel phụ được tính từ các pixel ở cả hai bên
Nhưng w và 1-w nên được viết ngược lại
			*/
			if (weight > 1)
			{
				weight = 1 / weight;
			}
			if ((0 <= direction && direction < 45) || 180 <= direction && direction < 225)
			{
				tmp1 = g10 * (1 - weight) + g20 * (weight);
				tmp2 = g02 * (weight)+g12 * (1 - weight);
			}
			if ((45 <= direction && direction < 90) || 225 <= direction && direction < 270)
			{
				tmp1 = g01 * (1 - weight) + g02 * (weight);
				tmp2 = g20 * (weight)+g21 * (1 - weight);
			}
			if ((90 <= direction && direction < 135) || 270 <= direction && direction < 315)
			{
				tmp1 = g00 * (weight)+g01 * (1 - weight);
				tmp2 = g21 * (1 - weight) + g22 * (weight);
			}
			if ((135 <= direction && direction < 180) || 315 <= direction && direction < 360)
			{
				tmp1 = g00 * (weight)+g10 * (1 - weight);
				tmp2 = g12 * (1 - weight) + g22 * (weight);
			}

			if (imageInput.at<uchar>(i, j) < tmp1 || imageInput.at<uchar>(i, j) < tmp2)
			{
				imageOutput.at<uchar>(i, j) = 0;
			}
		}
	}

	return 1;
}

/*
Cơ chế của ngưỡng kép là:
Chỉ định giá trị ngưỡng low A và giá trị ngưỡng high B và chọn 70% tổng biểu đồ là B và B gấp 1,5 đến 2 lần kích thước của A;
Nếu giá trị màu xám nhỏ hơn A, đặt giá trị này thành 0; nếu giá trị màu xám lớn hơn B, hãy đặt giá trị này thành 255;
*/
int CannyEdgeDetector::DoubleThreshold(Mat& imageInput)
{
	const int lowThreshold=getLow();
	const int highThreshold=getHigh();

	int cols = imageInput.cols;
	int rows = imageInput.rows;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			double temp = imageInput.at<uchar>(i, j);
			temp = temp > highThreshold ? (255) : (temp);
			temp = temp < lowThreshold ? (0) : (temp);
			imageInput.at<uchar>(i, j) = temp;
		}
	}

	return 1;
}
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

Các tham số đầu vào:
Tham số 1, imageInput: hình ảnh độ dốc của đầu vào và đầu ra
Tham số 2, lowTh: ngưỡng thấp
Tham số 3, highTh: ngưỡng cao
*/
int CannyEdgeDetector::Apply(Mat& imageInput, Mat& imageOutput )
{
	int lowTh,highTh;
	lowTh = getLow();
	highTh = getHigh();

	int cols = imageInput.cols;
	int rows = imageInput.rows;

	for (int i = 1; i < rows - 1; i++)
	{
		for (int j = 1; j < cols - 1; j++)
		{
			double pix = imageInput.at<uchar>(i, j);
			if (pix != 255)continue;
			bool change = false;
			for (int k = -1; k <= 1; k++)
			{
				for (int u = -1; u <= 1; u++)
				{
					if (k == 0 && u == 0)continue;
					double temp = imageInput.at<uchar>(i + k, j + u);
					if (temp >= lowTh && temp <= highTh)
					{
						imageInput.at<uchar>(i + k, j + u) = 255;
						change = true;
					}
				}
			}

// Nếu muốn kết nối các giá trị pixel, thì nếu bạn kết nối ba giá trị ở trên cùng hoặc một ở bên trái
// Có thể ảnh hưởng đến các pixel trước đó, nghĩa là các pixel liền kề cũng có thể thay đổi
// Phương thức được áp dụng ở đây là trả vị trí hiện tại về góc trên bên trái cho dù pixel nào sẽ bị thay đổi
// Cạnh trên cùng và cạnh ngoài cùng bên trái không thể quay lại góc trên bên trái, sau đó di chuyển sang trái và lên một khoảng trắng tương ứng
// Lưu ý rằng j cần sớm được +1, không cần +1
// Về lý do tại sao i> 1 và j> 2, trên thực tế j> 1 cũng được chấp nhận, chỉ vì j trừ 2 và 2-2 = 0, nhưng nó là +1 nên không có vấn đề gì
			if (change)
			{
				if (i > 1)i--;
				if (j > 2)j -= 2;

			}
		}
	}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (imageInput.at<uchar>(i, j) != 255)
			{
				imageInput.at<uchar>(i, j) = 0;
			}
		}
	}
	imageOutput = imageInput.clone();

	return 1;
}


int main(int argc, char* argv[])
{
	if (argc != 5) {
		cout << "So tham so dau vao khong hop le -_-" << endl;
		return 1;
	}

	char* arg = argv[1];
	if (strcmp(arg, "--canny") == 0)
	{
		int lowTh, highTh;
		char* low = argv[3];
		char* high = argv[4];
		lowTh = stoi(low);
		highTh = stoi(high);

		CannyEdgeDetector x;

		x.setLow(lowTh);
		x.setHigh(highTh);
		cout << x.getLow() + x.getHigh() << endl;

		//Ảnh grayscale đầu vào:
		Mat grayImage = imread(argv[2], 0);
		imshow("gray image", grayImage);

		//Lọc Gaussian
		Mat gausKernel;
		int kernel_size = 5;
		double sigma = 1;
		x.Gaussian_kernel(kernel_size, sigma, gausKernel);
		Mat gausImage;
		//Convolves an image with kernel: Đó là, sử dụng kernel để thực hiện thao tác tích chập trên ảnh
		filter2D(grayImage, gausImage, grayImage.depth(), gausKernel);
		//imshow("gaus image", gausImage);


		//Tính toán độ dốc XY
		Mat imageX, imageY, imageXY;
		//theta là hướng dốc, theta = arctan (imageY / imageX)
		Mat theta;
		x.GradDirection(gausImage, imageX, imageY, imageXY, theta);
		//imshow("XY grad", imageXY);


		//Không triệt tiêu tối đa biên độ dốc
		Mat localImage;
		x.NonLocalMaxValue(imageXY, localImage, theta, imageX, imageY);
		//imshow("Non local maxinum image", localImage);


		//Phát hiện thuật toán ngưỡng kép và kết nối cạnh
		x.DoubleThreshold(localImage);

		Mat CannyImage;
		x.Apply(localImage, CannyImage);
		imshow("CANNY image", CannyImage);


		/*Mat temMat;
		void Canny( InputArray image, OutputArray edges,
							 double threshold1, double threshold2,
							 int apertureSize = 3, bool L2gradient = false );

		void Canny( InputArray dx, InputArray dy,
							 OutputArray edges,
							 double threshold1, double threshold2,
							 bool L2gradient = false );*/
		Mat temMat;
		Canny(grayImage, temMat, 60, 100);
		imshow("opencv canny image", temMat);

		waitKey(0);
		return 0;
	}
}

