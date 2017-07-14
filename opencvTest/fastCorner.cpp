#include "fastCorner.h"
#include <iostream>
namespace cv
{
	



	void makeOffsets(int pixel[25], int rowStride, int patternSize)
	{
		std::cout << rowStride << std::endl;
		static const int offsets16[][2] =
		{
			{ 0, 3 }, { 1, 3 }, { 2, 2 }, { 3, 1 }, { 3, 0 }, { 3, -1 }, { 2, -2 }, { 1, -3 },
			{ 0, -3 }, { -1, -3 }, { -2, -2 }, { -3, -1 }, { -3, 0 }, { -3, 1 }, { -2, 2 }, { -1, 3 }
		};

		static const int offsets12[][2] =
		{
			{ 0, 2 }, { 1, 2 }, { 2, 1 }, { 2, 0 }, { 2, -1 }, { 1, -2 },
			{ 0, -2 }, { -1, -2 }, { -2, -1 }, { -2, 0 }, { -2, 1 }, { -1, 2 }
		};

		static const int offsets8[][2] =
		{
			{ 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 },
			{ 0, -1 }, { -1, -1 }, { -1, 0 }, { -1, 1 }
		};

		const int(*offsets)[2] = patternSize == 16 ? offsets16 :
			patternSize == 12 ? offsets12 :
			patternSize == 8 ? offsets8 : 0;

		CV_Assert(pixel && offsets);

		int k = 0;
		for (; k < patternSize; k++)
			pixel[k] = offsets[k][0] + offsets[k][1] * rowStride;
		for (; k < 25; k++)
			pixel[k] = pixel[k - patternSize];
	}


	template<>
	int cornerScore<16>(const uchar* ptr, const int pixel[], int threshold)
	{
		const int K = 8, N = K * 3 + 1;
		int k, v = ptr[0];
		short d[N];
		for (k = 0; k < N; k++)
			d[k] = (short)(v - ptr[pixel[k]]);

		int a0 = threshold;
		for (k = 0; k < 16; k += 2)
		{
			int a = std::min((int)d[k + 1], (int)d[k + 2]);
			a = std::min(a, (int)d[k + 3]);
			if (a <= a0)
				continue;
			a = std::min(a, (int)d[k + 4]);
			a = std::min(a, (int)d[k + 5]);
			a = std::min(a, (int)d[k + 6]);
			a = std::min(a, (int)d[k + 7]);
			a = std::min(a, (int)d[k + 8]);
			a0 = std::max(a0, std::min(a, (int)d[k]));
			a0 = std::max(a0, std::min(a, (int)d[k + 9]));
		}

		int b0 = -a0;
		for (k = 0; k < 16; k += 2)
		{
			int b = std::max((int)d[k + 1], (int)d[k + 2]);
			b = std::max(b, (int)d[k + 3]);
			b = std::max(b, (int)d[k + 4]);
			b = std::max(b, (int)d[k + 5]);
			if (b >= b0)
				continue;
			b = std::max(b, (int)d[k + 6]);
			b = std::max(b, (int)d[k + 7]);
			b = std::max(b, (int)d[k + 8]);

			b0 = std::min(b0, std::max(b, (int)d[k]));
			b0 = std::min(b0, std::max(b, (int)d[k + 9]));
		}

		threshold = -b0 - 1;

#if VERIFY_CORNERS
		testCorner(ptr, pixel, K, N, threshold);
#endif
		return threshold;
	}

}