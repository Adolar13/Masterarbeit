#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include <math.h>


class AmbientTemp
{
	int temp = 0;
	int threshMin = 0;
	int threshMax = 0;

public:

	int calc(
		cv::UMat src
	);

	int getTemp();

	void setThreshold(
		int min, 
		int max
	);

	AmbientTemp();
	~AmbientTemp();
};

