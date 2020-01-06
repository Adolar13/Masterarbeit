#include "RangeHighlight.h"


// Hervorheben des spezifischen Temperaturbereiches
// die farbigen Bereiche werden in dstColor hinterlegt und
// die Alpha-Map für die Integration im ursprünglichen Bild 
// wird in dstAlpha hinterlegt
void RangeHighlight::findTemp(cv::UMat src, cv::UMat  & dstColor, cv::UMat & dstAlpha, bool extended)
{
	// erweiterten Bereich hervorheben
	if (extended && validExtended) {
		cv::inRange(src, extendedRange.low, extendedRange.high, extendedMask);
		cv::inRange(src, tempRange.high, extendedRange.high, tempMask);

		src.copyTo(extendedMat, extendedMask);
		
		src.copyTo(temp);
		cv::bitwise_not(temp,temp);

		contrast.remap(extendedMat, extendedMat, { extendedRange.low, tempRange.low }, {0, 255 });
		contrast.remap(temp, temp, {65535 - extendedRange.high,  65535 - tempRange.high }, { 0, 255 });
		temp.copyTo(extendedMat, tempMask);
		
		extendedMat.convertTo(extendedMat, CV_8U, 1);
		extendedMat.copyTo(dstAlpha, extendedMask);

		cv::cvtColor(extendedMask, extendedMat, cv::COLOR_GRAY2BGR);
		cv::multiply(extendedMat, cv::Scalar(1, 1, 0), extendedMat);
	}

	// relevanten Bereich hervorheben
	cv::inRange(src, tempRange.low, tempRange.high, mask);
	cv::inRange(src, rangeMid, extendedRange.high, tempMask);

	src.copyTo(rangeMat, mask);
	cv::bitwise_not(rangeMat, rangeMat);

	src.copyTo(temp);
	
	contrast.remap(rangeMat, rangeMat, { 65535 - rangeMid , 65535 - tempRange.low}, { 0, 255 });
	contrast.remap(temp, temp, {   rangeMid, tempRange.high }, { 0, 255 });

	temp.copyTo(rangeMat, tempMask);

	rangeMat.convertTo(rangeMat, CV_8U, 1);

	cv::cvtColor(rangeMat, rangeMat, cv::COLOR_GRAY2BGR);

	cv::multiply(rangeMat, cv::Scalar(1, 0, 0), rangeMat);
	cv::add(rangeMat, cv::Scalar(0, 255, 0), rangeMat);
	
	rangeMat.copyTo(extendedMat, mask);
	extendedMat.copyTo(dstColor);

}

// setzen der Grenzen des hervorzuhebenden Bereiches
void RangeHighlight::setRange(range tempR, range extendedR)
{
	RangeHighlight::tempRange.low = tempToDigital(tempR.low);
	RangeHighlight::tempRange.high = tempToDigital(tempR.high);
	RangeHighlight::rangeMid = tempRange.low + ((tempRange.high - tempRange.low)) / 2;
	if (extendedR.low < extendedR.high && (extendedR.low < tempR.low || extendedR.high > tempR.high)) {
		validExtended = true;
		RangeHighlight::extendedRange.low = tempToDigital(extendedR.low);
		RangeHighlight::extendedRange.high = tempToDigital(extendedR.high);
	}
	else {
		validExtended = false;
	}
	
}

RangeHighlight::RangeHighlight()
{
	contrast = ContrastManipulation();
	setRange({ 28, 36 }, { 25, 40 });
}


RangeHighlight::~RangeHighlight()
{
}
