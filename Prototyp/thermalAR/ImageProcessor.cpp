#include "ImageProcessor.h"


// Initialisieren der LUT
void ImageProcessor::initializeLUT()
{
	// einzeilige Matrix mit der maximalen Anzahl an Werten anlegen
	// Hinweis: der negative Bereich der Kamera wird ignoriert
	lutBase = Mat(1, 12001, CV_16UC1);

	// initialisieren jedes Wertes um die LUT wie ein Bild verarbeiten zu können
	for (int i = 0; i < 12001; ++i) {
		ushort val = i;
		lutBase.at<ushort>(0, i) = val;
	}
	
}

// LUT mithilfe der entwickelten Algorithmen verarbeiten
void ImageProcessor::calcLut()
{
	double min, max;

	UMat detTemp, extrComb, mask, sharp, remap, test, alphaLow, alphaHigh, alpha;

	lutBase.copyTo(lookUpTable);

	UMat lut = lookUpTable.getUMat(cv::ACCESS_FAST);

	// Berechnen der Temperaturbereiche
	// die Temperaturbereiche wurden während der Initialisierung festgelegt 
	// die Bereiche müssen noch wieder im Bild integriert werden
	highlight.findTemp(lut, detTemp, alpha, true);

	// Berechnen der Temperaturextrema
	// wird in Mat low und high gespeichert
	extrema.getExtrema(lut, low, high, alphaLow, alphaHigh);


	// Grauwerttransformation zur Kontrastverbesserung
	// hier wird lediglich die einfache lineare Grauwerttransformation angewandt
	contrast.remap(lut, remap, visionRange, { 0, 65535 });
	

	///Integration der in detTemp hinterlegten und farblich hervorgehobenen Temperaturbereiche mittels Alpha Mapping///
	// Umwandeln der Alpha Map in ein dreikanaliges Bild
	cv::cvtColor(alpha, alpha, cv::COLOR_GRAY2BGR);

	// Konvertieren des Temperaturbereichs in 16 BIT um Clipping bei Multiplikation zu verhindern
	detTemp.convertTo(detTemp, CV_16UC3);

	// Multiplikation mit Alpha-Map
	cv::multiply(detTemp, alpha, detTemp, 1, CV_16UC3);

	// Invertieren der Alpha-Maske
	cv::subtract(cv::Scalar(255, 255, 255), alpha, alpha);

	// Transformieren von Kontrastverbesserten Bildes auf 8-Bit-Bereich um Clipping bei Multiplikation zu verhindern
	contrast.remap(remap, remap, { 0, 65535 }, { 0, 255 });

	// Konvertieren des kontrastverbesserten Bildes in 3-kanaliges Bild
	cv::cvtColor(remap, remap, cv::COLOR_GRAY2BGR);

	// Multiplikation mit invertierten Alpha-Map
	cv::multiply(remap, alpha, remap, 1, CV_16UC3);

	// Überlagern der hervorgehobenen Temperaturbereiche im kontrastverbesserten Bild
	cv::add(remap, detTemp, detTemp);

	// Rücktrnsformation auf 8-Bit-Bereich
	contrast.remap(detTemp, detTemp, { 0, 65535 }, { 0, 255 });
	///-----///

	/// Integration der Temperaturextrema ///
	// Kombinieren der Extrema
	add(high, low, extrComb);

	// die Alpha-Maps der Extrema kombinieren
	cv::add(alphaLow, alphaHigh, alpha);

	// Integrieren der Temperaturextrema mittels Alpha-Mapping
	// Analog des oben hervorgehobenen Temperaturbereiches
	cv::cvtColor(alpha, alpha, cv::COLOR_GRAY2BGR);
	extrComb.convertTo(extrComb, CV_16UC3);
	cv::multiply(extrComb, alpha, extrComb, 1, CV_16UC3);
	cv::subtract(cv::Scalar(255, 255, 255), alpha, alpha);
	cv::multiply(detTemp, alpha, detTemp, 1, CV_16UC3);
	cv::add(extrComb, detTemp, detTemp);
	///------///

	/// Speichern der neu berechneten LUT
	detTemp.copyTo(lookUpTable);
}



cv::UMat ImageProcessor::equalizeHist(cv::UMat mat)
{
	cv::UMat out;
	cv::equalizeHist(mat, out);
	return out;
}

