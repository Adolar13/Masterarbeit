#pragma once

#include <iostream>
#include <iomanip>
#include <opencv2/core.hpp>
#include "opencv2/imgproc.hpp"
#include <opencv2/highgui.hpp>

#include "DigitalTempConverter.h"

class TempReader
{
	cv::UMat mask, marker, markerMask;

	double correctTemp(
		double temp, 
		double emissivity,
		double ambientTemp
	);

public:
	void readTemp(
		cv::UMat src,
		cv::UMat & dst
	);
	void setMask(
		cv::UMat mask
	);
	void setMarker(
		cv::UMat marker,
		cv::UMat markerMask
	);

	TempReader();
	~TempReader();
};

