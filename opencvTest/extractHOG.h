#pragma once
#include <iostream>
#include <vector>
#include <string>

#include <cv.hpp>

void extractFeatures(std::string ImagePath,std::string featurePath);
void creatFeatureFile(cv::Mat& Img,std::string featureFileName = std::string(""),std::string featurePath="feature\\");