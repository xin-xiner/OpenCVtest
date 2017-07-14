#include "FileOperator.h"
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <Eigen\Core>
#include <Eigen\Geometry>
#include <string>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2\contrib\retina.hpp>

int jlkmain(int argv, char** argc)
{
	cv::Mat frame;
	std::string dataSetPath = argc[1];
	frame = cv::imread(dataSetPath);
	cv::Mat edge;
	cv::Canny(frame, edge, 210, 255);

	
	cv::Mat draw(edge.size(), CV_8UC3);
	uchar* edge_data = edge.data;
	cv::Vec3b* edge_draw_data = (cv::Vec3b*)draw.data;
	for (int i = 0; i < edge.cols*edge.rows; i++)
	{
		if (edge_data[i]>250)
			edge_draw_data[i] = cv::Vec3b(0, 0, 255);
		else
			edge_draw_data[i] = cv::Vec3b(255, 255, 255);
	}
	cv::imshow("draw", draw);
	cv::waitKey(0);
	cv::imwrite("edge.png", draw);
	return 0;
}

int sadgmain(int argv, char** argc)
{
	cv::Mat frame;
	std::string dataSetPath = argc[1];
	std::string framePath;
	FileParser<Eigen::Vector3f, Eigen::Vector4f> parser(dataSetPath);
	bool play = false;
	char input;
	parser.getNextRGBRecord(framePath);
	frame = cv::imread(dataSetPath + "\\" + framePath);

	int frame_count = 0;

	cv::Mat last_frame = frame;
	cv::Mat description, last_frame_description;
	cv::vector<cv::KeyPoint> keypoint, last_frame_keypoint;
	while (parser.getNextRGBRecord(framePath))//Process while successfully read a frame//wx 按s键终止重建循环
	{
		if (frame_count % 5 == 0)
		{
			frame = cv::imread(dataSetPath + "\\" + framePath);


			
			cv::ORB orb(100);

			
			orb.detect(frame, keypoint);
			

			//cv::imshow("tes", frame);
			cv::Mat draw = cv::Mat::zeros(frame.size(), frame.type());
			frame.copyTo(draw);
			cv::Canny(frame, draw, 150, 255);

			cv::imshow("draw", draw);
			/*orb.compute(frame, keypoint, description);

			if (frame_count != 0)
			{
				cv::BFMatcher matcher;
				std::vector<cv::DMatch> matches;
				matcher.match(description, last_frame_description, matches);
				cv::drawMatches(frame, keypoint, last_frame, last_frame_keypoint, matches, draw);

				cv::Mat draw_frame, draw_lastframe;
				frame.copyTo(draw_frame);
				last_frame.copyTo(draw_lastframe);
				for (int i = 0; i < keypoint.size(); i++)
				{
					cv::circle(draw_frame, keypoint[i].pt, 5, cv::Scalar(0, 0, 255), 3);
					cv::rectangle(draw_frame, cv::Rect(keypoint[i].pt.x - 8, keypoint[i].pt.y - 8, 16, 16), cv::Scalar(255, 0, 0), 3);
				}
				for (int i = 0; i < last_frame_keypoint.size(); i++)
				{
					cv::circle(draw_lastframe, last_frame_keypoint[i].pt, 5, cv::Scalar(0, 0, 255), 3);
					cv::rectangle(draw_lastframe, cv::Rect(last_frame_keypoint[i].pt.x - 8, last_frame_keypoint[i].pt.y - 8, 16, 16), cv::Scalar(255, 0, 0), 3);
				}
				cv::imshow("frame", draw_frame);
				cv::imshow("last_frame", draw_lastframe);
				cv::imshow("draw", draw);
			}
			frame.copyTo(last_frame);
			description.copyTo(last_frame_description);
			last_frame_keypoint = keypoint;*/

			if (!play)
				input = cv::waitKey(0);
			else
				input = cv::waitKey(30);

			if (input == 'p')
				play = false;
			else if (input == 'c')
				play = true;
		}


		frame_count++;
	}
	return 0;
}