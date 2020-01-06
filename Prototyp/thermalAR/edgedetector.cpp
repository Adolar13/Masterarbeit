#include "edgedetector.h"
#include <iostream>

/// Funktion zur Kantenerkennung
// das Kantenbild wird in dst abgelegt
void EdgeDetector::detectEdges(UMat src, UMat & dst) {
	UMat grad, grad_x, grad_y;
	UMat abs_grad_x, abs_grad_y;

	double scale = 1;
	int delta = 0;
	int ddepth = CV_16S;

	/// Berechnung des Kantenbildes mithilfe des Scharr-Operators ///
	// Berechnung des Gradienten in X-Richtung
	Scharr(src, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT);

	// Bilden des absoluten Gradienten um negative Werte zu entfernen
	cv::absdiff(grad_x, Scalar::all(0), abs_grad_x);
	abs_grad_x.convertTo(abs_grad_x, CV_16UC1);

	// Berechnung des Gradienten in Y-Richtung
	Scharr(src, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT);

	// Bilden des absoluten Gradienten um negative Werte zu entfernen
	cv::absdiff(grad_y, Scalar::all(0), abs_grad_y);
	abs_grad_y.convertTo(abs_grad_y, CV_16UC1);
	/// ---- ///

	/// Verknüpfen beider Gradienten (Schätzung)
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst);
	
}

EdgeDetector::~EdgeDetector()
{
}
