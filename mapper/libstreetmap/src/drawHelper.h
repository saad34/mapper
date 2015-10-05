#ifndef DRAWHELPER_H
#define	DRAWHELPER_H

#include <vector>

#define S  8
#define M  16
#define L  32
#define XL 50

#define STREET_TO_WORLD 0.75
//the lower the number, the higher the density
#define STREET_NAME_DENSITY .25

std::vector<int> getStreetsToDraw();
void classifyStreets();
double visibleWorldFunction();

/* Essentially parses the features, since some can be drawn in the same way,
   then calls the actual drawing function. */
void parseFeature(unsigned featureID);

//convert XY coordinates to LatLon
LatLon XYToLatLon(double x, double y);

//check if user has clicked on a POI
bool clickedOnPOI(double x, double y, std::string& name, int& id);

#endif	/* DRAWHELPER_H */

