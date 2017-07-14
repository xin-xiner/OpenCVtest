#pragma once
#include <iostream>
#include <vector>
#include <string>

#include <cv.hpp>

void extractColorHistFeature(std::string ImagePath,std::string featurePath);
void creatColorHistVecotor(cv::Mat& Img,std::string featureFileName = std::string(""),std::string featurePath="feature\\");