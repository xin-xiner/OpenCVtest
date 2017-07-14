#include <d2d1.h>
#include <windows.h>
#include <Kinect.h>
#include <NuiKinectFusionApi.h>
#pragma comment ( lib, "d2d1.lib" )
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <Eigen\core>
#include <Eigen\geometry>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>



#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

void readPointCloud(std::string fileName, int modelID, NUI_FUSION_IMAGE_FRAME*     my_pPointCloud, Matrix4&     m_worldToCameraTransform);

const unsigned int m_cDepthWidth = NUI_DEPTH_RAW_WIDTH;
const unsigned int m_cDepthHeight = NUI_DEPTH_RAW_HEIGHT;
const unsigned int m_cDepthImagePixels = m_cDepthWidth * m_cDepthHeight;
NUI_FUSION_CAMERA_PARAMETERS m_cameraParameters;
int main()
{
	int frameID = 0;
	NUI_FUSION_IMAGE_FRAME*     my_pPointCloud;
	Matrix4                     m_worldToCameraTransform;
	m_cameraParameters.focalLengthX = 0.716275;
	m_cameraParameters.focalLengthY = 0.864936;
	m_cameraParameters.principalPointX = 0.498276;
	m_cameraParameters.principalPointY = 0.48236;
	readPointCloud("D:\\นคื๗\\probability fusion\\project\\normalTest\\normalTest\\", frameID, my_pPointCloud, m_worldToCameraTransform);
	NUI_FUSION_IMAGE_FRAME*     m_pShadedSurface;
	HRESULT hr = 
	NuiFusionShadePointCloud(my_pPointCloud, &m_worldToCameraTransform, nullptr, m_pShadedSurface, nullptr);


	BYTE * pBuffer = m_pShadedSurface->pFrameBuffer->pBits;

	// Draw the data with Direct2D

	cv::Mat resultImage(m_cDepthHeight, m_cDepthWidth, CV_16U);
	unsigned short* resultImageData = (unsigned short*)resultImage.data;
	unsigned int* pBufferData = (unsigned int*)pBuffer;

	for (int i = 0; i < m_cDepthWidth * m_cDepthHeight; i++)
	{
		resultImageData[i] = (short)pBufferData[i];
	}

	cv::imshow("result", resultImage);
	cv::waitKey(10);
	std::string outPutFileName = "KinectFusionResult\\";

	std::stringstream sst;
	sst << frameID;
	outPutFileName += sst.str() + ".png";
	cv::imwrite(outPutFileName, resultImage);
	cv::waitKey(0);

}




