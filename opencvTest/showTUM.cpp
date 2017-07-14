#include "FileOperator.h"
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <Eigen\Core>
#include <Eigen\Geometry>
#include <string>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2\contrib\retina.hpp>

int fsfsdmain(int argv,char** argc)
{
	cv::Mat frame;
	std::string dataSetPath = argc[1];
	std::string framePath;
	FileParser<Eigen::Vector3f, Eigen::Vector4f> parser(dataSetPath);
	bool play = false;
	char input;
	parser.getNextRGBRecord(framePath);
	frame = cv::imread(dataSetPath + "\\" + framePath);
	cv::Retina myRetina(frame.size());

	// reset all retina buffers (imagine you close your eyes for a long time)
	myRetina.clearBuffers();
	myRetina.setColorSaturation(false);

	int frame_count = 0;
	cv::VideoWriter video("points.avi", CV_FOURCC('M', 'J', 'P', 'G'), 10.0, frame.size());
	while (parser.getNextRGBRecord(framePath))//Process while successfully read a frame//wx 按s键终止重建循环
	{
		if (frame_count % 1 == 0)
		{
			frame = cv::imread(dataSetPath + "\\" + framePath);

		
			cv::Mat description;
			cv::ORB orb(100);

			cv::vector<cv::KeyPoint> keypoint;
			//orb.detect(frame, keypoint);
			//for (int i = 0; i < 4; i++)
			//{
			//	for (int j = 0; j < 4; j++)
			//	{
			//		float thres = 10;
			//		cv::ORB orb(thres);
			//		cv::vector<cv::KeyPoint> keyp;
			//		cv::Mat sub(frame, cv::Rect(frame.cols / 4 * i, frame.rows / 4 * j, frame.cols / 4, frame.rows / 4));
			//		//cv::rectangle(frame, cv::Rect(frame.cols / 4 * i, frame.rows / 4 * j, frame.cols / 4, frame.rows / 4),cv::Scalar(i*255/4,j*255/4,255));
			//		orb.detect(sub, keyp);
			//		int run_count = 0;
			//		while (run_count<10&&(keyp.size()<5 || keyp.size()>15))
			//		{
			//			thres += (10 - (float)keyp.size())*20;
			//			if (thres <= 0)
			//				thres = 1;
			//			//std::cout << thres << "   " << keyp.size() << std::endl;
			//			cv::ORB orb(thres);
			//			orb.detect(sub, keyp);
			//			run_count++;
			//		}
			//		for (int t = 0; t < keyp.size();t++)
			//		{
			//			keyp[t].pt.x += frame.cols / 4 * i;
			//			keyp[t].pt.y += frame.rows / 4 * j;
			//		}
			//		keypoint.insert(keypoint.end(),keyp.begin(),keyp.end());
			//	}
			//}
		
			//cv::imshow("tes", frame);
			cv::Mat draw = cv::Mat::zeros(frame.size(), frame.type());
			//frame.copyTo(draw);
			cv::Canny(frame, draw, 180, 150);
		
			/*cv::Mat changes;
			myRetina.run(frame);
			myRetina.getMagno(changes);
			cv::Mat frame_ROI,bin_changes;
			cv::threshold(changes, bin_changes, 30, 255, cv::THRESH_BINARY);
			frame.copyTo(frame_ROI, bin_changes);
			cv::imshow("ROI", frame_ROI);

			cv::cvtColor(changes, changes, CV_GRAY2BGR);
			myRetina.getParvo(frame);
		
			
			
			cv::Mat edge;
			cv::Canny(frame, edge, 30, 150);

			uchar* edge_data = edge.data;
			cv::Vec3b* draw_data = (cv::Vec3b*)draw.data;
			cv::Vec3b* changes_data = (cv::Vec3b*)changes.data;
			for (int i = 0; i < draw.cols*draw.rows; i++)
			{
				if (edge_data[i] >= 200 && bin_changes.data[i] == 255)
				{
					draw_data[i] = cv::Vec3b(255,255,255);
				}
					
			}*/

			/*orb.detect(frame, keypoint);
			for (int i = 0; i < keypoint.size(); i++)
			{
				cv::circle(draw, keypoint[i].pt, 5, cv::Scalar(0, 0, 255),3);
				cv::rectangle(draw, cv::Rect(keypoint[i].pt.x-8, keypoint[i].pt.y-8, 16, 16), cv::Scalar(255, 0, 0),3);
			}*/
			//orb.compute(frame, keypoint, description)
			video << draw;
			cv::imshow("frame", draw);
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
	video.release();
	return 0;
}