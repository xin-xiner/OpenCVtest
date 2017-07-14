#pragma once
#include <iostream>
#include <vector>
#include <string>

#include "fileOperate.h"
#include "getClassList.h"

void getClassList(std::string ImagePath,std::vector<std::string>& classNameList);
void createClassTagVector(std::string classTagPath,int classId,int classNumber);
void createFileList(std::string imagePath);