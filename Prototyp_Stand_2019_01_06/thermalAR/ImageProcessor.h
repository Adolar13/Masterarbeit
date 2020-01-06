#pragma once
#include<future>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>


#include <iostream>
#include <omp.h>

#include "edgedetector.h"
#include "TempExtrema.h"
#include "TempReader.h"
#include "RangeHighlight.h"
#include "ContrastManipulation.h"
#include "NoiseReduction.h"
#include "AmbientTemp.h"

class ImageProcessor
{
	unsigned int cores = std::thread::hardware_concurrency();
	cv::UMat high, low;
	EdgeDetector edgeDetector;
	TempExtrema extrema;
	TempReader reader;
	RangeHighlight highlight;
	ContrastManipulation contrast;
	NoiseReduction denoiser;

	AmbientTemp ambient;

	cv::Mat lookUpTable;
	cv::Mat lutBase;

	void checkRange(cv::UMat mat);

	range visionRange;
	bool rangeSet = false;

	int counter = 0;
	int ambTemp = 2300;

public:
	cv::UMat equalizeHist(
		cv::UMat mat
	);


	void processImage(
		cv::UMat mat
	);

	void initializeLUT();

	void calcLut();

	void applyLut(
		cv::UMat src, cv::UMat &dst
	);

	ImageProcessor();
	~ImageProcessor();
};

