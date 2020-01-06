#include "ImageConverter.h"

// Konvertiert alle ASC Daten unter srcPath und speichert sie als PNG unter dstPath
void ImageConverter::ConvertAllAscToPng(string srcPath, string dstPath)
{
	string line;
	string val;
	int rows = 768;
	int cols = 1024;

	vector<string> files;

	// Dateinamen aller unter srcPath befindlicher ASC-Dateiern lesen
	for (const auto & entry : fs::directory_iterator(srcPath))
		files.push_back(entry.path().filename().string());
	
	// mittels INTEL TBB parallelisiertes Konvertieren aller gefunden Dateien
	tbb::parallel_for(tbb::blocked_range<int>(0, files.size()), 
		[&](const tbb::blocked_range <int> r) {
			for (size_t i = r.begin(); i < r.end(); ++i) {
				ConvertAscToPng(srcPath +"\\"+ files[i], dstPath + "\\" + files[i]);
			}
		});
		
	std::cout << "\nDone Converting Image.";
	return;
}

// Konvertiert eine einzelne ASC-Datei zu einem PNG 
void ImageConverter::ConvertAscToPng(std::string srcFile, std::string dstFile)
{
	string line;
	string val;
	int rows = 768;
	int cols = 1024;

	Mat out = Mat::zeros(rows, cols, CV_16UC1);

	// Filestream öffnen
	std::cout << srcFile << endl;
	std::cout << "Opening Filestream\n";
	ifstream fileStream(srcFile);

	// Puffer einlesen
	stringstream buffer;
	buffer << fileStream.rdbuf();

	// Datei Header überspringen
	while (getline(buffer, line)) {
		if (line == "[Data]")
			break;
	}

	int cnt = 0;

	// Daten Zeilen- und Spaltenweise einlesen und in ein Mat Objekt schreiben
	while (buffer >> val) {
		int temprow = cnt / cols;
		int tempcol = cnt % cols;
		out.at<short>(temprow, tempcol) = (stod(val) * 100);
		cnt++;
	}

	// Mat als PNG speichern
	imwrite(dstFile + ".png", out);
	
	std::cout << "\nDone Converting Image.";
	return;
}



ImageConverter::ImageConverter()
{
}


ImageConverter::~ImageConverter()
{
}
