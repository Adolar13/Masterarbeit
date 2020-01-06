#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <fstream>
#include <iostream>
#include <filesystem>

#include "tbb/parallel_for.h"

using namespace cv;
using namespace std;

namespace fs = std::filesystem;
class ImageConverter
{
public:
	void ConvertAllAscToPng(
		string srcPath, 
		string dstPath
	);

	void ConvertAscToPng(
		string srcFile, 
		string dstFile
	);

	ImageConverter();
	~ImageConverter();

private:
	Mat out;
};

