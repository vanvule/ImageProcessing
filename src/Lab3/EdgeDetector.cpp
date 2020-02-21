#include "stdafx.h"
#include "EdgeDetector.h"
#include<memory>
int EdgeDetector::DetectEdge(Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight, int method)
{
	if (sourceImage.empty())
	{
		return 0;
	}
	else
	{
		if (method != 3) {
			int gx, gy, sum;
			destinationImage = sourceImage.clone();
			for (int y = 0; y < sourceImage.rows; y++)
				for (int x = 0; x < sourceImage.cols; x++)
					destinationImage.at<uchar>(y, x) = 0.0;

			for (int y = 1; y < sourceImage.rows - 1; y++) {
				for (int x = 1; x < sourceImage.cols - 1; x++) {
					gx = xGradient(sourceImage, x, y, method);
					gy = yGradient(sourceImage, x, y, method);
					sum = abs(gx) + abs(gy);
					sum = sum > 255 ? 255 : sum;
					sum = sum < 0 ? 0 : sum;
					destinationImage.at<uchar>(y, x) = sum;
				}
			}
			return 1;
		}
		else if (method == 3) {
			Laplacian(sourceImage, destinationImage, sourceImage.depth());
		}
	}
}


int xGradient(Mat image, int x, int y, int method) {
	if (method == 1) {
		return image.at<uchar>(y - 1, x - 1) +
			2 * image.at<uchar>(y, x - 1) +
			image.at<uchar>(y + 1, x - 1) -
			image.at<uchar>(y - 1, x + 1) -
			2 * image.at<uchar>(y, x + 1) -
			image.at<uchar>(y + 1, x + 1);
	}
	else if (method == 2) {
		return image.at<uchar>(y - 1, x - 1) +
			image.at<uchar>(y, x - 1) +
			image.at<uchar>(y + 1, x - 1) -
			image.at<uchar>(y - 1, x + 1) -
			image.at<uchar>(y, x + 1) -
			image.at<uchar>(y + 1, x + 1);
	}
}


int yGradient(Mat image, int x, int y, int method)
{
	if (method == 1) {
		return image.at<uchar>(y - 1, x - 1) +
			2 * image.at<uchar>(y - 1, x) +
			image.at<uchar>(y - 1, x + 1) -
			image.at<uchar>(y + 1, x - 1) -
			2 * image.at<uchar>(y + 1, x) -
			image.at<uchar>(y + 1, x + 1);
	}
	else if (method == 2) {
		return image.at<uchar>(y - 1, x - 1) +
			image.at<uchar>(y - 1, x) +
			image.at<uchar>(y - 1, x + 1) -
			image.at<uchar>(y + 1, x - 1) -
			image.at<uchar>(y + 1, x) -
			image.at<uchar>(y + 1, x + 1);
	}
}

int Laplacian(const unsigned char* src_, unsigned char* dst_, int width_, int height_)
{
	const int kernel_size{ 3 };
	std::vector<float> kernel;
	int ksize_ = width_*height_;
	if (ksize_ == 1) kernel = { 0.f, 1.f, 0.f, 1.f, -4.f, 1.f, 0.f, 1.f, 0.f };
	else kernel = { 2.f, 0.f, 2.f, 0.f, -8.f, 0.f, 2.f, 0.f, 2.f };
	int new_width = width_ + kernel_size - 1, new_height = height_ + kernel_size - 1;
	std::unique_ptr<unsigned char[]> data(new unsigned char[new_width * new_height]);
	unsigned char* p = data.get();

	for (int y = 0; y < new_height; ++y) {
		if (y != 0 && y != new_height - 1) {
			for (int x = 0; x < new_width; ++x) {
				if (x == 0) {
					p[y * new_width + x] = src_[(y - 1) * width_ + 1];
				}
				else if (x == new_width - 1) {
					p[y * new_width + x] = src_[(y - 1) * width_ + (width_ - 1 - 1)];
				}
				else {
					p[y * new_width + x] = src_[(y - 1) * width_ + (x - 1)];
				}
			}
		}

		if (y == new_height - 1) {
			for (int x = 0; x < new_width; ++x) {
				p[y * new_width + x] = p[(y - 2) * new_width + x];
			}

			for (int x = 0; x < new_width; ++x) { // y = 0
				p[x] = p[2 * new_width + x];
			}
		}
	}

	for (int y = 1; y < new_height - 1; ++y) {
		for (int x = 1; x < new_width - 1; ++x) {
			float value{ 0.f };
			int count{ 0 };
			for (int m = -1; m <= 1; ++m) {
				for (int n = -1; n <= 1; ++n) {
					value += p[(y + m) * new_width + (x + n)] * kernel[count++];
				}
			}

			if (value < 0.) dst_[(y - 1) * width_ + (x - 1)] = 0;
			else if (value > 255.) dst_[(y - 1) * width_ + (x - 1)] = 255;
			else dst_[(y - 1) * width_ + (x - 1)] = static_cast<unsigned char>(value);
		}
	}

	return 1;
}

