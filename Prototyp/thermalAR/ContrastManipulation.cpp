#include "ContrastManipulation.h"


// lineare Grauwerttransformation
void ContrastManipulation::remap(cv::UMat src, cv::UMat &dst, range inputRange, range outputRange)
{
	cv::subtract(src, inputRange.low, dst);
	double factor = (outputRange.high - outputRange.low + 1) / (inputRange.high - inputRange.low + 1);
	cv::multiply(dst, factor, dst);
	cv::add(dst, outputRange.low, dst);
}

// Stückweise lineare Grauwerttransformation
void ContrastManipulation::remap(cv::UMat src, cv::UMat & dst, range inputRange, range outputRange, range minMaxIN, range minMaxOut)
{
	cv::UMat low, mid, high;
	cv::UMat maskLow, maskMid, maskHigh;

	/// Masken für die unterschiedlich zu behandelnden Bereiche erstellen
	// falls g < r_min
	cv::threshold(src, maskLow, inputRange.low - 1, 1, cv::THRESH_BINARY_INV);

	// falls r_min <= g <= r_max
	cv::threshold(src, maskMid, inputRange.low, 1, cv::THRESH_BINARY);
	cv::threshold(maskMid, maskMid, inputRange.high, 1, cv::THRESH_BINARY_INV);

	// falls g > r_max
	cv::threshold(src, maskHigh, inputRange.high + 1, 1, cv::THRESH_BINARY);


	/// Behandlung des unteren Bereiches
	cv::subtract(src, minMaxIN.low - 1, low);
	double factor = (outputRange.low) / (inputRange.low - minMaxIN.low + 1);
	cv::multiply(low, factor, low);


	/// Behandlung des relevanten Bereiches
	cv::subtract(src, inputRange.low, mid);
	factor = (outputRange.high - outputRange.low + 1) / (inputRange.high - inputRange.low + 1);
	cv::multiply(mid, factor, mid);
	cv::add(mid, outputRange.low, mid);

	/// Behandlung des oberen Bereiches
	cv::subtract(src, inputRange.high + 1, high);
	factor = (minMaxOut.high - outputRange.high) / (minMaxIN.high - inputRange.high);
	cv::multiply(high, factor, high);
	cv::add(high, outputRange.high + 1, high);

	/// Transformierte Bereiche mithilde der Masken kombinieren
	low.copyTo(dst, maskLow);
	mid.copyTo(dst, maskMid);
	high.copyTo(dst, maskHigh);
}

// einzelnen Wert aus inputRange nach outputRange transformieren
double ContrastManipulation::remapSingleValue(double in, range inputRange, range outputRange)
{
	double out = in - inputRange.low;
	double factor = (outputRange.high - outputRange.low) / (inputRange.high - inputRange.low);
	out *= factor;
	out += outputRange.low;

	// Clipping falls Wert außerhalb der Grenzen fällt
	if (out < outputRange.low)
		return outputRange.low;
	else if (out > outputRange.high)
		return outputRange.high;
	else
		return out;
}


// Kontrastverbesserung mittels CLAHE
void ContrastManipulation::enhanceContrast(cv::UMat src, cv::UMat & dst)
{
	cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
	clahe->setClipLimit(2*256);
	clahe->setTilesGridSize(cv::Size(10, 7));
	clahe->apply(src, dst);
}

ContrastManipulation::ContrastManipulation()
{
}


ContrastManipulation::~ContrastManipulation()
{
}
