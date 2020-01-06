#pragma once
#include <queue>

#include <opencv2/core.hpp>
#include "opencv2/imgproc.hpp"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <opencv2/photo.hpp>


using namespace cv;

class EdgeDetector
{
public:

	void detectEdges(
		UMat src, 
		UMat & dst
	);


	~EdgeDetector();
};

