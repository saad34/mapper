#ifndef USERINTERFACE_H
#define	USERINTERFACE_H

#include <string>
#include <sstream>
#include <iostream>
#include "graphics.h"
#include "MapData.h"
#include "draw.h"
#include <vector>

//same constants as for POI info display, to keep the box in the same spot
#define BOX_TO_SCREEN_RATIO_X 0.45
#define BOX_TO_SCREEN_RATIO_Y 0.085
#define INFO_BOX_OFFSET 0.01
#define SUGGESTION_BOX_OFFSET 0.06
#define TYPING_OFFSET 0.2


void getDirections();
void calculateDirections();
void searchParser();
void search(t_bound_box currentWorld, double xLeft, double xRight, double yTop, 
                            double yBottom);

//updates iterators' positions based on characters entered or if the user pressed backspace
/* returns 0 if user presses enter or backspace (no need to do anything else) or if
   " and " was found -> replace this with " & " */
//returns 1, 2, 3, 4, 5 respectively if the user wanted one of those options
//returns -1 for a regular search
int searchOffset(t_bound_box currentWorld, double xLeft, double xRight, double yTop, 
                    double yBottom, char c);

//centers the map around the specified xy coordinates
void centerTheMap(t_point coords);
void printDirections(std::vector<unsigned> path);
void calculateDirectionsPOI(std::string poiName);

#endif	/* USERINTERFACE_H */

