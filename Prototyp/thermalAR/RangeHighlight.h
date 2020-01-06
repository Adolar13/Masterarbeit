#pragma once

#include "DigitalTempConverter.h"
#include "Helper.h"
#include "ContrastManipulation.h"

#include <opencv2/core.hpp>
#include "opencv2/imgproc.hpp"

#include <opencv2/highgui.hpp>

class RangeHighlight
{
	ContrastManipulation contrast;

	range tempRange;
	range extendedRange;
	double rangeMid;

	bool validExtended = false;

	cv::UMat rangeMat, extendedMat, mask, extendedMask;
	cv::UMat temp, tempMask;

public:
	void findTemp(
		cv::UMat src,
		cv::UMat & dstColor,
		cv::UMat & dstAlpha,
		bool extended
	);

	void setRange(
		range tempRange,
		range extendedRange
	);



	RangeHighlight();
	~RangeHighlight();
};

