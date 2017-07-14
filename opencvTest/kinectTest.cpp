#include "KinectReader.h"
#include <omp.h>
#include "debugLog.h"
int main()
{
	KinectReader capture[4];

	cv::Mat depth[4], rgb[4];
	cv::Mat depthNormed[4];
	for (int i = 0; i < 4; i++)
	{
		depthNormed[i] = cv::Mat(KinectReader::cColorHeight, KinectReader::cColorWidth, CV_8U);
	}
	rgb[0]= cv::Mat::zeros(500, 500, CV_8UC3);
	//cv::imshow("rgb", rgb[0]);
	//cv::VideoWriter output_src("ScreenShoot.avi", CV_FOURCC('M', 'J', 'P', 'G'), 30, cv::Size(1920*0.5,1080*0.5), 1);
	bool Flag = true;

//
//	//#pragma omp parallel for
//	while(1)
//	{
//#pragma omp parallel sections
//
//		if (Flag)
//		{
//			if (capture[k].getNextFrame(depth[k], rgb[k]) && !depth[k].empty() && !rgb[k].empty())
//			{
//				//cv::imshow("depth", depth);
//				//cv::imshow("rgb", rgb);
//#pragma omp critical
//				{
//					output_src << rgb[k];
//				}
//			}
//
//
//			if (cv::waitKey(1) == 's')
//			{
//#pragma omp critical
//				{
//					if (Flag)
//					{
//						Flag = false;
//						output_src.release(); 
//					}
//				}
//				
//			}
//		}
//	}
	IMultiSourceFrame* pMultiSourceFrame[4] = { NULL };
	HRESULT hr[4];
	int count = 0;
	cv::namedWindow("RGB", 0);
	cv::namedWindow("depthNormed", 0);
	while (1)

	{
		for (int i = 0; i < 4; i++)
		{
			if (!capture[i].m_pMultiSourceFrameReader)
			{
				return false;
			} 
			hr[i] = capture[i].m_pMultiSourceFrameReader->AcquireLatestFrame(&pMultiSourceFrame[i]);
		}
		int Flag[4] = { 1,1,1,1 };
#pragma omp parallel sections
		{
#pragma omp section
			{
				if (SUCCEEDED(hr[0]))
				{
					hr[0] = capture[0].processFrame(pMultiSourceFrame[0], depth[0], rgb[0]);
				}

				if (!(SUCCEEDED(hr[0]) && !depth[0].empty() && !rgb[0].empty()))
				{
					Flag[0] = 0;
				}
			}
#pragma omp section
			{
				if (SUCCEEDED(hr[1]))
				{
					hr[1] = capture[1].processFrame(pMultiSourceFrame[1], depth[1], rgb[1]);
				}

				if (!(SUCCEEDED(hr[1]) && !depth[1].empty() && !rgb[1].empty()))
				{
					//exit(-5);
					Flag[1] = 0;
				}
			}
#pragma omp section
			{
				if (SUCCEEDED(hr[2]))
				{
					hr[2] = capture[2].processFrame(pMultiSourceFrame[2], depth[2], rgb[2]);
				}

				if (!(SUCCEEDED(hr[2]) && !depth[2].empty() && !rgb[2].empty()))
				{
					Flag[2] = 0;
				}
			}
#pragma omp section
			{
				if (SUCCEEDED(hr[3]))
				{
					hr[3] = capture[3].processFrame(pMultiSourceFrame[3], depth[3], rgb[3]);
				}

				if (!(SUCCEEDED(hr[3]) && !depth[3].empty() && !rgb[3].empty()))
				{
					Flag[3] = 0;
				}
			}
		}

		char buff[10];
		std::string name = "img\\img";

		for (int i = 0; i < 4; i++)
		{
			if (Flag[i])
				//output_src << rgb[i];
			{
				
				ushort* depthData = depth[i].ptr<ushort>();
				int maxdepth = 0,mindepth = 999999;
				for (int row = 0; row < KinectReader::cColorHeight; row++)
					for (int col = 0; col < KinectReader::cColorWidth; col++)
					{
						if (depthData[row*KinectReader::cColorWidth + col]>maxdepth)
							maxdepth = depthData[row*KinectReader::cColorWidth + col];
						if (depthData[row*KinectReader::cColorWidth + col]<mindepth)
							mindepth = depthData[row*KinectReader::cColorWidth + col];
					}
				uchar* depthNormedData = depthNormed[i].ptr<uchar>();
				for (int row = 0; row < KinectReader::cColorHeight; row++)
					for (int col = 0; col < KinectReader::cColorWidth; col++)
					{
						depthNormedData[row*KinectReader::cColorWidth + col] = depthData[row*KinectReader::cColorWidth + col];//(uchar)255 * (double(depthData[row*KinectReader::cColorWidth + col] - mindepth) / double(maxdepth - mindepth));
						//std::cout << "depth" << depthData[row*KinectReader::cColorWidth + col] << std::endl;
						//std::cout << "depth Norm" << (uchar)255  * (double(depthData[row*KinectReader::cColorWidth + col] - mindepth) / double(maxdepth - mindepth)) << std::endl;
					}
				cv::imshow("RGB", rgb[i]);
				cv::imshow("depthNormed", depthNormed[i]);
				//logMatrix(depthNormedData, KinectReader::cColorHeight, KinectReader::cColorWidth);
				if (cv::waitKey(1) == 'p')
				{
					count++;
					if (cv::imwrite(name + itoa(count, buff, 10) + ".jpg", rgb[i]))
						std::cout << "image saved" << name + itoa(count, buff, 10) + ".jpg" << std::endl;

				}
			}
		}
		if (cv::waitKey(1) == 's')
		{
			break;
		}
	}



	//	while (1)
	//	{
	//		
	//			if (!capture[0].m_pMultiSourceFrameReader)
	//			{
	//				return false;
	//			}
	//			hr[0] = capture[0].m_pMultiSourceFrameReader->AcquireLatestFrame(&pMultiSourceFrame[0]);

	//
	//#pragma omp parallel sections
	//		{
	//#pragma omp section
	//			{
	//				if (SUCCEEDED(hr[0]))
	//				{
	//					hr[0] = capture[0].processFrame(pMultiSourceFrame[0], depth[0], rgb[0]);
	//				}
	//
	//				/*if ((SUCCEEDED(hr[0]) && !depth[0].empty() && !rgb[0].empty()))
	//				{
	//					exit(-5);
	//				}*/
	//			}
	//		}
	////#pragma omp section
	////			{
	////				if (SUCCEEDED(hr[1]))
	////				{
	////					hr[1] = capture[1].processFrame(pMultiSourceFrame[1], depth[1], rgb[1]);
	////				}
	////
	////				if (!(SUCCEEDED(hr[1]) && !depth[1].empty() && !rgb[1].empty()))
	////				{
	////					exit(-5);
	////				}
	////			}
	////#pragma omp section
	////			{
	////				if (SUCCEEDED(hr[2]))
	////				{
	////					hr[2] = capture[2].processFrame(pMultiSourceFrame[2], depth[2], rgb[2]);
	////				}
	////
	////				if (!(SUCCEEDED(hr[2]) && !depth[2].empty() && !rgb[2].empty()))
	////				{
	////					exit(-5);
	////				}
	////			}
	////#pragma omp section
	////			{
	////				if (SUCCEEDED(hr[3]))
	////				{
	////					hr[3] = capture[3].processFrame(pMultiSourceFrame[3], depth[3], rgb[3]);
	////				}
	////
	////				if (!(SUCCEEDED(hr[3]) && !depth[3].empty() && !rgb[3].empty()))
	////				{
	////					exit(-5);
	////				}
	////			}
	////		}
	//			//output_src << rgb[0];

	//		cv::imshow("rgb", rgb[0]);
	//		cv::waitKey(1);
	//	}






}