// Überprüfen des Temperaturbereiches im Falle einer Änderung der Umgebungstemperatur
void ImageProcessor::checkRange(cv::UMat mat)
{
	// Initalie Berechnung des Temperaturbereiches für die Kontransverbesserung
	// und Berechnung der LUT
	if (!rangeSet) {
		ambTemp = ambient.calc(mat);
		visionRange = { (double)(ambTemp - 300), (double)(ambTemp + 300) };
		calcLut();
		rangeSet = true;
		counter++;
	}

	else {
		// Überprüfen der Umgebungstemperatur alle n Bilder
		if (counter > 9) {
			// Umgebungstemperatur berechnen
			int temp = ambient.calc(mat);
			counter = 0;

			// Falls die Änderung der Umgebungstemperatur größer als 0,5°C ist
			// setzte den Temperaturbereich neu und berechne die LUT
			if (std::abs(ambTemp - temp) > 50) {
				std::cout << "Recalc needed" << std::endl;
				std::cout << "New Ambient Temp: " << temp << std::endl;
				ambTemp = temp;
				visionRange = { (double)(ambTemp - 300), (double)(ambTemp + 300) };
				calcLut();
			}

		}

		counter++;
	}

}

// LUT auf Bilder src anwenden und Ergebnis in dst hinterlegen
void ImageProcessor::applyLut(cv::UMat src, cv::UMat& dst) {

	src.copyTo(dst);
	cv::Mat mat = src.getMat(ACCESS_FAST);
	cv::Mat newMat = cv::Mat::zeros(src.size(), CV_16UC3);

	// für Pointer Zugriff Elelmentanzahl berechnen
	int channels = src.channels();
	int nRows = src.rows;
	int nCols = src.cols * channels;
	if (src.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}
	int i, j;

	Vec3w* p;
	ushort* q;

	// Elementweise iterieren
	for (i = 0; i < nRows; ++i)
	{
		// Pointer für Bilder setzen
		p = newMat.ptr<Vec3w>(i);
		q = mat.ptr<ushort>(i);

		for (j = 0; j < nCols; ++j)
		{
			// neuen Pixelwert in Abhängigkeit des alten aus LUT lesen
			p[j] = lookUpTable.at<Vec3w>(0,q[j]);

		}
	}
	newMat.getUMat(ACCESS_FAST).copyTo(dst);
}

// Bild verarbeiten
void ImageProcessor::processImage(cv::UMat src)
{
	cv::UMat temps, out, edge, denoised, details, remap;

	// Temperaturbereich für Kontrastverbesserung prüfen
	checkRange(src);

	// Bild runterskalieren für effizientere Berechnung und besseren Überblick
	cv::pyrDown(src, src);
	
	temps = cv::UMat::zeros(src.size(), CV_8UC1);

	/// Kantenerkennung ///
	// Kontrast verbessern
	contrast.remap(src, remap, visionRange, { 0, 65535 });

	// Rauschreduzierung
	denoiser.reduceNoise(remap, denoised);

	// Kantenerkennung
	edgeDetector.detectEdges(denoised, edge);

	// Umwandlung des Kantenbildes in dreikanaliges Bild
	// Notwendig für Subtraktion mit ursprünglichen Bild 
	cv::cvtColor(edge, edge, cv::COLOR_GRAY2BGR);

	/// ------- ///


	// LUT anwenden
	applyLut(src, out);
	
	// Kantenverstärkung durch Subtraktion des Kantenbildes
	cv::addWeighted(out, 1, edge, -1, 0, details);

	// Konvertieren in 8-Bit Bild
	contrast.remap(details, details, { 0, 65535 }, { 0, 255 });
	details.convertTo(details, CV_8UC3);


	// Temperatur messen und Werte-Hud und Markierung in temps speichern
	reader.readTemp(src, temps);

	// temps für Integration in dreikanaliges Bild umwandeln
	cv::cvtColor(temps, temps, cv::COLOR_GRAY2BGR);
	
	// Integrieren der Messwerte und Markierung
	temps.copyTo(details, temps);

	// Anzeigen des verarbeiteten Bildes
	cv::imshow("Out", details);

}

// Initialisieren der Instanzen aller benötiger Klassen
ImageProcessor::ImageProcessor()
{
	edgeDetector = EdgeDetector();

	extrema = TempExtrema();
	extrema.setThreshold(1500, 3600, 4000);

	reader = TempReader();

	highlight = RangeHighlight();

	// Setzen der Grenzen für Hervorhebung des Temperaturbereiches
	highlight.setRange(
		{ 28, 36 },
		{ 25, 40 }
	);

	initializeLUT();

	contrast = ContrastManipulation();

	denoiser = NoiseReduction();

	ambient = AmbientTemp();

	// Setzen der Schwellwerte für Berechnung der Umgebungstemperatur
	ambient.setThreshold(1500, 4000);
}


ImageProcessor::~ImageProcessor()
{
}
