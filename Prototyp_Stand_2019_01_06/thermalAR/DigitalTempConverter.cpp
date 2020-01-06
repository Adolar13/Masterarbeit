#include "DigitalTempConverter.h"


// Auf die in der Arbeit beschriebene Verschiebung um negative Werte darzustellen
// wurde hier bewusst verzichtet, da bei den Aufnahmen keine negativen Werte
// auftraten und angenommen wird, dass diese auch im Feuerwehreinsatz nicht
// von Belangen sind

// Umwandeln von Temperaturwerten in Intensitätswerte
unsigned short tempToDigital(double temp) {
	return (unsigned short)((temp * 100)+0.5);
}

// Umwandeln von Intensitätswerten in Temperaturwerte
double digitalToTemp(int digital)
{
	return ((double)digital)/100;
}
