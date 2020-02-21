#include "stdafx.h"
#include "GeometricTransformer.h"

Mat affineMultiply(Mat _matrixTransformer, Mat point) {
	Mat result(3, 1, CV_64FC1);
	for (int i = 0; i < 3; i++) {
		result.at<double>(i, 0) = _matrixTransformer.at<double>(i, 0)*point.at<double>(0, 0)
			+ _matrixTransformer.at<double>(i, 1)*point.at<double>(1, 0)
			+ _matrixTransformer.at<double>(i, 2)*point.at<double>(2, 0);
	}
	return result;
}

void AffineTransform::Translate(float dx, float dy) {
	_matrixTransform = Mat::eye(3, 3, CV_64FC1);
	_matrixTransform.at<double>(0, 2) = dx;
	_matrixTransform.at<double>(1, 2) = dy;
}

void AffineTransform::Rotate(float angle) {
	_matrixTransform = Mat::zeros(3, 3, CV_64FC1);
	_matrixTransform.at<double>(0, 0) = cos(angle*3.141592653 / 180.0);
	_matrixTransform.at<double>(0, 1) = -sin(angle*3.141592653 / 180.0);

	_matrixTransform.at<double>(1, 0) = sin(angle*3.141592653 / 180.0);
	_matrixTransform.at<double>(1, 1) = cos(angle*3.141592653 / 180.0);

	_matrixTransform.at<double>(2, 2) = 1;
}

void AffineTransform::Scale(float sx, float sy) {
	_matrixTransform = Mat::eye(3, 3, CV_64FC1);
	_matrixTransform.at<double>(0, 0) = sx;
	_matrixTransform.at<double>(1, 1) = sy;
	_matrixTransform.at<double>(2, 2) = 1;
}

void AffineTransform::TransformPoint(float &x, float &y) {
	Mat point=Mat(3, 1,CV_64FC1);
	point.at<double>(0, 0) = x;
	point.at<double>(1, 0) = y;
	point.at<double>(2, 0) = 1;

	point = affineMultiply(_matrixTransform, point);

	x = point.at<double>(0, 0);
	y = point.at<double>(1, 0);
}

AffineTransform::AffineTransform() {}
AffineTransform::~AffineTransform() {}


PixelInterpolate::PixelInterpolate(){}
PixelInterpolate::~PixelInterpolate(){}


uchar* BilinearInterpolate::Interpolate(float tx, float ty, uchar* pSrc, int srcWidthStep, int nChannels) {
	return 0;
}
BilinearInterpolate::BilinearInterpolate() {}
BilinearInterpolate::~BilinearInterpolate() {}

uchar* NearestNeighborInterpolate::Interpolate(float tx, float ty, uchar* pSrc, int srcWidthStep, int nChannels) {
	int oldX, oldY;
	oldX = round(tx);
	oldY = round(ty);
	pSrc += oldY*srcWidthStep;
	uchar* pRows = pSrc;
	pRows += oldX*nChannels;
	return pRows;
}
NearestNeighborInterpolate::NearestNeighborInterpolate(){}
NearestNeighborInterpolate::~NearestNeighborInterpolate(){}

int GeometricTransformer::Scale(const Mat &srcImage,
	Mat &dstImage,
	float sx, float sy,
	PixelInterpolate* interpolator) {
	if (srcImage.empty()) {
		return 0;
	}
	else {
		AffineTransform transForm;
		transForm.Scale(1/sx,1/sy);
		dstImage = Mat::zeros(round(srcImage.rows*sx),round( srcImage.cols*sy), srcImage.type());
		uchar* pData =(uchar*) srcImage.data;
		int nChannels, widthStep;
		int width = dstImage.cols, height = dstImage.rows;
		nChannels = dstImage.step[1];
		widthStep = dstImage.step[0];
		uchar* pDataDst = (uchar*)dstImage.data;
		for (int y = 0; y < height; y++,pDataDst+=widthStep) {
			uchar* pRowsDst = pDataDst;
			for (int x = 0;x < width; x++,pRowsDst+=nChannels) {
				float oldX,oldY;
				oldX = x;
				oldY = y/sy;
				transForm.TransformPoint(oldX, oldY);
				uchar* color = new uchar[nChannels];
				color = interpolator->Interpolate(oldX, oldY, pData, widthStep, nChannels);
				for (int i = 0; i < nChannels; i++) {
					pRowsDst[i] =color[i];
				}
			}
		}
		return 1;
	}
}

