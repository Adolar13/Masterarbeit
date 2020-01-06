#pragma once
#include <opencv2/core.hpp>
#include "opencv2/imgproc.hpp"

#include "Helper.h"

#include <iostream>

class ContrastManipulation
{

public:
	void remap(
		cv::UMat src, 
		cv::UMat &dst, 
		range inputRange,
		range outputRange
	);

	void remap(
		cv::UMat src, 
		cv::UMat &dst, 
		range inputRange, 
		range outputRange, 
		range minMaxIn,
		range minMaxOut
	);

	double remapSingleValue(
		double in, 
		range inputRange, 
		range outputRange
	);

	void enhanceContrast(
		cv::UMat src, 
		cv::UMat & dst
	);

	ContrastManipulation();
	~ContrastManipulation();
};

