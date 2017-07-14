#include <iostream>
#include <fstream>
#include <vector>

#include <cv.hpp>
#include <core\core.hpp>
#include <highgui\highgui.hpp>
//#include <cvaux.hpp>

#include "extractHOG.h"
#include "fileOperate.h"
//When it is openCV 3.0, these head files should be include instead of cvaux.hpp
#include <objdetect.hpp>
#include <imgproc.hpp>

using namespace cv;
using namespace std;
void extractFeatures(string ImagePath,string featurePath)
{
	vector<string> imageList;
	getImagesList(ImagePath,imageList);


	for(int i = 0;i<imageList.size();i++)
	{
		Mat img = imread(ImagePath+imageList[i]+".jpg");
		creatFeatureFile(img,imageList[i],featurePath);
		img.~Mat();
	}
}




void creatFeatureFile(Mat& Img,string featureFileName,string featurePath)
{
 ofstream fResult(featurePath+featureFileName+".txt");
 for(int patchRow = 0;patchRow<16;patchRow++)
 {
	 for(int patchCol = 0;patchCol<16;patchCol++)
	 {
		 Mat patch = Img(Range(patchRow*16,patchRow*16+16),Range(patchCol*16,patchCol*16+16));
		 HOGDescriptor hog(cvSize(16,16),cvSize(16,16),cvSize(16,16),cvSize(8,8),18);  
		 vector<float> descriptors;//结果数组  
		 hog.compute(patch, descriptors,Size(16,16), Size(0,0)); //调用计算函数开始计算  
		 double descriptorsNorm = cv::norm(descriptors);

		 for(int i = 0;i<descriptors.size();i++)
		 {
			 descriptors[i]/=descriptorsNorm;
			 fResult<<descriptors[i]<<" ";
		 }
		 fResult<<endl;
	 }
 }
 fResult.close();
}
