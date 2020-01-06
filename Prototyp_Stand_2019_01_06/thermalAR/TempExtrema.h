#pragma once

#include "ContrastManipulation.h"

#include <opencv2/core.hpp>
#include "opencv2/imgproc.hpp"


//temporary includes
#include <opencv2/highgui.hpp>

class TempExtrema
{
	int lowThresh;
	int highThresh;
	int highCritThresh;

	double min, max;

	cv::Mat lut;
	cv::UMat temp, tempCrit, critMask;

	ContrastManipulation contrast;

	void getLowExtremes(
		cv::UMat & src, 
		cv::UMat &low, 
		cv::UMat &alpha
	);

	void getHighExtremes(
		cv::UMat & src, 
		cv::UMat &high, 
		cv::UMat &alpha
	);

public:

	void getExtrema(
		cv::UMat src,
		cv::UMat &low, 
		cv::UMat &high, 
		cv::UMat &alphaLow, 
		cv::UMat &alphaHigh
	);

	void setThreshold(
		int lowThresh, 
		int highThresh, 
		int hightCritThresh
	);

	TempExtrema();
	~TempExtrema();
};

