#pragma once

#include <opencv2/core.hpp>

#include <opencv2/photo.hpp>

#include <opencv2/highgui.hpp>


class NoiseReduction
{

public:
	void reduceNoise(
		cv::UMat src, 
		cv::UMat & dst
	);

	NoiseReduction();
	~NoiseReduction();
};

