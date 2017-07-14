#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
//#include "fileOperate.h"
#include <fstream>
#include <iomanip>
using namespace std;
using namespace cv;

const int max_frame = 1;

int sdfmain()
{
	//string path = "D:\\¹¤×÷\\ÌÖÂÛ°à\\result data\\experiment temp\\renderPicture\\flower_normal0.001\\";
	string path = "E:\\ORB_SLAM2-master\\mono_video\\";
	string fileName = "";
	stringstream ssr;


	VideoWriter video;//("reuslt.avi", CV_FOURCC('M', 'J', 'P', 'G'), 10.0, frame.size());
	float frameRate = 20.0;


		video.open(path + "video" + ".avi", CV_FOURCC('D', 'I', 'V', 'X'), frameRate, cv::Size(1042,350), 1);

		for (int i = 0; i < max_frame; i++)
		{
			std::stringstream sst;
			sst  << std::setfill('0') << std::setw(7) << i<<".png";
			Mat frame = imread(path + "frame\\frame" + sst.str());
			Mat scene = imread(path + "scene\\scene" + sst.str());
			if (frame.empty() || scene.empty())
				continue;
			Mat scene_part = scene(cv::Rect(170, 0, scene.cols - 170, scene.rows-68));
			Mat frame_part = frame(cv::Rect(50, 0, frame.cols-50, frame.rows-40));
			//cv::imshow("scene", scene_part);
			//cv::imshow("frame", frame_part);
			/*char c = cv::waitKey(0);
			if (c == ' ')
			{
				std::string  cmd = "DEL / q ";
				cmd += path + "frame\\" + file_frame[i];
				system(cmd.c_str());
				cmd = "DEL / q ";
				cmd += path + "scene\\" + file_scene[i];
				system(cmd.c_str());
			}
			else if (c>'0'&&c < '9')
			{
				i -= c - '0';
			}*/
			std::cout << frame_part.size() << std::endl;
			std::cout << scene_part.size() << std::endl;
			cv::resize(frame_part, frame_part, cv::Size(0, 0), 0.5, 0.5, cv::INTER_CUBIC);
			cv::resize(scene_part, scene_part, cv::Size(0, 0), 0.5, 0.5, cv::INTER_CUBIC);

			cv::Mat videoFrame;
			scene_part.copyTo(videoFrame);
			videoFrame = videoFrame.t();
			videoFrame.reserve(frame_part.cols);

			videoFrame.push_back(Mat(frame_part.t()));

			videoFrame = videoFrame.t();

			//cv::imshow("scene", scene_part);
			//cv::imshow("frame", frame_part);
			cv::imshow("video", videoFrame);
			std::cout << videoFrame.size() << std::endl;
			if (i % 500 == 0)
				std::cout << i << std::endl;
			video << videoFrame;
			cv::waitKey(1);
		}

		video.release();

		//VideoWriter video;//("reuslt.avi", CV_FOURCC('M', 'J', 'P', 'G'), 10.0, frame.size());
		//float frameFast = 30.0;
		//float frameSlow = 2.0;
		//	video.open(path + "xyz" + ".avi", CV_FOURCC('D', 'I', 'V', 'X'), frameFast, frame.size(), 1);
		////frameID++;
		//	for (int i = 100; i < 400; i++)
		//{
	
		//	video << frame;

		//	imshow("video", frame);
		//	cvWaitKey(10);
		//}
		//
		//video.release();
		//
	//}
		return 0;
}
#include <stack>

//int main(int argc, char** argv)
//{
//	string debugFlag = "gd";
//	string path = "F:\\programPlugs\\VTK 6.30\\lib\\";
//	std::vector<std::string> fileList;
//	
//	getImagesList(path, fileList);
//
//	ofstream libListDebug(path + "libListDebug.txt");
//	ofstream libListRelease(path + "libListRelease.txt");
//
//	for (int i = 0; i < fileList.size(); i++)
//	{
//		if (fileList[i].find(debugFlag) != string::npos)
//			libListDebug << fileList[i] << std::endl;
//		else
//			libListRelease << fileList[i] << std::endl;
//	}
//
//	libListDebug.close();
//	libListRelease.close();
//}

//
//
//class Result
//{
//public:
//	float poseError;
//	float positionError;
//	std::string parameters;
//
//	Result()
//	{
//		poseError = 99999;
//		positionError = 99999;
//	}
//
//	bool operator<(Result& b)
//	{
//		return positionError < b.positionError;
//		//return poseError < b.poseError;
//	}
//
//	friend istream& operator>> (istream& in, Result& r)
//	{
//		std::string line;
//		for (int i = 0; i < 1; i++)
//			getline(in, line);
//		in >> line;
//		in >> r.positionError;
//		//in >> line;
//		//in >> r.poseError;
//		return in;
//	}
//
//	friend ostream& operator<< (ostream& out, Result& r)
//	{
//		out << r.parameters << "   position: " << r.positionError << "  pose: " << r.poseError;
//		return out;
//	}
//};
//
//
//
//int main(int argc, char** argv)
//{
//	string DataSetpath = argv[1];
//	
//
//	std::vector<std::string> directoryList;
//	getDirectoryList(DataSetpath, directoryList);
//	for (int j = 0; j < directoryList.size(); j++)
//	{
//		if (directoryList[j] == "optimized parameters")
//			continue;
//		std::string path = DataSetpath + directoryList[j] + "\\";
//		std::vector<std::string> fileList;
//		getImagesList(path, fileList);
//		if (fileList.size() == 0)
//			continue;
//		std::vector<Result> resultList(fileList.size());
//
//		for (int i = 0; i < fileList.size(); i++)
//		{
//			if (fileList[i][0] != 'c' && fileList[i][0] != 'b'&&fileList[i][0] != 'a')
//				continue;
//			std::ifstream file(path + fileList[i]);
//			if (!file.is_open())
//				std::cout << "error open" << std::endl;
//			if (file >> resultList[i])
//				resultList[i].parameters = fileList[i];
//		}
//
//		std::sort(resultList.begin(), resultList.end());
//
//		int dataSetNamePos = path.substr(0, path.size() - 1).find_last_of('\\');
//		std::string resultPath = path.substr(0, dataSetNamePos + 1);
//		std::string resultFile = path.substr(dataSetNamePos + 1, path.size() - dataSetNamePos - 2);
//		std::ofstream rankResult(resultPath + "optimized parameters\\" + resultFile + ".txt");
//
//		for (int i = 0; i < 60 && i<resultList.size(); i++)
//		{
//			rankResult << resultList[i] << std::endl;
//			//std::cout << resultList[i] << std::endl;
//		}
//		rankResult.close();
//	}
//	system("pause");
//}