#include "rectify.h"
#include <opencv2\features2d\features2d.hpp>
#include <opencv2\nonfree\features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "globalParameter.h"
#include <iostream>
using namespace cv;



void match(Mat &leftDescriptor, Mat &rightDescriptor,
	vector<KeyPoint> &leftKeyPoint, vector<KeyPoint> &rightKeyPoint,
	vector<DMatch> &matches, Mat &fundamentalMat, vector<Point2f>& leftPoints, vector<Point2f>& rightPoints)
{
	/* Empirical threshold */
	static const float KNN_THRESHOLD = 0.3;

	//FlannBasedMatcher matcher;
	BFMatcher matcher;
	vector<vector<DMatch> > knnMatches;

	vector<Point2f> leftTemp;
	vector<Point2f> rightTemp;
	leftTemp.reserve(leftKeyPoint.size());
	rightTemp.reserve(rightKeyPoint.size());

	matcher.knnMatch(leftDescriptor, rightDescriptor, knnMatches, 2);
	for (int i = 0; i < knnMatches.size(); ++i) {
		cv::DMatch best = knnMatches[i][0], good = knnMatches[i][1];
		if (best.distance < good.distance * KNN_THRESHOLD) {
			matches.push_back(best);
			leftTemp.push_back(leftKeyPoint[best.queryIdx].pt);
			rightTemp.push_back(rightKeyPoint[best.trainIdx].pt);
		}
	}
	
	std::vector<uchar> mask;
	fundamentalMat = findFundamentalMat(leftTemp, rightTemp,
		FM_RANSAC, 3.0, 0.99, mask);

	leftPoints.reserve(matches.size());
	rightPoints.reserve(matches.size());

	for (int i = 0; i < matches.size(); i++)
	{
		if (mask[i])
		{
			leftPoints.push_back(leftTemp[i]);
			rightPoints.push_back(rightTemp[i]);
		}
	}
	fundamentalMat = findFundamentalMat(leftPoints, rightPoints,
		FM_RANSAC, 3.0, 0.99, mask);
	
}


void rectify(cv::Mat& imageL, cv::Mat& imageR, cv::Mat& imageLr, cv::Mat& imageRr, cv::Mat& H1, cv::Mat& H2)
{
	Mat imgL, imgR;
	imageL.copyTo(imgL);
	imageR.copyTo(imgR);

	undistort(imageL, imageR, left_intrinsic, distCoeffsL);
	undistort(imageL, imageR, right_intrinsic, distCoeffsR);

	//cv::imshow("undistort", imageL);
	//cv::imshow("undistortR", imageR);
	//cv::waitKey(0);
		Size imageSize = imgL.size();;
		

		Mat R1, R2, P1, P2, F;

		//stereoRectify(cameraMatrixL, distCoeffsL,
		//	cameraMatrixR, distCoeffsL,
		//	imageSize, R, t, R1, R2, P1, P2, Q,
		//	CALIB_ZERO_DISPARITY, 1, imageSize);

		/*cv::Ptr<cv::FeatureDetector> detector = cv::FeatureDetector::create("SIFT");
		cv::Ptr<cv::DescriptorExtractor> extractor = cv::DescriptorExtractor::create("SIFT");*/


		//cv::FastFeatureDetector detector;
		//cv::OrbDescriptorExtractor extractor;
		cv::SiftFeatureDetector     detector;
		cv::SiftDescriptorExtractor extractor;


		std::vector<cv::KeyPoint> keyPointVectorL;
		cv::Mat descriptorMatL;
		detector.detect(imgL, keyPointVectorL);
		extractor.compute(imgL, keyPointVectorL, descriptorMatL);

		std::vector<cv::KeyPoint> keyPointVectorR;
		cv::Mat descriptorMatR;
		detector.detect(imgR, keyPointVectorR);
		extractor.compute(imgR, keyPointVectorR, descriptorMatR);


		vector<Point2f> leftPoints;
		vector<Point2f> rightPoints;
		cv::Mat fundamentalMat;
		vector<DMatch> matches;

		match(descriptorMatL, descriptorMatR,
			keyPointVectorL, keyPointVectorR,
			matches, fundamentalMat, leftPoints, rightPoints);
		
		stereoRectifyUncalibrated(Mat(leftPoints), Mat(rightPoints), fundamentalMat, imgL.size(), H1, H2, 3);

		//Mat rmap[2][2];




		//vector<Point2f> allimgpt[2];

		//F = findFundamentalMat(Mat(allimgpt[0]), Mat(allimgpt[1]), FM_8POINT, 0, 0);
		//Mat H1, H2;
		//undistortPoints();
		//stereoRectifyUncalibrated(Mat(allimgpt[0]), Mat(allimgpt[1]), F, imageSize, H1, H2, 3);

		//R1 = left_intrinsic.inv()*H1*left_intrinsic;
		//R2 = right_intrinsic.inv()*H2*right_intrinsic;
		//P1 = left_intrinsic;
		//P2 = right_intrinsic;

		
		//initUndistortRectifyMap(left_intrinsic, distCoeffsL, R1, P1, imageSize, CV_32FC1, rmap[0][0], rmap[0][1]);
		//initUndistortRectifyMap(right_intrinsic, distCoeffsR, R2, P2, imageSize, CV_32FC1, rmap[1][0], rmap[1][1]);
		//remap(imgL, imgL, rmap[0][0], rmap[0][1], CV_INTER_LINEAR);
		//remap(imgR, imgR, rmap[1][0], rmap[1][1], CV_INTER_LINEAR);
		warpPerspective(imgL, imageLr, H1, imgL.size());
		warpPerspective(imgR, imageRr, H2, imgR.size());

		//imageLr = imgL;
		//imageRr = imgR;

		//Q = cv::Mat::eye(4, 4, CV_64F);
		
		//reverseRectify = R1.inv();
}
void reWrap(cv::Mat& image, cv::Mat& imageOrigin, cv::Mat& H)
{
	cv::Mat rmap[2][2];
	cv::Mat P1;
	initUndistortRectifyMap(left_intrinsic, distCoeffsL, H, P1, image.size(), CV_32FC1, rmap[0][0], rmap[0][1]);
	remap(image, imageOrigin, rmap[0][0], rmap[0][1], CV_INTER_LINEAR);
}


