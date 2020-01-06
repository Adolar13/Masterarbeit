#include "TempExtrema.h"

// Berechnen der Minima
void TempExtrema::getLowExtremes(cv::UMat & src, cv::UMat &low, cv::UMat &alpha) {

	contrast.remap(src, alpha, { 0, (double)lowThresh }, { 0, 255 });

	alpha.convertTo(alpha, CV_8UC1);
	
	cv::bitwise_not(alpha, alpha);

	// Einfärben des erkannten Bereiches
	low.setTo(cv::Scalar(255,0,0), alpha);
	
}

// Berechnen der Maxima
void TempExtrema::getHighExtremes(cv::UMat & src, cv::UMat & high, cv::UMat &alpha) {

	contrast.remap(src, alpha, { (double)highThresh, (double)highCritThresh }, { 0, 255 });
	contrast.remap(src, critMask, { (double)highCritThresh, 6000 }, { 0, 254 });
	alpha.convertTo(alpha, CV_8UC1, 1);
	critMask.convertTo(critMask, CV_8UC1, 1);

	high.setTo(cv::Scalar(255), alpha);
	
	cv::cvtColor(critMask, tempCrit, cv::COLOR_GRAY2BGR);

	cv::multiply(tempCrit, cv::Scalar(0, 0, 1), tempCrit);
	cv::subtract(cv::Scalar(255), critMask, critMask);
	cv::multiply(high, critMask, temp,1, CV_16UC1);

	contrast.remap(temp, high, {0, 65535 }, { 0, 255 });
	high.convertTo(high, CV_8UC1, 1);
	cv::cvtColor(high, high, cv::COLOR_GRAY2BGR);
	
	cv::multiply(cv::Scalar(0, 1, 1), high, high);
	
	cv::add(high, tempCrit, high);

}

// Berechnen der Extremwerte
// die farbigen Bereich werden in low und high gespeichert und die Alpha-Maps
// für die Integration im ursprünglichen Bild in alphaLow und alphaHigh
void TempExtrema::getExtrema(cv::UMat src, cv::UMat &low, cv::UMat &high, cv::UMat &alphaLow, cv::UMat &alphaHigh)
{
	cv::minMaxLoc(src, &min, &max);

	alphaHigh = cv::UMat::zeros(src.size(), CV_8UC1);
	alphaLow = cv::UMat::zeros(src.size(), CV_8UC1);
	low = cv::UMat::zeros(src.size(), CV_8UC3);
	high = cv::UMat::zeros(src.size(), CV_8UC1);

	if (min < lowThresh) {
		getLowExtremes(src, low, alphaLow);
	}

	if (max > highThresh) {
		getHighExtremes(src, high, alphaHigh);
	}
	else {
		high = cv::UMat::zeros(src.size(), CV_8UC3);
	}

}

// Setzen der Schwellwerte für Hervorhebung der Extrema
void TempExtrema::setThreshold(int lowThresh, int highThresh, int highCritThresh)
{
	TempExtrema::lowThresh = lowThresh;
	TempExtrema::highThresh = highThresh;
	TempExtrema::highCritThresh = highCritThresh;
}



TempExtrema::TempExtrema()
{
	contrast = ContrastManipulation();
}




TempExtrema::~TempExtrema()
{
}
