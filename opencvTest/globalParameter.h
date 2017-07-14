#include <opencv2\core\core.hpp>
#include "debugLog.h"
#include <fstream>


	extern cv::Mat left_intrinsic; 
	extern cv::Mat right_intrinsic;
	extern cv::Mat extrinsic_R;
	extern cv::Mat extrinsic_t;
	extern cv::Mat distCoeffsL;
	extern cv::Mat distCoeffsR;
	extern double baseline;
	extern double fL;
	extern double fx;
	extern double fy;
	extern double cx;
	extern double cy;
	extern double fR;



	bool IsNumber(double x);


	bool IsFiniteNumber(double x);
	std::string type2str(int type);

	extern std::ofstream out34;
	extern std::ofstream out23;
	extern std::ofstream out13;
	extern std::ofstream out14;
	extern std::ofstream outMean;
	extern std::ofstream outmin;
	extern std::ofstream outmax;