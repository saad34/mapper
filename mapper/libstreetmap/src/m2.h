#pragma once
#include <string>
#include "graphics.h"
#include "LatLon.h"
#include "m1.h"
#include "m3.h"
#include "UserInterface.h"
#include <sstream>


//all are in km/h
#define WALK_SPEED 4.3
#define BIKE_SPEED 15.5
#define CAR_SPEED 50

//5 minutes = 0.083333333333 hours
#define TIME_AWAY 0.083333333333

// Draws the map whose at map_path; this should be a .bin file.
bool draw_map(std::string map_path);

std::string parseNameOfMap(std::string map_path);

t_point latLonToCoordinates(LatLon point);

void setup(std::string nameOfMap, t_point worldCoords);

void drawscreen (void);

void act_on_new_button_func (void (*drawscreen_ptr) (void));

void act_on_button_press (float x, float y, t_event_buttonPressed event);

void act_on_mouse_move (float x, float y);

void act_on_key_press (char c);

void speedLimit (void (*drawscreen) (void));

void travelTime (void (*drawscreen) (void));

void pointsOfInterest (void (*drawscreen) (void));

bool checkDistanceToBoundaries (double& distance, t_bound_box region, 
                                    t_point coords, t_point& from, unsigned& intersectionID);
