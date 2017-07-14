#include "stereo.h"
#include "rectify.h"
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <fstream>
#include "globalParameter.h"
#include "opencv2/calib3d/calib3d.hpp"
using namespace cv;
using namespace std;

void saveAsVTX(cv::Mat& depthMap, cv::Mat& K, std::string& fileName);
static void saveXYZ(const char* filename, const Mat& mat);
int main()
{

	Mat frameL, frameR, disparityMap, depthMap;
	//string imageLpath = "D:\\test\\opencvTest\\opencvTest\\0000000008L.png";
	frameL = imread(string("testImage\\0000000008L.png"), 0);
	frameR = imread(string("testImage\\0000000008R.png"),0);
	disparityMap = imread(string("dispMap\\000005_10.png"),-1);
	//stereoGC(frameL, frameR, disparityMap);
	//stereo(frameL, frameR, disparityMap);
	//Mat disp8;
	//disparityMap.convertTo(disp8, CV_8U);
	convertDisparityToDepth(disparityMap, depthMap);
	Mat xyz;
	double cxl = right_intrinsic.at<double>(0, 2);
	double cyl = right_intrinsic.at<double>(1, 2);
	double fxl = right_intrinsic.at<double>(0, 0);
	double fyl = right_intrinsic.at<double>(1, 1);
	double cxr = right_intrinsic.at<double>(0, 2);
	double cyr = right_intrinsic.at<double>(1, 2);
	double fxr = right_intrinsic.at<double>(0, 0);
	double fyr = right_intrinsic.at<double>(1, 1);
	double t = baseline*fxl;
	Mat Q = Mat(Matx44d(1, 0, 0, cxl, 0, 1, 0, cyl, 0, 0, 0, fxl, 0, 0, -1. /t , (cxl-cxr)/t));
	cout << Q << endl;
	//reprojectImageTo3D(disparityMap, xyz, Q, true);
	//const char* point_cloud_filename = "point.vtx";
	//saveXYZ(point_cloud_filename, xyz);

	//namedWindow("left", 1);
	//imshow("left", cv::Mat(&img1));
	//namedWindow("right", 1);
	//imshow("right", cv::Mat(&img2));
	namedWindow("disparity", 0);
	imshow("disparity", disparityMap);
	namedWindow("depth", 0);
	imshow("depth", depthMap);
	saveAsVTX(depthMap, left_intrinsic, string("result\\pointCloud.vtx"));
	cv::waitKey(0);

}



void saveAsVTX(cv::Mat& depthMap, cv::Mat& K, std::string& fileName)
{
	double* depthData = (double*)depthMap.data;
	int dataSize = depthMap.cols*depthMap.rows;
	int width = depthMap.cols;
	
	double cx = K.at<double>(0, 2);
	double cy = K.at<double>(1, 2);
	double fx = K.at<double>(0, 0);
	double fy = K.at<double>(1, 1);

	ofstream VTXfile(fileName);



	for (int row = 0; row < depthMap.rows; row++)
		for (int col = 0; col < depthMap.cols; col++)
		{
			double Z = depthMap.at<double>(row,col);
			if (Z == 0)
				continue;
			double X = ((col - cx)*Z) / fx;
			double Y = ((row - cy)*Z) / fy;
			VTXfile << X << " " << Y << " " << Z << endl;
		}
}



static void saveXYZ(const char* filename, const Mat& mat)
{
	const double max_z = 1.0e4;
	FILE* fp = fopen(filename, "wt");
	for (int y = 0; y < mat.rows; y++)
	{
		for (int x = 0; x < mat.cols; x++)
		{
			Vec3f point = mat.at<Vec3f>(y, x);
			if (fabs(point[2] - max_z) < FLT_EPSILON || fabs(point[2]) > max_z) continue;
			fprintf(fp, "%f %f %f\n", point[0], point[1], point[2]);
		}
	}
	fclose(fp);
}