int GeometricTransformer::Transform(
	const Mat &beforeImage,
	Mat &afterImage,
	AffineTransform* transformer,
	PixelInterpolate* interpolator) {
	return 1;
}

int GeometricTransformer::RotateUnkeepImage(
	const Mat &srcImage, Mat &dstImage, float angle, PixelInterpolate* interpolator) {
	if (srcImage.empty()) {
		return 0;
	}
	else {
		AffineTransform transForm;
		transForm.Rotate(angle);
		dstImage = Mat::zeros(srcImage.size(), srcImage.type());
		uchar* pData = (uchar*)srcImage.data;
		int nChannels, widthStep;
		int width = dstImage.cols, height = dstImage.rows;
		nChannels = dstImage.step[1];
		widthStep = dstImage.step[0];
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; width++) {
				float newX, newY;
				newX = x;
				newY = y;
				transForm.TransformPoint(newX, newY);
				int newX1, newY1;
				newX1 = round(newX);
				newY1 = round(newY);
				uchar* color = new uchar[nChannels];
				color = interpolator->Interpolate(x, y, pData, widthStep, nChannels);
				return 1;
				for (int c = 0; c < nChannels; c++)
					dstImage.at<Vec3b>(newX1, newY1)[c] = saturate_cast<uchar>(color[c]);
			}
		}
		return 1;
	}
}

int GeometricTransformer::Flip(
	const Mat &srcImage,
	Mat &dstImage,
	bool direction,
	PixelInterpolate* interpolator) {
	if (srcImage.empty()) {
		return 0;
	}
	else {
		dstImage = Mat::zeros(srcImage.size(), srcImage.type());
		int width = srcImage.cols,height=srcImage.rows;
		int nChannels = srcImage.step[1];
		if (direction == 1) {
			if (height % 2) {
				for (int x = 0; x < width; x++) {
					for (int y = 0; y < height / 2; y++) {
						for (int c = 0; c < nChannels; c++) {
							dstImage.at<Vec3b>(y, x)[c] =
								saturate_cast<uchar>(srcImage.at<Vec3b>(height - y - 1, x)[c]);
							dstImage.at<Vec3b>(height - y - 1, x)[c] =
								saturate_cast<uchar>(srcImage.at<Vec3b>(y, x)[c]);
						}
					}
					for (int c = 0; c < nChannels; c++) {
						dstImage.at<Vec3b>(height/2, x)[c] =
							saturate_cast<uchar>(srcImage.at<Vec3b>(height/2, x)[c]);
					}

				}

			}
			else {
				for (int x = 0; x < width; x++) {
					for (int y = 0; y <= height / 2; y++) {
						for (int c = 0; c < nChannels; c++) {
							dstImage.at<Vec3b>(y, x)[c] =
								saturate_cast<uchar>(srcImage.at<Vec3b>(height - y - 1, x)[c]);
							dstImage.at<Vec3b>(height - y - 1, x)[c] =
								saturate_cast<uchar>(srcImage.at<Vec3b>(y, x)[c]);
						}
					}
				}
			}
		}
		else {
			if (width % 2) {
				for (int y = 0; y < height; y++) {
					for (int x = 0; x < width / 2; x++) {
						for (int c = 0; c < nChannels; c++) {
							dstImage.at<Vec3b>(y, x)[c] =
								saturate_cast<uchar>(srcImage.at<Vec3b>(y, width - x - 1)[c]);
							dstImage.at<Vec3b>(y, width-1-x)[c] =
								saturate_cast<uchar>(srcImage.at<Vec3b>(y, x)[c]);
						}
					}
					for (int c = 0; c < nChannels; c++) {
						dstImage.at<Vec3b>(y, width/2)[c] =
							saturate_cast<uchar>(srcImage.at<Vec3b>(y, width/2)[c]);
					}
				}
			}
			else {
				for (int y = 0; y < height; y++) {
					for (int x = 0; x <= width / 2; x++) {
						for (int c = 0; c < nChannels; c++) {
							dstImage.at<Vec3b>(y, x)[c] =
								saturate_cast<uchar>(srcImage.at<Vec3b>(y, width - x - 1)[c]);
							dstImage.at<Vec3b>(y, width - 1 - x)[c] =
								saturate_cast<uchar>(srcImage.at<Vec3b>(y, x)[c]);
						}
					}
				}
			}
		}
		return 1;
	}
}

GeometricTransformer::GeometricTransformer(){}
GeometricTransformer::~GeometricTransformer(){}