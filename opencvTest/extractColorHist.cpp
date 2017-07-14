#include <iostream>
#include <fstream>
#include <vector>

#include <cv.hpp>
#include <core\core.hpp>
#include <highgui\highgui.hpp>
//#include <cvaux.hpp>

#include "extractColorHist.h"
#include "fileOperate.h"
//When it is openCV 3.0, these head files should be include instead of cvaux.hpp
#include <objdetect.hpp>
#include <imgproc.hpp>

using namespace cv;
using namespace std;
void extractColorHistFeature(string ImagePath,string featurePath)
{
	vector<string> imageList;
	getImagesList(ImagePath,imageList);


	for(int i = 0;i<imageList.size();i++)
	{
		Mat img = imread(ImagePath+imageList[i]+".jpg");
		creatColorHistVecotor(img,imageList[i],featurePath);
		img.~Mat();
	}
}




void creatColorHistVecotor(Mat& Img,string featureFileName,string featurePath)
{
 int hbins = 9, sbins = 8;
 int histSize[] = {hbins, sbins};
 float hranges[] = { 0, 180 };
 float sranges[] = { 0, 256 };
 const float* ranges[] = { hranges, sranges };
 int channels[] = {0, 1};

 ofstream fResult(featurePath+featureFileName+"ColorHist.txt");
 for(int patchRow = 0;patchRow<16;patchRow++)
 {
	 for(int patchCol = 0;patchCol<16;patchCol++)
	 {
		 Mat patch = Img(Range(patchRow*16,patchRow*16+16),Range(patchCol*16,patchCol*16+16));
		 cvtColor(patch, patch, COLOR_BGR2HSV); 
		 MatND hist;//结果数组  

		calcHist( &patch, 1, channels, Mat(), // do not use mask
		hist, 2, histSize, ranges,
		true, // the histogram is uniform
		false );
		normalize(hist,hist,1.0);

		for( int h = 0; h < hbins; h++ )
			for( int s = 0; s < sbins; s++ )
			{
				fResult<<hist.at<float>(h, s)<<" ";
			}
		 fResult<<endl;
	 }
 }
 fResult.close();
}
