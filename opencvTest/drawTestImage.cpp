#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <Eigen\Core>
#include <Eigen\Geometry>
#include <string>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2\contrib\retina.hpp>
#include <vector>
#include <string>
#include <sstream>

//std::vector<cv::Point> generateTriangle(float vertex_x,float vertex_y,float edge_length, float axis_first_edge,float point_angle)
//{
//	cv::Point vertex(vertex_x, vertex_y);
//	float x_left_vertex = vertex_x + cos(axis_first_edge)*edge_length;
//	float y_left_vertex = vertex_y + sin(axis_first_edge)*edge_length;
//}
//
//int main()
//{
//	int width = 640;
//	int height = 480;
//
//	cv::Mat image = cv::Mat::ones(height, width, CV_8U);
//
//	std::vector<cv::Point> triangle;
//	//add all points of the contour to the vector
//	triangle.push_back(cv::Point(x_coord, y_coord));
//	[...]
//
//	std::vector<std::vector<cv::Point> > fillContAll;
//	//fill the single contour 
//	//(one could add multiple other similar contours to the vector)
//	fillContAll.push_back(fillContSingle);
//	cv::fillPoly(image, fillContAll, cv::Scalar(128));
//
//}