void readPointCloud(std::string filePath,int modelID ,NUI_FUSION_IMAGE_FRAME*     my_pPointCloud, Matrix4&    m_worldToCameraTransform)
{
	std::stringstream sst;
	sst << filePath << "pointCloud\\" << "model" << modelID << ".vtx";
	std::ifstream pointFile(sst.str());
	sst.clear();
	sst.str("");
	sst << filePath << "camera\\" << "camera" << modelID << ".vtx";
	std::ifstream cameraFile(sst.str());
	Eigen::Matrix4d camM;

	double transMat[4][4];
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cameraFile >> transMat[i][j];
		}
	}
	camM <<
		transMat[0][0], transMat[0][1], transMat[0][2], transMat[0][3],
		transMat[1][0], transMat[1][1], transMat[1][2], transMat[1][3],
		transMat[2][0], transMat[2][1], transMat[2][2], transMat[2][3],
		transMat[3][0], transMat[3][1], transMat[3][2], transMat[3][3];

	camM = camM.inverse();
	m_worldToCameraTransform.M11 = camM(0, 0); m_worldToCameraTransform.M12 = camM(0, 1); m_worldToCameraTransform.M13 = camM(0, 2); m_worldToCameraTransform.M14 = camM(0, 3);
	m_worldToCameraTransform.M21 = camM(1, 0); m_worldToCameraTransform.M22 = camM(1, 1); m_worldToCameraTransform.M23 = camM(1, 2); m_worldToCameraTransform.M24 = camM(1, 3);
	m_worldToCameraTransform.M31 = camM(2, 0); m_worldToCameraTransform.M32 = camM(2, 1); m_worldToCameraTransform.M33 = camM(2, 2); m_worldToCameraTransform.M34 = camM(2, 3);
	m_worldToCameraTransform.M41 = camM(3, 0); m_worldToCameraTransform.M42 = camM(3, 1); m_worldToCameraTransform.M43 = camM(3, 2); m_worldToCameraTransform.M44 = camM(3, 3);
	bool inputFlag = true;
	HRESULT hr = S_OK;
	
	hr = NuiFusionCreateImageFrame(NUI_FUSION_IMAGE_TYPE_POINT_CLOUD, m_cDepthWidth, m_cDepthHeight, &m_cameraParameters, &my_pPointCloud);
	unsigned int step = my_pPointCloud->pFrameBuffer->Pitch / my_pPointCloud->width;



	for (int y = 0; y < my_pPointCloud->height; y++)
	{
		for (int x = 0; x < my_pPointCloud->width; ++x)
		{
			*(float*)(my_pPointCloud->pFrameBuffer->pBits + (step * (y * my_pPointCloud->width + x))) = 0;
			*(float*)(my_pPointCloud->pFrameBuffer->pBits + (step * (y * my_pPointCloud->width + x) + 1 * sizeof(float))) = 0;
			*(float*)(my_pPointCloud->pFrameBuffer->pBits + (step * (y * my_pPointCloud->width + x) + 2 * sizeof(float))) = 0;

			*(float*)(my_pPointCloud->pFrameBuffer->pBits + (step * (y * my_pPointCloud->width + x) + 3 * sizeof(float))) = 0;
			*(float*)(my_pPointCloud->pFrameBuffer->pBits + (step * (y * my_pPointCloud->width + x) + 4 * sizeof(float))) = 0;
			*(float*)(my_pPointCloud->pFrameBuffer->pBits + (step * (y * my_pPointCloud->width + x) + 5 * sizeof(float))) = 0;
		}
	}



	float fx = my_pPointCloud->width*m_cameraParameters.focalLengthX;
	float fy = my_pPointCloud->height*m_cameraParameters.focalLengthY;
	float cx = my_pPointCloud->width*m_cameraParameters.principalPointX;
	float cy = my_pPointCloud->height*m_cameraParameters.principalPointY;
	float position[3] = { 0 };
	float normal[3] = { 0 };
	while ((pointFile >> position[0]))
	{
		for (int i = 1; i < 3; i++)
		{
			pointFile >> position[i];
		}
		for (int i = 0; i < 3; i++)
		{
			pointFile >> normal[i];
		}




		int x = position[0] * fx / position[2] + cx;
		int y = position[1] * fy / position[2] + cy;
		if (x >= my_pPointCloud->width || y >= my_pPointCloud->height)
			continue;
		float oldX = *(float*)(my_pPointCloud->pFrameBuffer->pBits + (step * (y * my_pPointCloud->width + x)));
		float oldY = *(float*)(my_pPointCloud->pFrameBuffer->pBits + (step * (y * my_pPointCloud->width + x) + 1 * sizeof(float)));
		float oldZ = *(float*)(my_pPointCloud->pFrameBuffer->pBits + (step * (y * my_pPointCloud->width + x) + 2 * sizeof(float)));

		if ((oldX == 0 && oldY == 0 && oldZ == 0) || (oldX*oldX + oldY*oldY + oldZ*oldZ>position[0] * position[0] + position[1] * position[1] + position[2] * position[2]))
		{
			*(float*)(my_pPointCloud->pFrameBuffer->pBits + (step * (y * my_pPointCloud->width + x))) = position[0] / 2;
			*(float*)(my_pPointCloud->pFrameBuffer->pBits + (step * (y * my_pPointCloud->width + x) + 1 * sizeof(float))) = position[1] / 2;
			*(float*)(my_pPointCloud->pFrameBuffer->pBits + (step * (y * my_pPointCloud->width + x) + 2 * sizeof(float))) = position[2] / 2;

			*(float*)(my_pPointCloud->pFrameBuffer->pBits + (step * (y * my_pPointCloud->width + x) + 3 * sizeof(float))) = normal[0];
			*(float*)(my_pPointCloud->pFrameBuffer->pBits + (step * (y * my_pPointCloud->width + x) + 4 * sizeof(float))) = normal[1];
			*(float*)(my_pPointCloud->pFrameBuffer->pBits + (step * (y * my_pPointCloud->width + x) + 5 * sizeof(float))) = normal[2];
		}
	}
}