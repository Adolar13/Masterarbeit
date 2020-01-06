#include "TempReader.h"


// Korrektur der Temperatur in Abhängigkeit des Emissionsgrades und der Umgebungstemperatur
double TempReader::correctTemp(double temp, double emissivity, double ambientTemp)
{
	// Umwandlung in Kelvin
	double t = temp + 273.15;
	double amb = ambientTemp + 273.15;

	// Korrektur
	double corr = std::pow((std::pow(t, 4) - (1.0 - emissivity)*std::pow(amb, 4)) / emissivity, 1.0 / 4.0) - 273.15;

	return corr;
}

// Messen der Temperatur in der Bildmitte und hinterlegen der Messwerte und der Markierung in dst
void TempReader::readTemp(cv::UMat src, cv::UMat & dst)
{
	// Messbereich definieren
	cv::Rect rect(src.cols/2-5, src.rows/2-5, 10, 10);
	cv::UMat roi = src(rect);
	
	// Mittelwert des Messfeldes bilden
	// das Feld wird durch mask verfeinert
	double t = digitalToTemp(cv::mean(roi, mask)[0]);

	// Emissionsgradkorrigierte Temperatur
	double tCorr = correctTemp(t, 0.05, 22.0);

	// Zusammensetzen der Strings zur Anzeige
	std::ostringstream sstream;
	sstream << std::fixed;
	sstream << std::setprecision(1);
	sstream << t;
	std::string temp = sstream.str();
	sstream.str("");
	sstream.clear();
	sstream << "(";
	sstream << tCorr;
	sstream << ")";
	std::string tempCorrected = sstream.str();

	// setzen des Markers 
	cv::Rect center = cv::Rect(src.cols / 2 - marker.cols / 2, src.rows / 2 - marker.rows / 2, marker.cols, marker.rows);
	marker.copyTo(dst(center), markerMask);

	// Platzierung der gemessenen und berechneten Werte
	cv::Size textSize = cv::getTextSize(temp, 1,3, 5, 0);

	cv::Point textOrg((dst.cols - textSize.width),(textSize.height+10));
	cv::putText(dst, temp, textOrg, 1, 3, cv::Scalar(1, 1, 1), 5);
	cv::putText(dst, temp, textOrg, 1, 3, cv::Scalar(254, 254, 254), 2);

	cv::Point corrOrg(textOrg.x, textOrg.y + 25);
	cv::putText(dst, tempCorrected, corrOrg, 1, 1.5, cv::Scalar(1, 1, 1), 5);
	cv::putText(dst, tempCorrected, corrOrg, 1, 1.5, cv::Scalar(254, 254, 254), 2);

}

// Setzen der Maske zur Verfeinerung des Messbereiches
// wird auf die Standardmaske gesetzte falls mask nichr 10x10 Pixel groß ist
void TempReader::setMask(cv::UMat mask)
{
	if (mask.cols != 10 && mask.rows != 10) {
		bool data[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
			0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
			0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
			0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		cv::Mat temp = cv::Mat(10, 10, CV_8U, data);
		temp.copyTo(TempReader::mask);
	}
	else {
		TempReader::mask = mask;
	}

	
}

// festlegen der in der Mitte zu platzierenden Markierung
void TempReader::setMarker(cv::UMat marker, cv::UMat markerMask)
{
	TempReader::marker = marker;
	TempReader::markerMask = markerMask;
}

TempReader::TempReader()
{
	bool data[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
			0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
			0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
			0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
			0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	cv::Mat temp = cv::Mat(10, 10, CV_8U, data);
	temp.copyTo(TempReader::mask);

	// laden der Markierung under Verfeinerungsmaske
	cv::imread("data/cross_hair_ring.png", CV_8UC1).copyTo(TempReader::marker);
	cv::imread("data/cross_hair_ring_mask.png", CV_8UC1).copyTo(TempReader::markerMask);
}


TempReader::~TempReader()
{
}
