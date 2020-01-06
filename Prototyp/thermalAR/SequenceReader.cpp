#include "SequenceReader.h"

// Sequenz unter path einlesen
// alle Daten werden hierbei für eine schnellere Verarbeitung vorab geladen
void SequenceReader::readSequence(std::string path)
{
	std::vector<std::string> files;

	// über das Verzeichnis path iterieren und alle Dateinamen auslesen
	for (const auto & entry : fs::directory_iterator(path))
		files.push_back(entry.path().string());
	
	// für jede gefundene Datei das Bild einlesen und in die Queue legen
	for (int i = 0; i < files.size(); ++i) {
		cv::Mat mat = cv::imread(files[i], CV_16UC1);
		frameQueue.push(mat);
	}
	std::cout << frameQueue.size() << std::endl;
}

// Ein Bild für die Verarbeitung aus der Queue herausnehmen
cv::Mat SequenceReader::getFrame()
{
	if (!frameQueue.empty()) {
		out = frameQueue.front();
		frameQueue.pop();
		return out;
	}
	else {
		std::cout << "not popped\n";
		return cv::Mat();
	}
}

SequenceReader::SequenceReader()
{
}


SequenceReader::~SequenceReader()
{
}
