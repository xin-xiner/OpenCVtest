#pragma once

#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/core/utility.hpp>
//#include <opencv2/core/private.hpp>
//#include <opencv2/core/ocl.hpp>
//#include <opencv2/core/hal/hal.hpp>

#include <algorithm>

namespace cv
{

	void makeOffsets(int pixel[25], int row_stride, int patternSize);

	template<int patternSize>
	int cornerScore(const uchar* ptr, const int pixel[], int threshold);
	template<int patternSize>
	void FAST_my(InputArray _img, std::vector<KeyPoint>& keypoints, int threshold, bool nonmax_suppression)
	{
		Mat img = _img.getMat();
	
	
		const int K = patternSize / 2, N = patternSize + K + 1;

		int i, j, k, pixel[25];
		makeOffsets(pixel, (int)img.step, patternSize);

		keypoints.clear();

		threshold = std::min(std::max(threshold, 0), 255);

		uchar threshold_tab[512];
		for (i = -255; i <= 255; i++)
			threshold_tab[i + 255] = (uchar)(i < -threshold ? 1 : i > threshold ? 2 : 0);

		AutoBuffer<uchar> _buf((img.cols + 16) * 3 * (sizeof(int) + sizeof(uchar)) + 128);
		uchar* buf[3];
		buf[0] = _buf; buf[1] = buf[0] + img.cols; buf[2] = buf[1] + img.cols;
		int* cpbuf[3];
		cpbuf[0] = (int*)alignPtr(buf[2] + img.cols, sizeof(int)) + 1;
		cpbuf[1] = cpbuf[0] + img.cols + 1;
		cpbuf[2] = cpbuf[1] + img.cols + 1;
		memset(buf[0], 0, img.cols * 3);

		for (i = 3; i < img.rows - 2; i++)
		{
			const uchar* ptr = img.ptr<uchar>(i) +3;
			uchar* curr = buf[(i - 3) % 3];
			int* cornerpos = cpbuf[(i - 3) % 3];
			memset(curr, 0, img.cols);
			int ncorners = 0;

			if (i < img.rows - 3)
			{
				j = 3;

				for (; j < img.cols - 3; j++, ptr++)
				{
					int v = ptr[0];
					const uchar* tab = &threshold_tab[0] - v + 255;
					int d = tab[ptr[pixel[0]]] | tab[ptr[pixel[8]]];//At least one of the diagonal should be state 1 or 2

					if (d == 0)
						continue;

					d &= tab[ptr[pixel[2]]] | tab[ptr[pixel[10]]];
					d &= tab[ptr[pixel[4]]] | tab[ptr[pixel[12]]];
					d &= tab[ptr[pixel[6]]] | tab[ptr[pixel[14]]];

					if (d == 0)
						continue;

					d &= tab[ptr[pixel[1]]] | tab[ptr[pixel[9]]];
					d &= tab[ptr[pixel[3]]] | tab[ptr[pixel[11]]];
					d &= tab[ptr[pixel[5]]] | tab[ptr[pixel[13]]];
					d &= tab[ptr[pixel[7]]] | tab[ptr[pixel[15]]];

					if (d & 1)
					{
						int vt = v - threshold, count = 0;

						for (k = 0; k < N; k++)
						{
							int x = ptr[pixel[k]];
							if (x < vt)
							{
								if (++count > K)
								{
									cornerpos[ncorners++] = j;
									if (nonmax_suppression)
										curr[j] = (uchar)cornerScore<patternSize>(ptr, pixel, threshold);
									break;
								}
							}
							else
								count = 0;
						}
					}

					if (d & 2)
					{
						int vt = v + threshold, count = 0;

						for (k = 0; k < N; k++)
						{
							int x = ptr[pixel[k]];
							if (x > vt)
							{
								if (++count > K)
								{
									cornerpos[ncorners++] = j;
									if (nonmax_suppression)
										curr[j] = (uchar)cornerScore<patternSize>(ptr, pixel, threshold);
									break;
								}
							}
							else
								count = 0;
						}
					}
				}
			}

			cornerpos[-1] = ncorners;

			if (i == 3)
				continue;

			const uchar* prev = buf[(i - 4 + 3) % 3];
			const uchar* pprev = buf[(i - 5 + 3) % 3];
			cornerpos = cpbuf[(i - 4 + 3) % 3];
			ncorners = cornerpos[-1];

			for (k = 0; k < ncorners; k++)
			{
				j = cornerpos[k];
				int score = prev[j];
				if (!nonmax_suppression ||
					(score > prev[j + 1] && score > prev[j - 1] &&
					score > pprev[j - 1] && score > pprev[j] && score > pprev[j + 1] &&
					score > curr[j - 1] && score > curr[j] && score > curr[j + 1]))
				{
					keypoints.push_back(KeyPoint((float)j, (float)(i - 1), 7.f, -1, (float)score));
				}
			}
		}
	}
}