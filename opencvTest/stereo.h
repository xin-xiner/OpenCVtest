#include <opencv2\core\core.hpp>


void stereo(const cv::Mat& imageL, const cv::Mat& imageR, cv::Mat& disparityMap);
void convertDisparityToDepth(cv::Mat& disparityMap, cv::Mat& depthMap);
void stereoGC(const cv::Mat& imageL, const cv::Mat& imageR, cv::Mat& disparityMap);