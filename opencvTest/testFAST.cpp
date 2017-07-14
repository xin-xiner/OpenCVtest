#include "fastCorner.h"
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\features2d\features2d.hpp>
int main()
{
	cv::Mat img = cv::imread("frame_part.png");
	std::vector<cv::KeyPoint> keypoints;

	std::vector<cv::Mat> rgb;
	cv::split(img, rgb);
	cv::cvtColor(img, img, CV_BGR2GRAY);
	cv::FAST_my<16>(img, keypoints, 50, true);
	
	//cv::drawKeypoints(img, keypoints, img);
	cv::cvtColor(img, img, CV_GRAY2BGR);
	for (int i = 0; i < keypoints.size(); i++)
	{
		img.at<cv::Vec3b>(keypoints[i].pt) = cv::Vec3b(255, 0, 0);
	}
	cv::namedWindow("frame", 0);
	cv::imshow("frame", img);
	

	cv::waitKey(0);
}