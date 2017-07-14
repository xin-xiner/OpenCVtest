#include "FileOperator.h"
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <Eigen\Core>
#include <Eigen\Geometry>
#include <string>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2\contrib\retina.hpp>

int dfsmain(int argv, char** argc)
{
	cv::Mat frame;
	std::string dataSetPath = argc[1];
	std::string framePath;
	FileParser<Eigen::Vector3f, Eigen::Vector4f> parser(dataSetPath);
	bool play = false;
	char input;
	parser.getNextRGBRecord(framePath);
	frame = cv::imread(dataSetPath + "\\" + framePath);
	cv::Retina retina_frame(frame.size());
	cv::Retina retina_edge(frame.size());
	// reset all retina buffers (imagine you close your eyes for a long time)
	retina_frame.clearBuffers();
	retina_frame.setColorSaturation(false);
	retina_edge.clearBuffers();
	retina_edge.setColorSaturation(false);


	int frame_count = 0;

	
	while (parser.getNextRGBRecord(framePath))//Process while successfully read a frame//wx 按s键终止重建循环
	{
		frame = cv::imread(dataSetPath + "\\" + framePath);
		cv::Mat draw = cv::Mat::zeros(frame.size(), frame.type());
		frame.copyTo(draw);
		std::vector<cv::KeyPoint> keypoints;

		cv::MSER mser;
		mser.detect(frame, keypoints);
		cv::drawKeypoints(frame, keypoints, draw,cv::Scalar(0,255,0));
		cv::imshow("result", draw);
		cv::waitKey(0);
	}
	return 0;
}