void rectify_trans(cv::Mat& K1, const cv::Mat& R1, const cv::Mat& t1, cv::Mat& K2, const cv::Mat& R2, const cv::Mat& t2, cv::Size size, cv::Mat& T1, cv::Mat& T2)
{
	Mat_<double> Kn1(3, 3), Kn2(3, 3);
	Mat_<double> c1(3, 1), c2(3, 1),
		v1(3, 1), v2(3, 1), v3(3, 1);

	K1.copyTo(Kn1);
	K2.copyTo(Kn2);

	// optical centers
	c1 = -R1.t() * t1;
	c2 = -R2.t() * t2;

	// x axis
	v1 = c2 - c1;
	// y axis
	v2 = Mat(R1.row(2).t()).cross(v1);
	// z axis
	v3 = v1.cross(v2);

	Mat_<double> m_R(3, 3);
	v1 = v1 / norm(v1);
	v2 = v2 / norm(v2);
	v3 = v3 / norm(v3);

	m_R(0, 0) = v1(0), m_R(0, 1) = v1(1), m_R(0, 2) = v1(2);
	m_R(1, 0) = v2(0), m_R(1, 1) = v2(1), m_R(1, 2) = v2(2);
	m_R(2, 0) = v3(0), m_R(2, 1) = v3(1), m_R(2, 2) = v3(2);

	T1 = (Kn1 * m_R) * (R1.t() * K1.inv());
	T2 = (Kn2 * m_R) * (R2.t() * K2.inv());

	// Image center displacement
	Mat_<double> o1(3, 1), o2(3, 1);
	o1(0) = 0.5 * size.width, o1(1) = 0.5 * size.height, o1(2) = 1.0;
	o2(0) = 0.5 * size.width, o2(1) = 0.5 * size.height, o2(2) = 1.0;

	Mat_<double> on1(3, 1), on2(3, 1), d1(2, 1), d2(2, 1);
	on1 = T1 * o1;
	on2 = T2 * o2;

	d1(0) = o1(0) - on1(0) / on1(2);
	d1(1) = o1(1) - on1(1) / on1(2);
	d2(0) = o2(0) - on2(0) / on1(2);
	d2(1) = o2(1) - on2(1) / on1(2);
	d1(1) = d2(1);

	Kn1(0, 2) = Kn1(0, 2) + d1(0);
	Kn1(1, 2) = Kn1(1, 2) + d1(1);
	Kn2(0, 2) = Kn2(0, 2) + d2(0);
	Kn2(1, 2) = Kn2(1, 2) + d2(1);

	T1 = (Kn1 * m_R) * (R1.t() * K1.inv());
	T2 = (Kn2 * m_R) * (R2.t() * K2.inv());
}


void rectify_t(cv::Mat& imageL, cv::Mat& imageR, cv::Mat& imageLr, cv::Mat& imageRr, cv::Mat& H1, cv::Mat& H2)
{
	Mat imgL, imgR;
	imageL.copyTo(imgL);
	imageR.copyTo(imgR);


	Size imageSize = imgL.size();;


	Mat R1, R2, P1, P2, F;

	rectify_trans(left_intrinsic, cv::Mat::eye(3, 3, CV_64F), cv::Mat::zeros(3, 1, CV_64F), right_intrinsic, extrinsic_R, extrinsic_t, imageSize, H1, H2);

	warpPerspective(imgL, imgL, H1, imgL.size());
	warpPerspective(imgR, imgR, H2, imgR.size());

	imageLr = imgL;
	imageRr = imgR;
}