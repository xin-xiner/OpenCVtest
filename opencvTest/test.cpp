#include <iostream>
#include <cv.h>
#include <highgui.h>

#include "extractHOG.h"
#include "fileOperate.h"
#include "getClassList.h"
#include "extractColorHist.h"

using namespace std;
using namespace cv;
string classListChar[] = {"coast","forest","highway","inside_city","mountain","Opencountry","street","tallbuilding"};
int classNumber = 8;
vector<string> classList;
string imagePath = "data\\";
string featurePath = "feature\\";
string resultPath = "result\\";
string classTagPath = "classVector\\";


	

int main()
{
	//createFileList(imagePath);
	
	getClassList(imagePath,classList);
	for(int i = 0;i<classList.size();i++)
	{
		cout<<classList[i]<<" class "<<i+1<<endl;
		//createClassTagVector(classTagPath+classList[i]+"\\",i,classList.size());
		//extractFeatures(imagePath+classList[i]+"\\",featurePath+classList[i]+"\\");
		extractColorHistFeature(imagePath+classList[i]+"\\",featurePath+classList[i]+"\\");
	}
	
	cout<<"finish"<<endl;
	system("pause");
	return 0;
}