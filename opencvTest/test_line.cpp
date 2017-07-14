#include <opencv2\opencv.hpp>
#include <vector>
using namespace cv;
using namespace std;
int sdfdsmain()
{
	

	cv::Mat src = imread("road.jpg");

	cv::Mat src_gray;
	cvtColor(src, src_gray, CV_BGR2GRAY);

	/// Reduce the noise so we avoid false circle detection
	//GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );

	//vector<Vec3f> circles;

	/// Apply the Hough Transform to find the circles
	//    HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/8,
	//                  100, 40, 0, 0 );
	//
	//    /// Draw the circles detected
	//    for( size_t i = 0; i < circles.size(); i++ )
	//    {
	//      Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
	//      int radius = cvRound(circles[i][2]);
	//      // circle center
	//      circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );
	//      // circle outline
	//      circle( src, center, radius, Scalar(0,0,255), 3, 8, 0 );
	//    }

	vector<Vec4i> lines;
	HoughLinesP(src_gray, lines, 1, CV_PI / 180, 50, 50, 10);
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		line(src, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3,
			CV_AA);
	}
	/// Show your results
	namedWindow("Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE);
	imshow("Hough Circle Transform Demo", src);
	cv::waitKey(-1);
	return 1;
}