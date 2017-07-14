#include "KinectReader.h"
#include <opencv2\imgproc\imgproc.hpp>



KinectReader::KinectReader()
	:
	m_pKinectSensor(NULL),
	m_pCoordinateMapper(NULL),
	m_pMultiSourceFrameReader(NULL),
	m_pColorRGBX(NULL),
	m_pColorCoordinates(NULL),
	m_pDepthCoordinates(NULL)
{


	HRESULT hr;
	m_pColorRGBX = new RGBQUAD[cColorWidth * cColorHeight];
	m_pColorCoordinates = new ColorSpacePoint[cDepthWidth * cDepthHeight];
	m_pDepthCoordinates = new DepthSpacePoint[cColorWidth * cColorHeight];
	hr = GetDefaultKinectSensor(&m_pKinectSensor);
	if (FAILED(hr))
	{
		exit(-9);
	}

	if (m_pKinectSensor)
	{
		// Initialize the Kinect and get coordinate mapper and the frame reader

		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->get_CoordinateMapper(&m_pCoordinateMapper);
		}

		hr = m_pKinectSensor->Open();

		if (SUCCEEDED(hr))
		{
			hr = m_pKinectSensor->OpenMultiSourceFrameReader(
				FrameSourceTypes::FrameSourceTypes_Depth | FrameSourceTypes::FrameSourceTypes_Color,
				&m_pMultiSourceFrameReader);
		}
	}
	if (!m_pKinectSensor || FAILED(hr))
	{
		exit(-9);
	}
}

HRESULT KinectReader::getKinectFrame(IMultiSourceFrame* pMultiSourceFrame)
{

}
bool KinectReader::getNextFrame(cv::Mat& depth, cv::Mat& rgb)
{
	if (!m_pMultiSourceFrameReader)
	{
		return false;
	}

	IMultiSourceFrame* pMultiSourceFrame = NULL;


	HRESULT hr = m_pMultiSourceFrameReader->AcquireLatestFrame(&pMultiSourceFrame);

	if (SUCCEEDED(hr))
	{
		hr = processFrame(pMultiSourceFrame, depth, rgb);
	}

	return hr;
}


void KinectReader::createFrame(
	const UINT16* pDepthBuffer, int nDepthWidth, int nDepthHeight,
	const RGBQUAD* pColorBuffer, int nColorWidth, int nColorHeight,
	cv::Mat& depth, cv::Mat& rgb
	)
{
	// Make sure we've received valid data
	if (m_pCoordinateMapper && m_pColorCoordinates &&
		pDepthBuffer && (nDepthWidth == cDepthWidth) && (nDepthHeight == cDepthHeight) &&
		pColorBuffer && (nColorWidth == cColorWidth) && (nColorHeight == cColorHeight)
		)
	{
		//HRESULT hr = m_pCoordinateMapper->MapDepthFrameToColorSpace(nDepthWidth * nDepthHeight, (UINT16*)pDepthBuffer, nDepthWidth * nDepthHeight, m_pColorCoordinates);
		HRESULT hr = m_pCoordinateMapper->MapColorFrameToDepthSpace(nDepthWidth * nDepthHeight, (UINT16*)pDepthBuffer, nColorWidth * nColorHeight, m_pDepthCoordinates);
		//hr = 10;
		if (SUCCEEDED(hr))
		{
			//**************************************************
			//depth = cv::Mat(cDepthHeight, cDepthWidth, CV_16U);
			//rgb = cv::Mat(cDepthHeight, cDepthWidth, CV_8UC3);//The size should equal with depth map£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡
			//ushort* depthData = depth.ptr<ushort>();
			//cv::Vec3b* colorData = rgb.ptr<cv::Vec3b>();
			//for (int depthIndex = 0; depthIndex < (nDepthWidth*nDepthHeight); ++depthIndex)
			//{
			//	
			//	ColorSpacePoint p = m_pColorCoordinates[depthIndex];

			//	if (p.X != -std::numeric_limits<float>::infinity() && p.Y != -std::numeric_limits<float>::infinity())
			//	{
			//		int colorX = static_cast<int>(p.X + 0.5f);
			//		int colorY = static_cast<int>(p.Y + 0.5f);

			//	/*	if (colorX + (colorY * nColorWidth))
			//			std::cout << depthIndex << std::endl;*/

			//		if ((colorX >= 0 && colorX < nColorWidth) && (colorY >= 0 && colorY < nColorHeight))
			//		{
			//			RGBQUAD pixel = m_pColorRGBX[colorX + (colorY * nColorWidth)];
			//			depthData[depthIndex] = pDepthBuffer[depthIndex];
			//			colorData[depthIndex] = cv::Vec3b(pixel.rgbRed,pixel.rgbGreen,pixel.rgbBlue);
			//		}
			//	}
			//}

			//*******************************************
			depth = cv::Mat(cColorHeight, cColorWidth, CV_16U);
			rgb = cv::Mat(cColorHeight, cColorWidth, CV_8UC3);//The size should equal with depth map£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡£¡
			ushort* depthData = depth.ptr<ushort>();
			cv::Vec3b* colorData = rgb.ptr<cv::Vec3b>();
			for (int colorIndex = 0; colorIndex < (cColorWidth*cColorHeight); ++colorIndex)
			{

				DepthSpacePoint p = m_pDepthCoordinates[colorIndex];

				if (p.X != -std::numeric_limits<float>::infinity() && p.Y != -std::numeric_limits<float>::infinity())
				{
					int depthX = static_cast<int>(p.X + 0.5f);
					int depthY = static_cast<int>(p.Y + 0.5f);

					if ((depthX >= 0 && depthX < nDepthWidth) && (depthY >= 0 && depthY < nDepthHeight))
					{
						UINT16 pixel = pDepthBuffer[depthX + (depthY * nDepthWidth)];
						depthData[colorIndex] = pDepthBuffer[depthX + (depthY * nDepthWidth)];//pDepthBuffer[depthIndex];
					}
				}
				RGBQUAD rgbpixel = m_pColorRGBX[colorIndex];
				colorData[colorIndex] = cv::Vec3b(rgbpixel.rgbRed, rgbpixel.rgbGreen, rgbpixel.rgbBlue);
			}

			//**************************************************
			/*for (int depthIndex = 0; depthIndex < (nDepthWidth*nDepthHeight); ++depthIndex)
			{
			depthData[depthIndex] = pDepthBuffer[depthIndex];
			}*/
			//for (int colorIndex = 0; colorIndex < (nColorWidth*nColorHeight); ++colorIndex)
			//{
			//	//if (depthData[colorIndex]==0)
			//	//colorData[colorIndex] /= 3;
			//}
			//cv::resize(depth, depth, cv::Size(0, 0), 0.4, 0.4);
			//cv::resize(rgb, rgb, cv::Size(0, 0), 0.4, 0.4);
		}
	}
}



