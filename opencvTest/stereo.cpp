#include "stereo.h"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/contrib/contrib.hpp>
#include<opencv2/legacy/legacy.hpp>
#include "globalParameter.h"
#include "debugLog.h"
using namespace cv;

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


void stereo(const cv::Mat& imageL, const cv::Mat& imageR, cv::Mat& disparityMap)
{
	int SADWindowSize = 0, numberOfDisparities = 0;
	float scale = 1.f;

	StereoSGBM sgbm;

	Size img_size = imageL.size();


	numberOfDisparities = ((img_size.width / 8) + 15) & -16;

	sgbm.preFilterCap = 63;
	sgbm.SADWindowSize = SADWindowSize > 0 ? SADWindowSize : 3;

	int cn = imageL.channels();

	sgbm.P1 = 8 * cn*sgbm.SADWindowSize*sgbm.SADWindowSize;
	sgbm.P2 = 32 * cn*sgbm.SADWindowSize*sgbm.SADWindowSize;
	sgbm.minDisparity = 0;
	sgbm.numberOfDisparities = numberOfDisparities;
	sgbm.uniquenessRatio = 10;
	sgbm.speckleWindowSize = 100;//bm.state->speckleWindowSize;
	sgbm.speckleRange = 32;//bm.state->speckleRange;
	sgbm.disp12MaxDiff = 1;
	sgbm.fullDP = true;//alg == STEREO_HH;

	Mat disp8;
	sgbm(imageL, imageR, disparityMap);
#ifdef SHOW
	disparityMap.convertTo(disp8, CV_8U);
	namedWindow("left", 1);
	imshow("left", imageL);
	namedWindow("right", 1);
	imshow("right", imageR);
	namedWindow("disparity", 0);
	imshow("disparity", disp8);
	cv::waitKey(0);
#endif
}



void stereoGC(const cv::Mat& imageL, const cv::Mat& imageR, cv::Mat& disparityMap)
{
	int SADWindowSize = 0, numberOfDisparities = 0;
	float scale = 1.f;


	CvStereoGCState* state = cvCreateStereoGCState(16, 2);
	CvMat* leftDisp = cvCreateMat(imageL.rows, imageL.cols, CV_16S);
	CvMat* righttDisp = cvCreateMat(imageL.rows, imageL.cols, CV_16S);


	//cv::Mat imageLgray, imageRgray;
	//cv::cvtColor(imageL, imageLgray, COLOR_BGR2GRAY);
	//cv::cvtColor(imageR, imageRgray, COLOR_BGR2GRAY);
	IplImage img1 = IplImage(imageL);
	IplImage img2 = IplImage(imageR);


	cvFindStereoCorrespondenceGC(&img1, &img2, leftDisp, righttDisp, state, 0); // useDisparityGuess
	cvReleaseStereoGCState(&state);

	cv::Mat(leftDisp).copyTo(disparityMap);
	
	//namedWindow("left", 1);
	//imshow("left", cv::Mat(&img1));
	//namedWindow("right", 1);
	//imshow("right", cv::Mat(&img2));
	//namedWindow("disparity", 0);
	//imshow("disparity", disparityMap);
	//cv::waitKey(0);

}





void convertDisparityToDepth(cv::Mat& disparityMap, cv::Mat& depthMap)
{

	/*namedWindow("before", 1);
	imshow("before", disparityMap);*/

	//medianBlur(disparityMap, disparityMap, 5);

	/*namedWindow("after", 1);
	imshow("after", disp8);*/
	//cv::waitKey(10);

	depthMap = cv::Mat(disparityMap.size(), CV_64F);
	std::cout << type2str(disparityMap.type()) << std::endl;
	for (int i = 0; i < disparityMap.rows; i++)
	{
		for (int j = 0; j < disparityMap.cols; j++)
		{
			

			/*if (disparityMap.at<short>(i, j) <= 2)
				depthMap.at<double>(i, j) = 0;
			else */
			double d = ((double)disparityMap.at<unsigned short>(i, j))/256;
				depthMap.at<double>(i, j) = (baseline*fL) / d;//16 is a disparity scale used in openCV ref to document of stereoSGBM for more information

#ifdef DEBUG
			
			double depth_ij = depthMap.at<double>(i, j);
			disp(disparity_ij);
			disp(depth_ij);
#endif
			short disparity_ij = disparityMap.at<unsigned short>(i, j);
			if (!IsNumber(depthMap.at<double>(i, j)) || !IsFiniteNumber(depthMap.at<double>(i, j)) || disparity_ij<=1)
			{
				depthMap.at<double>(i, j) = 0;
			}

		}
	}
	
}