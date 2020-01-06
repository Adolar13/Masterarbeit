#include <opencv2/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/dnn.hpp>

#include <iostream>

#include "edgedetector.h"
#include "ImageConverter.h"
#include "SequenceReader.h"
#include "ImageProcessor.h"

using namespace cv;
using namespace std;

ImageConverter converter;
SequenceReader seqReader;
ImageProcessor imgProc;

//Instanziert die benötigten Klassen
void initialize() {
	converter = ImageConverter();
	seqReader = SequenceReader();
	imgProc = ImageProcessor();
}

//Liest zunächst alle Bilddateien unter path ein und verarbeitet erst dann die Bilder
void PreloadAndPlaySequence(string path) {
	//Variablen
	Mat image;
	time_t start, end;

	//Sequenz unter path einlesen
	seqReader.readSequence(path);

	//Timer für FPS Counter starten
	time(&start);

	//Verarbeitungsloop
	for (;;) {

		//Nächsten Frame lesen und verarbeiten
		image = seqReader.getFrame();
		if (image.empty())
		{
			cout << "End of Sequence" << endl;
			break;
		}
		imgProc.processImage(image.getUMat(ACCESS_FAST));
		waitKey(1);
	}

	//FPS auswerten
	time(&end);
	double seconds = difftime(end, start);
	double fps = 601 / seconds;
	cout << "FPS: " << fps;
}

//Bilder der Sequenz unter path einzeln laden und verarbeiten
void PlaySequence(string path) {
	//Variablen
	time_t start, end;
	UMat image;

	//Sequenz lesen
	VideoCapture sequence(path);
	if (!sequence.isOpened())
	{
		cerr << "Failed to open Image Sequence!\n" << endl;
		return;
	}

	//Timer für FPS starten
	time(&start);

	//Verarbeitungsloop
	for (;;)
	{
		//nächstes Bild in der Sequenz lesen
		sequence.read(image);

		if (image.empty())
		{
			cout << "End of Sequence" << endl;
			break;
		}

		//gelesenes Bild verarbeiten
		imgProc.processImage(image);

		char key = (char)waitKey(1);
		if (key == 'q' || key == 'Q' || key == 27)
			break;
	}

	//FPS auswerten
	time(&end);
	double seconds = difftime(end, start);
	double fps = 601 / seconds;
	cout << "FPS: " << fps;
	

}

//Verarbeiten eines einzelnen Bildes unter path
void ShowSingleImage(string path) {
	//Variablen
	Mat src;
	UMat image;

	//Bild einlesen und UMat holen
	src = imread(path, CV_16UC1);
	image = src.getUMat(ACCESS_READ); // Read the file
	
	if (image.empty())
	{
		cout << "unable to read image" << endl;
		return;
	}
	
	//Bild verarbeiten
	imgProc.processImage(image);
	
	//Auf Eingabe zum Beenden warten
	waitKey(0);
}

int main(int argc, char** argv)
{
	///Initializieren
	initialize();

	///Funktionsaufrufe für die Verarbeitung von Sequenzen
	PlaySequence("img\\converted\\AA090504\\AA090504_%03d.asc.png");
	//PreloadAndPlaySequence("img\\converted\\AA090504\\");

	///Funktionsaufruf für die Verarbeitung von Einzelbildern
	//ShowSingleImage("img/Ergebnisse/png/AA090517.png");
	

	///Funktionsaufrufe zum Konvertieren der Bilder im von IRBIS 3 exportieren ASC Format
	//converter.ConvertAscToPng("img/Ergebnisse/input/AA090518(765).asc", "img/Ergebnisse/png/AA090518(765)");
	//converter.ConvertAllAscToPng("img/meeting/AA090518", "img/meeting/AA090518/converted");                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 	return 0;
 }