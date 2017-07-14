#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <opencv2\video\tracking.hpp>
#include <opencv2\contrib\retina.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <Eigen\Core>
#include "fileOperator.h"
using namespace cv;
using namespace std;

Mat src; Mat src_gray;
Mat current_frame, last_frame;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

/// Function header
void thresh_callback(int, void*);
cv::Ptr<cv::Retina> myRetina;
std::string rgbImgPath, depthImgPath;
/** @function main */
int asdlfkjmain(int argc, char** argv)
{
	std::string DataSetPath = argv[1];//"E:\\RGBD-dataSet\\rgbd_dataset_freiburg2_xyz\\";
	

	FileParser<Eigen::Vector3f, Eigen::Vector4f> parser(DataSetPath);

	bool init = false;

	while (parser.getNextRGBRecord(rgbImgPath))//Process while successfully read a frame//wx 按s键终止重建循环
	{
		/// Load source image and convert it to gray
		src = imread(DataSetPath + rgbImgPath, 1);

		/// Convert image to gray and blur it
		//cvtColor(src, src_gray, CV_BGR2GRAY);
		src.copyTo(src_gray);
		src_gray.copyTo(current_frame);
		if (!init) 
		{ 
			src_gray.copyTo(last_frame); init = true; 
			

			// if the last parameter is 'log', then activate log sampling (favour foveal vision and subsamples peripheral vision)
			//if (useLogSampling)
			//{
			//	myRetina = new cv::Retina(inputFrame.size(), true, cv::RETINA_COLOR_BAYER, true, 2.0, 10.0);
			//}
			//else// -> else allocate "classical" retina :
			myRetina = new cv::Retina(current_frame.size());

			// reset all retina buffers (imagine you close your eyes for a long time)
			myRetina->clearBuffers();
			myRetina->setColorSaturation(false);
		}
		
		/// Create Window
		char* source_window = "Source";
		namedWindow(source_window, CV_WINDOW_AUTOSIZE);
		imshow(source_window, src);

		createTrackbar(" Canny thresh:", "Source", &thresh, max_thresh, thresh_callback);
		thresh_callback(0, 0);
		current_frame.copyTo(last_frame);
		//waitKey(0);
	}
	return(0);
}

