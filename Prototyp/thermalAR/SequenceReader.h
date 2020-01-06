 #pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <filesystem>
#include <iostream>

#include <queue>

namespace fs = std::filesystem;

class SequenceReader
{

public:
	void readSequence(
		std::string path
	);

	cv::Mat getFrame();

	cv::Mat out;
	
	
	std::queue<cv::Mat> frameQueue;
	SequenceReader();
	~SequenceReader();
};

