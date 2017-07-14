#include <opencv2\core\core.hpp>



void rectify(cv::Mat& imageL, cv::Mat& imageR, cv::Mat& imageLr, cv::Mat& imageRr, cv::Mat& H1, cv::Mat& H2);
void rectify_t(cv::Mat& imageL, cv::Mat& imageR, cv::Mat& imageLr, cv::Mat& imageRr, cv::Mat& H1, cv::Mat& H2);
void rectify_trans(cv::Mat& K1, cv::Mat& R1, cv::Mat& t1, cv::Mat& K2, cv::Mat& R2, cv::Mat& t2, cv::Size size, cv::Mat& T1, cv::Mat& T2);
void reWrap(cv::Mat& image,cv::Mat& imageOrigin,cv::Mat& H);