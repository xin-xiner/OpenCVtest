#include <iostream>
#include <fstream>
#include <vector>

#include "fileOperate.h"

void getClassList(std::string ImagePath,std::vector<std::string>& classNameList)
{
	getDirectoryList(ImagePath,classNameList);
}

void createClassTagVector(std::string classTagPath,int classId,int classNumber)
{
	std::vector<int> classTag(classNumber,-1);
	classTag[classId] = 1;
	std::ofstream classVector(classTagPath+"class.txt");
	for(int i = 0;i<classNumber;i++)
		classVector<<classTag[i]<<" ";
}