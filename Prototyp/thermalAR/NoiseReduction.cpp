#include "NoiseReduction.h"



// Rauschreduzierung
void NoiseReduction::reduceNoise(cv::UMat src, cv::UMat & dst)
{
	// Anwenden des NLM-Algorithmus
	cv::fastNlMeansDenoising(src, dst, { 3*256 }, 7, 21, cv::NORM_L1);
}

NoiseReduction::NoiseReduction()
{
}


NoiseReduction::~NoiseReduction()
{
}
