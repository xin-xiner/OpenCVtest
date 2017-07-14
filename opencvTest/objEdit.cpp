#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include "fileOperate.h"
using namespace std;

//int main(int argc, char **argv)
//{
//	std::string path = argv[1];
//	std::vector<std::string> fileList;
//	getImagesList(path, fileList);
//
//	int start_x = 140, start_y = 60;
//	int end_x = 780, end_y = 400;
//
//	for (int i = 0; i < fileList.size(); i++)
//	{
//		cv::Mat img = cv::imread(path + "\\" + fileList[i]);
//		cv::Mat img_cut = img(cv::Range(start_y, end_y), cv::Range(start_x, end_x));
//		cv::imshow("cut image", img_cut);
//		cv::imwrite(path + "\\cut_" + fileList[i],img_cut);
//		cv::waitKey(10);
//	}
//
//	return 0;
//
//
//}