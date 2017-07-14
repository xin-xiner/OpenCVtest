#include "FileOperator.h"
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <Eigen\Core>
#include <Eigen\Geometry>
#include <string>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2\contrib\retina.hpp>

int sdfmain(int argv, char** argc)
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

	cv::Mat edge_current;
	cv::Mat edge_all;
	cv::Mat edge_feature;
	cv::Mat changes;
	uchar* edge_current_data;
	uchar* edge_feature_data;
	uchar* changes_data;
	cv::Vec3b* frame_data = (cv::Vec3b*)frame.data;
	cv::Canny(frame, edge_current, 150, 150);
	cv::Mat frame_ROI, bin_changes, edge_dilate;
	cv::Mat edge_change;
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
	cv::Mat video_frame = cv::Mat::zeros(frame.size() * 4, frame.type());
	cv::VideoWriter video("points.avi", CV_FOURCC('M', 'J', 'P', 'G'), 10.0, video_frame.size());
	//基本的逻辑应该是
	//先提取边缘，然后选择那些有运动响应的部分的边缘作为要跟踪的特征
	//新的一帧进来，计算运动响应，(利用上一帧的要跟踪的特征选择临近的运动响应)在运动响应区倾向于提取更多边缘
	
	//然后现在的问题就是，新的一帧的运动响应区和特征边缘无直接对应
	//为了让这个运动响应区仅响应特征边缘的变动，需要做处理
	while (parser.getNextRGBRecord(framePath))//Process while successfully read a frame//wx 按s键终止重建循环
	{
		cv::Mat draw = cv::Mat::zeros(frame.size(), frame.type());
		frame = cv::imread(dataSetPath + "\\" + framePath);
		frame_data = (cv::Vec3b*)frame.data;
		cv::Canny(frame, edge_current, 150, 150);
		edge_current_data = edge_current.data;

		if (frame_count < 5)
		{
			retina_frame.run(frame);
			retina_frame.getMagno(changes);
			cv::blur(frame, frame, cv::Size(3, 3));
			edge_current_data = edge_current.data;
			cv::Canny(frame, edge_current, 150, 150);
			

			//仅保留有change的边缘
			//cv::threshold(changes, bin_changes, 25, 255, cv::THRESH_BINARY);
			cv::adaptiveThreshold(changes, bin_changes, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 9, 0);
			//cv::dilate(edge_current, edge_dilate, element, cv::Point(-1, -1), 1);
			cv::bitwise_and(edge_current, bin_changes, edge_feature);
			edge_feature_data = edge_feature.data;
			frame.copyTo(frame_ROI, bin_changes);
			cv::imshow("ROI", bin_changes);
			retina_edge.clearBuffers();
			retina_edge.run(edge_feature);
		}
		else
		{
			/*edge_feature_data = edge_feature.data;
			for (int i = 0; i < frame.cols*frame.rows; i++)
			{
				if (edge_feature_data[i] >= 200)
				{
					frame_data[i] = cv::Vec3b(0, 0, 255);
				}
			}*/
			


			cv::Mat frame_ROI, bin_changes, edge_dilate;
			//选择与之前选定的边缘特征对应的运行响应区
			/*if (frame_count == 5)
			{*/
				//计算当前的运动响应
				retina_frame.run(frame);
				retina_frame.getMagno(changes);
				cv::blur(frame, frame, cv::Size(3, 3));
				//cv::threshold(changes, bin_changes, 50, 255, cv::THRESH_BINARY);
				cv::adaptiveThreshold(changes, bin_changes, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 31, 0);
				cv::dilate(edge_feature, edge_dilate, element, cv::Point(-1, -1), 3);
				cv::imshow("changes", bin_changes);
				cv::bitwise_and(bin_changes, edge_dilate, bin_changes);
				cv::namedWindow("edge changes", CV_WINDOW_NORMAL);
				//cvSetWindowProperty("edge changes", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
			/*}*/
			if (frame_count >= 5)
			{
				retina_edge.run(edge_feature);
				retina_edge.getMagno(edge_change);
				uchar* edge_change_data = edge_change.data;
				cv::Mat bin_changes_edge;
				//cv::threshold(edge_change, bin_changes_edge, 50, 255, cv::THRESH_BINARY);
				/*for (int i = 0; i < edge_change.cols*edge_change.rows; i++)
				{
					if (edge_change_data[i]>220)
						edge_change_data[i] = 0;
				}*/
				cv::adaptiveThreshold(edge_change, bin_changes_edge, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 31, 0);
				cv::dilate(edge_feature, edge_dilate, element, cv::Point(-1, -1), 1);
				cv::bitwise_and(bin_changes_edge, edge_dilate, bin_changes_edge);
				//cv::dilate(bin_changes, bin_changes, element, cv::Point(-1, -1), 1);
				//cv::erode(bin_changes_edge, bin_changes_edge, element, cv::Point(-1, -1), 3);
				cv::imshow("edge changes", bin_changes_edge);
				cv::bitwise_or(bin_changes, bin_changes_edge, bin_changes);
			}
			


			frame.copyTo(frame_ROI, bin_changes);
			cv::imshow("ROI", frame_ROI);
			//从筛选出的运动响应区，计算这一帧的edge feature
			cv::Canny(frame, edge_feature, 30, 40);
			uchar* edge_feature_data = edge_feature.data;
			cv::imshow("edge 10", edge_feature);

			cv::Canny(frame, edge_current, 150, 150);
			edge_current_data = edge_current.data;
		
			cv::Vec3b* draw_data = (cv::Vec3b*)draw.data;

			for (int i = 0; i < draw.cols*draw.rows; i++)
			{
				if (edge_feature_data[i] >= 200 && bin_changes.data[i] == 255)
				{
					draw_data[i] = cv::Vec3b(0, 0, 255);
				}
				else
				{
					edge_feature_data[i] = 0;
				}
				if (edge_current_data[i] >= 200)
				{
					//draw_data[i] += cv::Vec3b(255, 0, 0);
				}
			}
		}
		int width = frame.cols;
		int height = frame.rows;

		video_frame(cv::Range(0, 0), cv::Range(height, width)) = frame_ROI;
		/*frame_ROI.copyTo(video_frame(cv::Range(0, 0), cv::Range(height, width)));
		edge_current.copyTo(video_frame(cv::Range(height, 0), cv::Range(height*2, width)));
		edge_feature.copyTo(video_frame(cv::Range(0, width), cv::Range(height, width*2)));
		draw.copyTo(video_frame(cv::Range(height, width), cv::Range(height*2, width * 2)));*/
		//cv::imshow("videoFrame", video_frame);

		cv::imshow("edge", edge_current);
		cv::imshow("frame", draw);
		if (!play)
			input = cv::waitKey(0);
		else
			input = cv::waitKey(30);

		
		if (input == 'p')
			play = false;
		else if (input == 'c' || input == 'C')
			play = true;
		frame_count++;
	}
	video.release();
	return 0;
}