HRESULT KinectReader::processFrame(IMultiSourceFrame* pMultiSourceFrame, cv::Mat& depth, cv::Mat& rgb)
{

	IDepthFrame* pDepthFrame = NULL;
	IColorFrame* pColorFrame = NULL;


	HRESULT hr = 1;

	if (SUCCEEDED(hr))
	{
		IDepthFrameReference* pDepthFrameReference = NULL;

		hr = pMultiSourceFrame->get_DepthFrameReference(&pDepthFrameReference);
		if (SUCCEEDED(hr))
		{
			hr = pDepthFrameReference->AcquireFrame(&pDepthFrame);
		}

		SafeRelease(pDepthFrameReference);
	}

	if (SUCCEEDED(hr))
	{
		IColorFrameReference* pColorFrameReference = NULL;

		hr = pMultiSourceFrame->get_ColorFrameReference(&pColorFrameReference);
		if (SUCCEEDED(hr))
		{
			hr = pColorFrameReference->AcquireFrame(&pColorFrame);
		}

		SafeRelease(pColorFrameReference);
	}


	if (SUCCEEDED(hr))
	{
		IFrameDescription* pDepthFrameDescription = NULL;
		int nDepthWidth = 0;
		int nDepthHeight = 0;
		UINT nDepthBufferSize = 0;
		UINT16 *pDepthBuffer = NULL;

		IFrameDescription* pColorFrameDescription = NULL;
		int nColorWidth = 0;
		int nColorHeight = 0;
		ColorImageFormat imageFormat = ColorImageFormat_None;
		UINT nColorBufferSize = 0;
		RGBQUAD *pColorBuffer = NULL;

		// get depth frame data

		hr = pDepthFrame->get_RelativeTime(&timeStamp);

		if (SUCCEEDED(hr))
		{
			hr = pDepthFrame->get_FrameDescription(&pDepthFrameDescription);
		}

		if (SUCCEEDED(hr))
		{
			hr = pDepthFrameDescription->get_Width(&nDepthWidth);
		}

		if (SUCCEEDED(hr))
		{
			hr = pDepthFrameDescription->get_Height(&nDepthHeight);
		}

		if (SUCCEEDED(hr))
		{
			hr = pDepthFrame->AccessUnderlyingBuffer(&nDepthBufferSize, &pDepthBuffer);
		}

		// get color frame data

		if (SUCCEEDED(hr))
		{
			hr = pColorFrame->get_FrameDescription(&pColorFrameDescription);
		}

		if (SUCCEEDED(hr))
		{
			hr = pColorFrameDescription->get_Width(&nColorWidth);
		}

		if (SUCCEEDED(hr))
		{
			hr = pColorFrameDescription->get_Height(&nColorHeight);
		}

		if (SUCCEEDED(hr))
		{
			hr = pColorFrame->get_RawColorImageFormat(&imageFormat);
		}

		if (SUCCEEDED(hr))
		{
			if (imageFormat == ColorImageFormat_Bgra)
			{
				hr = pColorFrame->AccessRawUnderlyingBuffer(&nColorBufferSize, reinterpret_cast<BYTE**>(&pColorBuffer));
			}
			else if (m_pColorRGBX)
			{
				pColorBuffer = m_pColorRGBX;
				nColorBufferSize = cColorWidth * cColorHeight * sizeof(RGBQUAD);
				hr = pColorFrame->CopyConvertedFrameDataToArray(nColorBufferSize, reinterpret_cast<BYTE*>(pColorBuffer), ColorImageFormat_Rgba);
			}
			else
			{
				hr = E_FAIL;
			}
		}

		// get body index frame data



		if (SUCCEEDED(hr))
		{
			createFrame(pDepthBuffer, nDepthWidth, nDepthHeight,
				pColorBuffer, nColorWidth, nColorHeight,
				depth, rgb);
		}

		SafeRelease(pDepthFrameDescription);
		SafeRelease(pColorFrameDescription);
	}

	SafeRelease(pDepthFrame);
	SafeRelease(pColorFrame);
	SafeRelease(pMultiSourceFrame);

	return hr;
}



bool KinectReader::close()
{


	if (m_pColorRGBX)
	{
		delete[] m_pColorRGBX;
		m_pColorRGBX = NULL;
	}

	// done with frame reader
	SafeRelease(m_pMultiSourceFrameReader);

	// done with coordinate mapper
	SafeRelease(m_pCoordinateMapper);

	// close the Kinect Sensor
	if (m_pKinectSensor)
	{
		m_pKinectSensor->Close();
	}

	SafeRelease(m_pKinectSensor);
	return true;
}