/** @function thresh_callback */
void thresh_callback(int, void*)
{
	Mat drawing,current_edge,last_edge,flow;
	drawing = current_frame - last_frame;
	cv::cvtColor(drawing, drawing, CV_BGR2GRAY);
	cv::threshold(drawing, drawing, thresh, 255, THRESH_BINARY);
	imshow("flow", drawing);
	cv::waitKey(0);
	/*drawing = current_frame - last_frame;
	cv::threshold(drawing, drawing, thresh,255,THRESH_BINARY);
	cv::Canny(current_frame, current_edge, 50, 120);
	cv::Canny(last_frame, last_edge, 50, 120);
	cv::cvtColor(drawing, drawing, CV_GRAY2BGR);
	uchar* cur_edge_data = current_edge.data;
	uchar* last_edge_data = last_edge.data;
	cv::Vec3b* drawing_data = (cv::Vec3b*)drawing.data;
	for (int i = 0; i < drawing.cols*drawing.rows;i++)
	{
		if (cur_edge_data[i] >= 200)
			drawing_data[i] = cv::Vec3b(0, 0, 255);
		else if (last_edge_data[i] >= 200)
			drawing_data[i] = cv::Vec3b(0, 255, 0);
	}*/
	
	
	//cv::calcOpticalFlowSF(last_frame, current_frame, flow, 3, 2, 4);
	Mat xy[2];
	//split(flow, xy);

	//Mat magnitude, angle;
	//cartToPolar(xy[0], xy[1], magnitude, angle, true);

	////translate magnitude to range [0;1]
	double mag_max, mag_min;
	//minMaxLoc(magnitude, 0, &mag_max);
	//magnitude.convertTo(magnitude, -1, 1.0 / mag_max);

	////build hsv image
	//Mat _hsv[3], hsv;
	//_hsv[0] = angle;
	//_hsv[1] = Mat::ones(angle.size(), CV_32F);
	//_hsv[2] = magnitude;
	//merge(_hsv, 3, hsv);

	////convert to BGR and show
	//Mat bgr;//CV_32FC3 matrix
	//cvtColor(hsv, bgr, COLOR_HSV2BGR);
	//imshow("flow", bgr);
	//cv::waitKey(0);
	

	// declare retina output buffers
	cv::Mat retinaOutput_parvo;
	cv::Mat retinaOutput_magno;

	myRetina->run(current_frame);
	// Retrieve and display retina output
	myRetina->getParvo(retinaOutput_parvo);
	myRetina->getMagno(retinaOutput_magno);
	cv::imshow("Retina Parvo", retinaOutput_parvo);
	cv::imwrite(rgbImgPath, retinaOutput_parvo);
	//blur(retinaOutput_parvo, retinaOutput_parvo, cv::Size(20, 20));
	//blur(current_frame, last_edge, cv::Size(20, 20));
	//cv::imshow("Retina blur", retinaOutput_parvo);
	//cv::Laplacian(retinaOutput_parvo, current_edge, CV_8U, 3);
	cv::Mat hsv_parvo,v_parvo[3];
	cv::cvtColor(retinaOutput_parvo, hsv_parvo, CV_BGR2HSV);
	
	cv::split(hsv_parvo, v_parvo);
	//blur(v_parvo[2], v_parvo[2], cv::Size(3, 3));
	cv::imshow("v_parvo", v_parvo[2]);
	//cv::Laplacian(v_parvo[2], current_edge, CV_8U, 3);
	cv::Sobel(v_parvo[2], xy[0], CV_32F, 1, 0);
	cv::Sobel(v_parvo[2], xy[1], CV_32F, 0, 1);
	cv::sqrt(xy[0].mul(xy[0]) + xy[1].mul(xy[1]), current_edge);

	std::string path_edge = "D:\\dataSets\\";
	current_edge = cv::imread(path_edge + rgbImgPath);
	cv::cvtColor(current_edge, current_edge, CV_BGR2GRAY);
	cv::threshold(current_edge, current_edge, 30, 255, THRESH_BINARY);
	cv::imshow("edge_test", current_edge);
	cv::waitKey(10);

	//cv::Canny(retinaOutput_parvo, current_edge, 150, 150);
	cv::Canny(last_edge, last_edge, 100, 30);
	//cv::threshold(retinaOutput_magno, retinaOutput_magno, thresh, 255, THRESH_BINARY);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(current_edge, contours, hierarchy, RETR_TREE, 1);
	
	minMaxLoc(current_edge, &mag_min, &mag_max);
	current_edge = (current_edge - mag_min) / (mag_max - mag_min);
	retinaOutput_magno.convertTo(retinaOutput_magno, CV_32F);
	minMaxLoc(retinaOutput_magno, &mag_min, &mag_max);
	retinaOutput_magno = (retinaOutput_magno - mag_min) / (mag_max - mag_min);
	cv::imshow("Retina Magno", retinaOutput_magno);
	//current_edge = current_edge + retinaOutput_magno;
	minMaxLoc(current_edge, &mag_min, &mag_max);
	current_edge = (current_edge - mag_min) / (mag_max - mag_min);
	current_edge = current_edge*255;
	current_edge.convertTo(current_edge, CV_8U);
	//cv::cvtColor(current_edge, current_edge, CV_BGR2GRAY);
	//cv::threshold(current_edge, current_edge, thresh, 255, THRESH_BINARY);
	imshow("edge", current_edge);
	
	
	cv::cvtColor(retinaOutput_magno, retinaOutput_magno, CV_GRAY2BGR);
	uchar* cur_edge_data = current_edge.data;
	cv::Vec3b* drawing_data = (cv::Vec3b*)retinaOutput_magno.data;
	/*for (int i = 0; i < retinaOutput_magno.cols*retinaOutput_magno.rows; i++)
	{
		if (cur_edge_data[i] >= 200)
			drawing_data[i] = cv::Vec3b(0, 0, 255);
	}*/
	drawContours(retinaOutput_magno, contours, -1, Scalar(255, 0, 255),
		1, CV_AA, hierarchy);
	cv::imshow("Retina Magno edge", retinaOutput_magno);
	cv::waitKey(0);
	


	/// Show in a window
	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	
	//imshow("original edge", last_edge);
}