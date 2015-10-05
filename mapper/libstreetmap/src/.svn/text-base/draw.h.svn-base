#pragma once
#include "graphics.h"
#include "m2.h"
#include "MapData.h"
#include <vector>

//arbitrarily small number
#define SMALL_VALUE 0.000000001

//Points of Interest constants
#define POI_MAX_WORLD_SIZE 5

//constants for proportionality to currentWorld
#define POI_OUT_CIR_RAD 0.008
#define POI_IN_CIR_RAD 0.006
#define POI_TEXT "P"
#define TREE_CIRCLE_SIZE 0.004

//street constants
#define STREET_HALF_WIDTH_RATIO 0.004
#define STREET_NAME_GRID_DIM 5
#define STREET_NUMBER_OF_NAMES 3
#define STREET_FONT_SIZE 9

//constants to define where the streets line up
#define NEITHER 0
#define WIDTH 1
#define HEIGHT 2
#define BOTH 3

//POI info box constants
#define BOX_TO_SCREEN_RATIO_X 0.45
#define BOX_TO_SCREEN_RATIO_Y 0.085
#define INFO_BOX_OFFSET 0.01

//token/marker constants
#define MARKER_OFFSET 0.025
#define OUTER_CIRCLE_RADIUS 0.013
#define INNER_CIRCLE_RADIUS 0.005

//draw all streets in the given vector
void drawStreets(std::vector<int> streets);
std::pair<std::pair<int, t_point>, double> drawStreetSegment(int segmentID, int writeName);


void drawFeature(unsigned featureID, std::string attribute);

void drawPointofInterest(unsigned pointOfInterestID); 

void showPOIInfo();

void drawPathFinding(int segmentID, int color);

void drawPath(std::vector<unsigned> segments);


//checks to see if various tokens need to be drawn based on what the user wants
//note that the overloaded function "parseMarker" also centers the map
void parseMarker();
void parseMarker(t_point searchLocation);
void drawMarker(t_point location);
void drawText(std::string message);

//purely m4 debug
void drawCircles(std::vector<unsigned> circles, int kind);
