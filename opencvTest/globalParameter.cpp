#include "globalParameter.h"


cv::Mat left_intrinsic = cv::Mat(cv::Matx33d(9.842439e+02, 0.000000e+00, 6.900000e+02,
	0.000000e+00, 9.808141e+02, 2.331966e+02,
	0, 0, 1));
cv::Mat right_intrinsic = cv::Mat(cv::Matx33d(9.895267e+02, 0.000000e+00, 7.020000e+02,
	0.000000e+00, 9.878386e+02, 2.455590e+02,
	0, 0, 1));
cv::Mat extrinsic_R = cv::Mat(cv::Matx33d(0.9730203391826149, 0.07468099733653843, 0.218298346704158,
	-0.07187088056887703, 0.9971971125155458, -0.02079652175033575,
	-0.2192395859857501, 0.004546144242972484, 0.9756604616921394));
cv::Mat extrinsic_t = cv::Mat(cv::Matx31d(-0.4230212147674422, -0.005333912645634608, -0.29717607818));


cv::Mat distCoeffsL = cv::Mat(cv::Vec4d(-0.244440474747523, 0.979149874492738, 0, 0));
cv::Mat distCoeffsR = cv::Mat(cv::Vec4d(-0.189113410939168, 0.664655167200717, 0, 0));


double baseline = 0.5400f;
double fL = 9.842439e+02;
double fR = 9.895267e+02;

double fx = left_intrinsic.at<double>(0,0);
double fy = left_intrinsic.at<double>(1, 1);
double cx = left_intrinsic.at<double>(0, 2);
double cy = left_intrinsic.at<double>(0, 2);



bool IsNumber(double x)
{
	return (x == x);
}


bool IsFiniteNumber(double x)
{
	return (x <= DBL_MAX && x >= -DBL_MAX);
}


std::string type2str(int type) 
{
	std::string r;

	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	switch (depth) {
	case CV_8U:  r = "8U"; break;
	case CV_8S:  r = "8S"; break;
	case CV_16U: r = "16U"; break;
	case CV_16S: r = "16S"; break;
	case CV_32S: r = "32S"; break;
	case CV_32F: r = "32F"; break;
	case CV_64F: r = "64F"; break;
	default:     r = "User"; break;
	}

	r += "C";
	r += (chans + '0');

	return r;
}
