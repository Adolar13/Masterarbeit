#include "AmbientTemp.h"

//Umgebungstemperatur aus Bild src berechnen
int AmbientTemp::calc(cv::UMat src)
{
	cv::UMat reduced;
	
	// Limitieren der extremen Werte im Bild auf threshMax und threshMin
	cv::min(src, threshMax, reduced );
	cv::max(reduced, threshMin, reduced);

	//Mittelwert des Bildes berechnen und runden
	temp = cv::mean(reduced).val[0]+0.5;

	return temp;
}

//Funktion zum Abfragen der momentanen Umgebungstemperatur
int AmbientTemp::getTemp()
{
	return temp;
}

//Setzen der Schwellwerte für die Limitierung
void AmbientTemp::setThreshold(int min, int max)
{
	threshMin = min;
	threshMax = max;
}

AmbientTemp::AmbientTemp()
{
	//Schwellwert auf Messbereichsgrenzen der Kamera als Standardwerte setzen
	setThreshold(0, 12000);
}


AmbientTemp::~AmbientTemp()
{
}
