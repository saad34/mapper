#include <iostream>
#include <string>
#include "m2.h"
#include "m3.h"
#include "m4.h"
#include <chrono>
#include <thread>
#include <cstdlib>
#include <vector>
#include "graphics.h"
#include "drawHelper.h"
#include "MapData.h"
#include "draw.h"
#include "StreetsDatabaseAPI.h"
#include <cfloat>

using namespace std;

bool draw_map(std::string map_path) {
    
    
    //loads map
    //set SW and NE
    bool mapLoaded = load_map(map_path); 
    
    if(mapLoaded) {
        classifyStreets();

        //parses the string of path name to obtain the name of the map
        string nameOfMap = parseNameOfMap(map_path);

        //gets world coordinates to draw.
        //the origin is the SW corner, the x and y max's are equal to the East and North components respectively
        t_point worldCoords = latLonToCoordinates(MapData::getNE());


        setup(nameOfMap, worldCoords);
    }

    close_map();
    
    return mapLoaded;
}

//parses the name of the map from the file path
/*this only works if the path name follows the following format
*map_path = "/____/____/.../nameofmap.bin"*/
string parseNameOfMap(string map_path) {
    string nameOfMap = map_path;
    string delimiter = "/";
    size_t pos = 0;
    string token;
    
    //loops through the file path string.
    //every time it finds a "/" it deletes the part of the string before it
    //should remove the whole path except the name of the .bin file (e.g. "mapName.bin")
    while ((pos = nameOfMap.find(delimiter)) != string::npos) {
        
        token = nameOfMap.substr(0, pos);
        nameOfMap.erase(0, pos + delimiter.length());
    }
    
    //removes the .bin part of the file name
    //all that should be left is the name of the map (if the .bin file is well named)
    delimiter = '.';
    pos = nameOfMap.find(delimiter);
    nameOfMap.erase(pos, string::npos);
        
    return nameOfMap;
}

//returns appropriate world coordinates from the map
/*this only works for maps that do not contain the international date line*/
t_point latLonToCoordinates(LatLon point) {
    
    //the origin is the SouthWest (SW) corner
    //find the number of km from origin the N and E component are
    //these will be our x and y coordinates
    LatLon SW = MapData::getSW();
    double avgLat = (point.lat + SW.lat)/2;
    LatLon N(point.lat, 0), s(SW.lat,0), E(avgLat, point.lon), W(avgLat, SW.lon); 
    double x = find_distance_between_two_points(W, E)*METRES_TO_KM;
    double y = find_distance_between_two_points(s, N)*METRES_TO_KM;
    return t_point(x,y);
}

void setup(string nameOfMap, t_point worldCoords) {

    // Set the name of the window (in UTF-8), with light grey background.
    init_graphics("Map of " + nameOfMap, t_color(233,229,220));

    // Set-up coordinates. The coordinates passed in define what coordinate
    // limits you want to use; this coordinate system is mapped to fill 
    // the screen.
    set_visible_world(0,0,worldCoords.x, worldCoords.y);

    // Enable mouse movement (not just button presses) and key board input.
    // The appropriate callbacks will be called by event_loop.
    set_keypress_input (true);
    set_mouse_move_input (true);
    
    //set POI clicked to false to begin with
    MapData::setPOIClicked(false);
    
    //set bool variables for buttons + drawn box to false
    MapData::setSpeedBool(false);
    MapData::setTravelBool(false);
    MapData::setPointBool(false);
    MapData::setBoxDrawnBool(false);
    MapData::setRightClickBool(false);
    
    //no travel time circles have been drawn yet
    MapData::setTravelTimeDrawnBool(false);
    
    //no directions or searching have been asked for yet
    MapData::setDirections(false);
    MapData::setSearchPOI(false);
    MapData::setSearchIntersections(false);
    MapData::setSearchStreets(false);
    MapData::setDrawMarkers(false);
    
    //set all intersections and t_points to invalid values
    MapData::setIntersectionID1(-1);
    MapData::setIntersectionID2(-1);
    t_point invalid(-1,-1);
    MapData::setFromPoint(invalid);
    MapData::setToPoint(invalid);
    MapData::setFromMarker(invalid);
    MapData::setToMarker(invalid);
    
    MapData::getDirectionPOI() == "";

    //buttons for 3 different modes: Speed Limit, Travel Time, and Points of Interest
    create_button("Window", "Speed Limit", speedLimit);
    create_button("Speed Limit", "Travel Time", travelTime);
    create_button("Travel Time", "POI", pointsOfInterest);
    
    /* Call event_loop so we get interactive graphics. Pass in all the 
       optional callbacks so we can take action on mouse buttons presses, 
       mouse movement, and keyboard key presses. */
    event_loop(act_on_button_press, act_on_mouse_move, act_on_key_press, drawscreen);

    close_graphics ();

}

void drawscreen (void) {

    /* The redrawing routine for still pictures.  The graphics package calls  
     * this routine to do redrawing after the user changes the window in any way. */
    clearscreen();

    for(int i = 0; i < (int) getFeatureCount(); i++) {
        
        parseFeature(i);
    }
    
    drawStreets(getStreetsToDraw());
    
    //note that this only draws if the POI bool is enabled
    for(int i = 0; i < (int) getNumberOfPointsOfInterest(); i++) {
        
        drawPointofInterest(i);    
    }
    
    update_message("Right Click for More Options");
     
    
//    //TESTS FOR MILESTONE 4 -> REMOVE AFTER
//    //--------------------------------------------
//    
//    //very hard nyc
//    std::vector<unsigned> delivery_ids = {2329, 12330, 18825, 19724, 21752, 21962, 22528, 24598, 31686, 33236, 39338, 42709, 43735, 47109, 47814, 56874, 78752, 86098, 100663, 112486, 112539, 118951, 122254, 126194, 126539, 127242, 127663, 130487, 139127, 139584, 147591, 148112, 149819, 152685, 156801, 158718, 160019, 161839, 163431, 171776};
//    std::vector<unsigned> depot_ids = {5000, 14178, 19173, 43253, 69775, 74531, 105070, 118483, 121711, 130296};

    
    //very hard
//    std::vector<unsigned> delivery_ids = {159, 6118, 11108, 13397, 15488, 16525, 17019, 19072, 22986, 24242, 24567, 25122, 27591, 32835, 40283, 47653, 51513, 52181, 53180, 60687, 63494, 68860, 74010, 75486, 76352, 82903, 87781, 99109, 99257, 100334, 106475, 138162, 143340, 144891, 150279, 151704, 152207, 153603, 161514, 164537};
//    std::vector<unsigned> depot_ids = {12487, 17593, 22983, 31837, 45118, 54428, 62587, 88803, 108746, 160177};
    
    //hard
    std::vector<unsigned> delivery_ids = {4949, 8301, 9982, 32228, 44379, 54653, 60373, 67343, 83152, 83319, 100371, 119340, 128421, 130889, 137022, 141546, 147785, 148648, 150125, 151922};
    std::vector<unsigned> depot_ids = {49617, 52651, 53760, 62640, 115286, 116829, 119059, 155370, 158803, 159662};
    
    //medium
//    std::vector<unsigned> delivery_ids = {1275, 14982, 20520, 26810, 45633, 50454, 74030, 97943, 160769, 161097};
//    std::vector<unsigned> depot_ids = {15149, 26707, 46831, 61511, 66987, 135189};
     
    //easy
//    std::vector<unsigned> delivery_ids = {1176, 4800, 43655, 66335, 121022};
//    std::vector<unsigned> depot_ids = {64314, 111573, 112996, 122728};
    
    //simple
//    std::vector<unsigned> delivery_ids = {26713, 111288, 159340};
//    std::vector<unsigned> depot_ids = {9085, 122230};
//    
    //trivial
//    std::vector<unsigned> delivery_ids = {41999, 103969};
//    std::vector<unsigned> depot_ids = {138722};

    //saint_helena
//    std::vector<unsigned> delivery_ids;
//    std::vector<unsigned> depot_ids;
//    delivery_ids = {350};
//    depot_ids = {472};

//    delivery_ids = {535, 29, 545, 39, 46, 49, 55};
//    depot_ids = {58, 572, 577,};
//
//    delivery_ids = {69, 70,};
//    depot_ids = {583, 588, 79, 81, 84, 88, 94};
    
//    drawCircles(depot_ids, 0);
//    drawCircles(delivery_ids, 1);
//
//    vector<unsigned> CIRCLES;
//    CIRCLES.push_back(47653);
//    setcolor(GREEN);
//    drawCircles(CIRCLES, 0);
//    
//    CIRCLES.clear();
//    setcolor(BLUE);
//    CIRCLES.push_back(144891);
//    drawCircles(CIRCLES, 0);
//    
//    CIRCLES.clear();
//    CIRCLES.push_back(51513);
//    CIRCLES.push_back(53180);
//    setcolor(RED);
//    drawCircles(CIRCLES, 0);
    
//    std::vector<unsigned> tour;
//    {
////            UNITTEST_TIME_CONSTRAINT(30000);
//            tour = traveling_salesman(delivery_ids, depot_ids);
//    }
////
//////    bool tour_check = tour_is_legal(delivery_ids, depot_ids, tour);
//////    CHECK(tour_is_legal == true);
////
////    if(true) {
//            double tour_cost = compute_path_travel_time(tour);
//            std::cout << "Test Cost: " << tour_cost << std::endl;
////    }
//    //--------------------------------------------
//
//    
    showPOIInfo();
    getDirections();
    parseMarker();
}

void speedLimit(void (*drawscreen) (void)) {

    //let the button work with a "toggle" effect
    if(MapData::getSpeedBool()) MapData::setSpeedBool(false);
    else MapData::setSpeedBool(true);
    
    vector<int> streets = getStreetsToDraw();
    drawStreets(streets); 
}

void travelTime(void (*drawscreen)(void)) {
    
    //let the button work with a "toggle" effect
    if(MapData::getTravelBool()) {
        
        MapData::setTravelTimeDrawnBool(false);
        MapData::setTravelBool(false);
        drawscreen();
    }
    else MapData::setTravelBool(true);
}

void pointsOfInterest(void (*drawscreen) (void)) {
    
    //let the button work with a "toggle" effect
    if(MapData::getPointBool()) {
        MapData::setPointBool(false);
        drawscreen();
    }
    else {
        MapData::setPointBool(true);
        
        for(int i = 0; i < (int)getNumberOfPointsOfInterest(); i++) {
            drawPointofInterest(i);
        }
        flushinput();
    }
    
}

void act_on_button_press (float x, float y, t_event_buttonPressed event) {

/* Called whenever event_loop gets a button press in the graphics 
   area.  Allows the user to do whatever he/she wants with button clicks. */
    if(event.button == 1) {
        
        if(MapData::getSearchPOI()) MapData::setSearchPOI(false);
        if(MapData::getSearchIntersections()) MapData::setSearchIntersections(false);
        if(MapData::getSearchStreets()) MapData::setSearchStreets(false);
        
        if(MapData::getDirections()) {
            
            //from point has not been changed from invalid, first point (from) must be set
            if(MapData::getFromPoint().x == -1) {
                
                bool regionEmpty = true;
                
                t_point coords(x,y);
                t_point from(x,y);
                t_bound_box region = MapData::getIntersectionRegion(coords);
                
                //must make it a large number to start with
                double distance = DBL_MAX;
                unsigned fromID;
                vector<unsigned> regionIntersections = MapData::getREGIONVector(region);
                
                for(auto iter = regionIntersections.begin(); iter != regionIntersections.end();
                        iter++) {
                    
                    LatLon position = getIntersectionPosition(*iter);
                    t_point comparisonPoint = latLonToCoordinates(position);
                    double comparisonDistance = sqrt(pow(abs(coords.x - comparisonPoint.x), 2) +
                                                pow(abs(coords.y - comparisonPoint.y), 2));
                    
                    if(comparisonDistance < distance) {
                        
                        regionEmpty = false;
                        distance = comparisonDistance;
                        from = comparisonPoint;
                        fromID = *iter;
                    }
                }

                //check if an intersection in another region is closer
                bool surroundingRegionsEmpty =  checkDistanceToBoundaries(distance, region, coords, from, fromID);

                if(regionEmpty && surroundingRegionsEmpty) {
                    
                    string message = "No intersection exists close to here, please choose again.";
                    drawText(message);
                    flushinput();
                }
                else{
                    
                    MapData::setFromIntersection(fromID);
                    MapData::setFromPoint(from); 
                    MapData::setFromMarker(from);
                    parseMarker();
                    flushinput();
                }
            }
            
            //second point (to) must be set
            else {

                bool regionEmpty = true;
                
                t_point coords(x,y);
                t_point to(x,y);
                t_bound_box region = MapData::getIntersectionRegion(coords);
                
                //must make it a large number to start with
                double distance = DBL_MAX;
                unsigned toID;
                vector<unsigned> regionIntersections = MapData::getREGIONVector(region);
                
                for(auto iter = regionIntersections.begin(); iter != regionIntersections.end();
                        iter++) {
                    
                    LatLon position = getIntersectionPosition(*iter);
                    t_point comparisonPoint = latLonToCoordinates(position);
                    double comparisonDistance = sqrt(pow(abs(coords.x - comparisonPoint.x), 2) +
                                                pow(abs(coords.y - comparisonPoint.y), 2));
                    
                    if(comparisonDistance < distance) {
                        
                        regionEmpty = false;
                        
                        distance = comparisonDistance;
                        to = comparisonPoint;
                        toID = *iter;
                    }
                }
                
                //check if an intersection in another region is closer
                bool surroundingRegionsEmpty = checkDistanceToBoundaries(distance, region, coords, to, toID);
                
                if(regionEmpty && surroundingRegionsEmpty){
                    cout<<"No intersection exists close to here, please choose again."<<endl;
                }
                else {
                
                    MapData::setToIntersection(toID);
                    MapData::setToPoint(to);
                    MapData::setToMarker(to);

                    //we can now find the path between the from and to points
                    t_point invalid(-1,-1);
                    calculateDirections();
                    MapData::setDrawMarkers(true);
                    MapData::setToPoint(invalid);
                    MapData::setFromPoint(invalid);
                    MapData::setDirections(false);
                    printDirections(MapData::getPath());
                    drawscreen();
                }
            }     
            
        }
        
        
        //check if the user clicked an option the box that pops up when you right click
        if(MapData::getRightClickBool()) {

            t_bound_box currentWorld = get_visible_world();
            float xOffset = currentWorld.get_width();
            float yOffset = currentWorld.get_height();
            float xclick = MapData::getXClick();
            float yclick = MapData::getYClick();
            
            
            if( (x < xclick) || (x > (xclick + xOffset/4)) ||
                (y > yclick)  || ( (y < (yclick - 5*yOffset/16) && MapData::getDrawMarkers()) || (y < (yclick - yOffset/4) && !MapData::getDrawMarkers()) ) ) {
                
                //click was not regarding the drawn box
                MapData::setBoxDrawnBool(false);
                MapData::setRightClickBool(false);
                drawscreen(); //reset screen to clear the option box
            }
 
            //now the click must be somewhere inside the box (check section by section)
            else if( (y < yclick) && (y > (yclick - yOffset/16)) ) {
                
                //Get Directions
                MapData::setBoxDrawnBool(false);
                MapData::setRightClickBool(false);
                MapData::setDirections(true);
                MapData::setDrawMarkers(false);
                t_point invalid(-1,-1);
                MapData::setFromMarker(invalid);
                MapData::setToMarker(invalid);
                drawscreen();
                getDirections();
            }
            
            else if( (y < (yclick - yOffset/16)) && (y > (yclick - yOffset/8)) ) {
                
                //Search POI
                if(MapData::getFromPoint().x != -1) MapData::setDirections(true);
                MapData::setBoxDrawnBool(false);
                MapData::setRightClickBool(false);
                MapData::setSearchPOI(true);
                MapData::setSearchIntersections(false);
                MapData::setSearchStreets(false);
                drawscreen();
                searchParser();        
            }        
            
            else if( (y < (yclick - yOffset/8)) && (y > (yclick - 3*yOffset/16)) ) {
                
                //Search Intersections
                MapData::setDirections(true);
                MapData::setBoxDrawnBool(false);
                MapData::setRightClickBool(false);
                MapData::setSearchIntersections(true);
                MapData::setSearchPOI(false);
                MapData::setSearchStreets(false);
                drawscreen();
                searchParser();
            }
            
            else if( (y < (yclick - 3*yOffset/16)) && (y > (yclick - yOffset/4)) ) {
                
                //Search Streets
                MapData::setBoxDrawnBool(false);
                MapData::setRightClickBool(false);
                MapData::setSearchStreets(true);
                MapData::setSearchPOI(false);
                MapData::setSearchIntersections(false);
                drawscreen();
                searchParser();
            }
            
            else if( (y < (yclick - yOffset/4)) && (y > (yclick - 5*yOffset/16)) ) {
                
                //clear directions
                t_point invalid(-1,-1);
                MapData::setBoxDrawnBool(false);
                MapData::setRightClickBool(false);
                MapData::setDrawMarkers(false);
                MapData::setFromMarker(invalid);
                MapData::setToMarker(invalid);
                drawscreen();
            }
            //the click was on a border if it was not dealt with (i.e. do nothing)      
        }
        
        
        //if POI Mode is enabled, we must deal with this accordingly
        if(MapData::getPointBool()) {
            string name;
            int id;
            if(clickedOnPOI(x,y, name, id)) {
                
                //draw previously clicked POI as unlicked
                if(MapData::getPOIClicked()) {
                    
                    MapData::setPOIClicked(false);
                    drawPointofInterest(MapData::getClickedPOIid());
                }
                
                //draw new POI as clicked
                MapData::setPOIClicked(true);
                MapData::setClickedPOIid(id);
                showPOIInfo();
                drawPointofInterest(MapData::getClickedPOIid());
            }
            
            else {
                
                if(MapData::getPOIClicked()) {
                    
                    MapData::setPOIClicked(false);
                    drawPointofInterest(MapData::getClickedPOIid());
                }
                
                MapData::setClickedPOIid(-1);
                showPOIInfo();
            }
            flushinput();
        }
        
        
        //if Travel Time Mode is enabled, we must deal with this accordingly
        if(MapData::getTravelBool()) {
            
            if(MapData::getTravelTimeDrawnBool()) {
               
                MapData::setTravelBool(false);
                drawscreen();
                MapData::setTravelBool(true);
            }
            else MapData::setTravelTimeDrawnBool(true);

            double walkRadius = WALK_SPEED*TIME_AWAY;
            double bikeRadius = BIKE_SPEED*TIME_AWAY;
            double carRadius = CAR_SPEED*TIME_AWAY;

            t_bound_box currentWorld = get_visible_world();
            t_point coords(x,y);
            
            //drawing rings
            setlinewidth(4);
            setcolor(7); //green
            drawarc(x, y, walkRadius, 0, 360);

            setcolor(6); //yellow
            drawarc(x, y, bikeRadius, 0, 360);

            setcolor(4); //red
            drawarc(x, y, carRadius, 0, 360);
    
            
            //drawing text
            setcolor(1); //black
            setfontsize(9);
            drawtext(x, y + walkRadius, "Distance by Walking", currentWorld.right(), currentWorld.top());
            setfontsize(10);
            drawtext(x, y + bikeRadius, "Distance by Biking", currentWorld.right(), currentWorld.top());
            setfontsize(12);
            drawtext(x, y + carRadius, "Distance by Driving", currentWorld.right(), currentWorld.top());

            flushinput();
        } 
   
    }
    
    
    //the user clicked the right mouse button to make an option box appear
    if(event.button == 3) {
        
        MapData::setDirections(false);
        setlinewidth(0);
             
        t_bound_box currentWorld = get_visible_world();
        float xOffset = currentWorld.get_width();
        float yOffset = currentWorld.get_height();
        
        MapData::setXClick(x);
        MapData::setYClick(y);
        MapData::setRightClickBool(true);
        
        if(MapData::getBoxDrawnBool()) drawscreen(); //clear screen from other boxes
        else MapData::setBoxDrawnBool(true);
        
        //draw a rectangle with options, similar to that of the Internet
        if(MapData::getDrawMarkers()) {

            setcolor(0);
            fillrect(x, y, x + xOffset / 4, y - 5 * yOffset / 16);
            setcolor(1);
            drawrect(x, y, x + xOffset / 4, y - 5 * yOffset / 16); //set a border
        }
        else {
            
            setcolor(0);
            fillrect(x, y, x + xOffset/4, y - yOffset/4);
            setcolor(1);
            drawrect(x, y, x + xOffset/4, y - yOffset/4); //set a border
        }
        
        //make separations between options
        drawline(x, y - yOffset/8, x + xOffset/4, y - yOffset/8);
        drawline(x, y - yOffset/16, x + xOffset/4, y - yOffset/16);
        drawline(x, y - 3*yOffset/16, x + xOffset/4, y - 3*yOffset/16);
        if(MapData::getDrawMarkers()) drawline(x, y - yOffset/4, x + xOffset/4, y - yOffset/4);
        
        //list options (bounds are large so as not to interfere with drawing)
        setfontsize(11);
        drawtext(x + xOffset/8, y - yOffset/32, "Get Directions", 100, 100);
        drawtext(x + xOffset/8, y - 3*yOffset/32, "Search POI", 100, 100);
        drawtext(x + xOffset/8, y - 5*yOffset/32, "Search Intersections", 100, 100);
        drawtext(x + xOffset/8, y - 7*yOffset/32, "Search Streets", 100, 100);
        if(MapData::getDrawMarkers()) drawtext(x + xOffset/8, y - 9*yOffset/32, "Clear Directions", 100, 100);
        
        
        if(MapData::getIntersectionID1() < 0); //do nothing
        else {
            
            flushinput();
            MapData::setIntersectionID1(-1);
            MapData::setIntersectionID2(-1);
        }
    }

}

void act_on_mouse_move (float x, float y) {
    
    /* Function to handle mouse move event. The current mouse position in the
       current world coordinate system (as defined in your call to init_world) 
       is returned. */
    
    if(MapData::getRightClickBool()) {
        
        t_bound_box currentWorld = get_visible_world();
        float xOffset = currentWorld.get_width();
        float yOffset = currentWorld.get_height();
        float xclick = MapData::getXClick();
        float yclick = MapData::getYClick();
    
        if(MapData::getDrawMarkers()) {
            setcolor(0);
            fillrect(xclick, yclick, xclick + xOffset/4, yclick - 5*yOffset/16);
            setcolor(1);
            drawrect(xclick, yclick, xclick + xOffset/4, yclick - 5*yOffset/16); //set a border
        }else {
            setcolor(0);
            fillrect(xclick, yclick, xclick + xOffset/4, yclick - yOffset/4);
            setcolor(1);
            drawrect(xclick, yclick, xclick + xOffset/4, yclick - yOffset/4); //set a border
        }
        
        
        /* The following code is to make it so that when the mouse hovers over any
           options in the right click option box, that box will be highlighted in blue. */
        
        //make separations between options
        drawline(xclick, yclick - yOffset/8, xclick + xOffset/4, yclick - yOffset/8);
        drawline(xclick, yclick - yOffset/16, xclick + xOffset/4, yclick - yOffset/16);
        drawline(xclick, yclick - 3*yOffset/16, xclick + xOffset/4, yclick - 3*yOffset/16);
        if(MapData::getDrawMarkers()) drawline(xclick, yclick - yOffset/4, xclick + xOffset/4, yclick - yOffset/4);
        
        //list options (bounds are large so as not to interfere with drawing)
        setfontsize(11);
        drawtext(xclick + xOffset / 8, yclick - yOffset / 32, "Get Directions", 100, 100);
        drawtext(xclick + xOffset / 8, yclick - 3 * yOffset / 32, "Search POI", 100, 100);
        drawtext(xclick + xOffset / 8, yclick - 5 * yOffset / 32, "Search Intersections", 100, 100);
        drawtext(xclick + xOffset / 8, yclick - 7 * yOffset / 32, "Search Streets", 100, 100);
        if (MapData::getDrawMarkers()) drawtext(xclick + xOffset / 8, yclick - 9 * yOffset / 32,
                "Clear Directions", 100, 100);


        if ((x < xclick) || (x > (xclick + xOffset / 4)) ||
                (y > yclick) || ((y < (yclick - 5 * yOffset / 16) &&
                MapData::getDrawMarkers()) || (y < (yclick - yOffset / 4) &&
                !MapData::getDrawMarkers()))); //do nothing (mouse moved outside option box)

        //now the click must be somewhere inside the box (check section by section)
        else if ((y < yclick) && (y > (yclick - yOffset / 16))) {

            //first box
            setcolor(BLUE);
            fillrect(xclick, yclick, xclick + xOffset / 4, yclick - yOffset / 16);
            setcolor(WHITE);
            drawtext(xclick + xOffset / 8, yclick - yOffset / 32, "Get Directions", 100, 100);
            setcolor(BLACK);
        } 
        
        else if ((y < (yclick - yOffset / 16)) && (y > (yclick - yOffset / 8))) {

            //second box
            setcolor(BLUE);
            fillrect(xclick, yclick - yOffset / 16, xclick + xOffset / 4, yclick - yOffset / 8);
            setcolor(WHITE);
            drawtext(xclick + xOffset / 8, yclick - 3 * yOffset / 32, "Search POI", 100, 100);
            setcolor(BLACK);

        }
        
        else if ((y < (yclick - yOffset / 8)) && (y > (yclick - 3 * yOffset / 16))) {

            //third box
            setcolor(BLUE);
            fillrect(xclick, yclick - yOffset / 8, xclick + xOffset / 4, yclick - 3 * yOffset / 16);
            setcolor(WHITE);
            drawtext(xclick + xOffset / 8, yclick - 5 * yOffset / 32, "Search Intersections", 100, 100);
            setcolor(BLACK);

        } 
        
        else if ((y < (yclick - 3 * yOffset / 16)) && (y > (yclick - yOffset / 4))) {

            //fourth box
            setcolor(BLUE);
            fillrect(xclick, yclick - 3 * yOffset / 16, xclick + xOffset / 4, yclick - yOffset / 4);
            setcolor(WHITE);
            drawtext(xclick + xOffset / 8, yclick - 7 * yOffset / 32, "Search Streets", 100, 100);
            setcolor(BLACK);

        } 
        
        else if ((y < (yclick - yOffset / 4)) && (y > (yclick - 5 * yOffset / 16))
                    && MapData::getDrawMarkers()) {

            //fifth box
            setcolor(BLUE);
            fillrect(xclick, yclick - yOffset / 4, xclick + xOffset / 4, yclick - 5 * yOffset / 16);
            setcolor(WHITE);
            drawtext(xclick + xOffset / 8, yclick - 9 * yOffset / 32, "Clear Directions", 100, 100);
            setcolor(BLACK);
        }

        //the click was on a border if it was not dealt with (i.e. do nothing)      
        flushinput();
    }
}

void act_on_key_press (char c) {
    
    // function to handle keyboard press event, the ASCII character is returned
   
    if(MapData::getSearchPOI() || MapData::getSearchIntersections() || MapData::getSearchStreets()) {

        t_bound_box currentWorld = get_visible_world();
        double xLeft = currentWorld.left() + INFO_BOX_OFFSET * currentWorld.get_width();
        double yTop = currentWorld.top() - INFO_BOX_OFFSET * currentWorld.get_height();
        double xRight = xLeft + BOX_TO_SCREEN_RATIO_X * currentWorld.get_width();
        double yBottom = yTop - BOX_TO_SCREEN_RATIO_Y * currentWorld.get_height();

        int action;

        action = searchOffset(currentWorld, xLeft, xRight, yTop, yBottom, c);

        //"-1" means that just a regular character was pressed
        if (action == -1) {

            MapData::getSearchCount() += 1;
            
            if (MapData::getSearchPOI()) MapData::getPOISearch() += c;
            else if (MapData::getSearchIntersections()) MapData::getIntersectionSearch() += c;
            else MapData::getStreetSearch() += c;

            search(currentWorld, xLeft, xRight, yTop, yBottom);

            t_bound_box textBound(xLeft, yBottom, xRight, yTop);
            string search;
            if (MapData::getSearchPOI()) search = MapData::getPOISearch();
            else if (MapData::getSearchIntersections()) search = MapData::getIntersectionSearch();
            else search = MapData::getStreetSearch();
            drawtext(t_point((xLeft + xRight) / 2,
                    (yTop + yBottom) / 2 - 2 * INFO_BOX_OFFSET * currentWorld.get_height()),
                    search, textBound);
        }        
        else if(action == 0); //do nothing (already dealt with)
        
        //action = 1, 2, 3, 4 or 5
        //these correspond to different options listed that the user wants
        else {
            
            unsigned ID;
            if(action == 1) ID = MapData::getOption(1);
            else if(action == 2) ID = MapData::getOption(2);
            else if(action == 3) ID = MapData::getOption(3);
            else if(action == 4) ID = MapData::getOption(4);
            else ID = MapData::getOption(5);


            if(MapData::getSearchPOI()) { 
                 
                LatLon position;
                if(ID < getNumberOfPointsOfInterest()) position = getPointOfInterestPosition(ID);
            	t_point location = latLonToCoordinates(position);

                
                if(MapData::getDirections() && (MapData::getFromMarker().x != -1)) {
                    
                    //we can now find the path between the from and to points
                    t_point invalid(-1,-1);
                    string poiName = getPointOfInterestName(ID);
                    calculateDirectionsPOI(poiName);
                    
                    vector<unsigned> pointOfInterestIDs = MapData::getPOIHT().find(poiName)->second;
    
                    //find path to all POIs of that name for now (need minimum travel time)
                    //maybe only check intersection's region later

                    StreetSegmentEnds lastSeg = getStreetSegmentEnds(MapData::getPath().back());
                    
                    
                    for(auto iter = pointOfInterestIDs.begin(); iter != pointOfInterestIDs.end(); iter++) {

                        unsigned currPOIID = *iter;
                        unsigned currIntID = (*(MapData::getPOIClosestIntersection().find(currPOIID))).second;
                        
                        if(currIntID == lastSeg.from || currIntID == lastSeg.to) {
                            location = MapData::getIntersectionInfo()[currIntID].xyCoord;
                        }
                    }
                    
                    MapData::setDrawMarkers(true);
                    MapData::setToMarker(location);
                    MapData::setToPoint(invalid);
                    MapData::setFromPoint(invalid);
                    MapData::setDirections(false);
                    printDirections(MapData::getPath());
                    drawscreen();
                }
                else {

                    parseMarker(location);
                }
                MapData::setSearchPOI(false);
            }

            else if(MapData::getSearchIntersections()) {

                LatLon position = getIntersectionPosition(ID);
                t_point location = latLonToCoordinates(position);
                
                if((MapData::getFromMarker().x == -1)) {
                    
                    MapData::setFromIntersection(ID);
                    MapData::setFromPoint(location); 
                    MapData::setFromMarker(location);
                    parseMarker();
                    flushinput();
                }
                else if(MapData::getToMarker().x == -1) {

                    MapData::setToIntersection(ID);
                    MapData::setToPoint(location);
                    MapData::setToMarker(location);

                    //we can now find the path between the from and to points
                    t_point invalid(-1,-1);
                    calculateDirections();
                    MapData::setDrawMarkers(true);
                    MapData::setToPoint(invalid);
                    MapData::setFromPoint(invalid);
                    MapData::setDirections(false);
                    printDirections(MapData::getPath());
                    drawscreen();
                }
                centerTheMap(location);
                MapData::setSearchIntersections(false);
            }

            else {

                LatLon position = getIntersectionPosition(ID);
            	t_point location = latLonToCoordinates(position);
            	parseMarker(location);
                MapData::setSearchStreets(false);
            }
        }
    }
}

//see if other regions need to be checked to find a closer intersection
bool checkDistanceToBoundaries (double& distance, t_bound_box region, 
                                    t_point coords, t_point& location, unsigned& intersectionID) {
    
    bool surroundingRegionEmpty = true;
    
    //for regions on the: left side, right side, top, and bottom
    double rightDistance = abs(coords.x - region.right());
    double leftDistance = abs(coords.x - region.left());
    double topDistance = abs(coords.y - region.top());
    double bottomDistance = abs(coords.y - region.bottom());
    
    //for regions at corners (e.g. up + left, down + right)
    double topRightDistance = sqrt(pow(abs(coords.x - region.right()), 2) +
                                pow(abs(coords.y - region.top()), 2));
    double topLeftDistance = sqrt(pow(abs(coords.x - region.left()), 2) +
                                pow(abs(coords.y - region.top()), 2));
    double bottomRightDistance = sqrt(pow(abs(coords.x - region.right()), 2) +
                                pow(abs(coords.y - region.bottom()), 2));
    double bottomLeftDistance = sqrt(pow(abs(coords.x - region.left()), 2) +
                                pow(abs(coords.y - region.bottom()), 2));
    
    LatLon NE = MapData::getNE();
    t_point NEPoint = latLonToCoordinates(NE);
 
    
    /*we always need to make sure we are not along an edge of a side we
      are trying to check, otherwise results will be incorrect*/
    
    //first check the 4 adjacent regions
    if((rightDistance < distance) && (region.right() < NEPoint.x)) {

        //we want the region to the right (i.e. region + 1)
        vector<unsigned> newRegion = MapData::getNewIntersectionRegion(region, 1);

        if(newRegion != MapData::getREGIONVector(region)) {    
            for (auto iter = newRegion.begin(); iter != newRegion.end(); iter++) {

                LatLon position = getIntersectionPosition(*iter);
                t_point comparisonPoint = latLonToCoordinates(position);
                double comparisonDistance = sqrt(pow(abs(coords.x - comparisonPoint.x), 2) +
                                                pow(abs(coords.y - comparisonPoint.y), 2));

                if (comparisonDistance < distance) {
                    
                    surroundingRegionEmpty = false;
                    
                    distance = comparisonDistance;
                    location = comparisonPoint;
                    intersectionID = *iter;
                }
            }   
        }
    }
    
    if((leftDistance < distance) && (region.left() > 0)) {
        
        //we want the region to the left (i.e. region - 1)
        vector<unsigned> newRegion = MapData::getNewIntersectionRegion(region, -1);
        
        if(newRegion != MapData::getREGIONVector(region)) {
            for (auto iter = newRegion.begin(); iter != newRegion.end(); iter++) {

                LatLon position = getIntersectionPosition(*iter);
                t_point comparisonPoint = latLonToCoordinates(position);
                double comparisonDistance = sqrt(pow(abs(coords.x - comparisonPoint.x), 2) +
                                                pow(abs(coords.y - comparisonPoint.y), 2));

                if (comparisonDistance < distance) {
                    
                    surroundingRegionEmpty = false;

                    distance = comparisonDistance;
                    location = comparisonPoint;
                    intersectionID = *iter;
                }
            }     
        }
    }
    
    if((topDistance < distance) && (region.top() < NEPoint.y)) {
        
        //we want the region above (i.e. region - 5) (rows of 5 regions)
        vector<unsigned> newRegion = MapData::getNewIntersectionRegion(region, -5);
        
        if(newRegion != MapData::getREGIONVector(region)) {
            for (auto iter = newRegion.begin(); iter != newRegion.end(); iter++) {

                LatLon position = getIntersectionPosition(*iter);
                t_point comparisonPoint = latLonToCoordinates(position);
                double comparisonDistance = sqrt(pow(abs(coords.x - comparisonPoint.x), 2) +
                                                pow(abs(coords.y - comparisonPoint.y), 2));

                if (comparisonDistance < distance) {
                    
                    surroundingRegionEmpty = false;
                    
                    distance = comparisonDistance;
                    location = comparisonPoint;
                    intersectionID = *iter;
                }
            } 
        }
    }
    
    if(bottomDistance < distance && (region.bottom() > 0)) {
        
        //we want the region below (i.e. region + 5) (rows of 5 regions)
        vector<unsigned> newRegion = MapData::getNewIntersectionRegion(region, 5);
        
        if(newRegion != MapData::getREGIONVector(region)) {
            for (auto iter = newRegion.begin(); iter != newRegion.end(); iter++) {

                LatLon position = getIntersectionPosition(*iter);
                t_point comparisonPoint = latLonToCoordinates(position);
                double comparisonDistance = sqrt(pow(abs(coords.x - comparisonPoint.x), 2) +
                                                pow(abs(coords.y - comparisonPoint.y), 2));

                if (comparisonDistance < distance) {
                    
                    surroundingRegionEmpty = false;

                    distance = comparisonDistance;
                    location = comparisonPoint;
                    intersectionID = *iter;
                }
            } 
        }
    }
    
    
    //now check the 4 corner regions
    if((topRightDistance < distance) && (region.right() < NEPoint.x)
            && (region.top() < NEPoint.y)) {
        
        //we want the region to the top right (i.e. region - 4)
        vector<unsigned> newRegion = MapData::getNewIntersectionRegion(region, -4);
        
        if(newRegion != MapData::getREGIONVector(region)) {
            for (auto iter = newRegion.begin(); iter != newRegion.end(); iter++) {

                LatLon position = getIntersectionPosition(*iter);
                t_point comparisonPoint = latLonToCoordinates(position);
                double comparisonDistance = sqrt(pow(abs(coords.x - comparisonPoint.x), 2) +
                                                pow(abs(coords.y - comparisonPoint.y), 2));

                if (comparisonDistance < distance) {
                    
                    surroundingRegionEmpty = false;

                    distance = comparisonDistance;
                    location = comparisonPoint;
                    intersectionID = *iter;
                }
            }     
        }
    }
    
    if((topLeftDistance < distance) && (region.left() > 0)
            && (region.top() < NEPoint.y)) {
        
        //we want the region to the top left (i.e. region - 6)
        vector<unsigned> newRegion = MapData::getNewIntersectionRegion(region, -6);
        
        if(newRegion != MapData::getREGIONVector(region)) {
            for (auto iter = newRegion.begin(); iter != newRegion.end(); iter++) {

                LatLon position = getIntersectionPosition(*iter);
                t_point comparisonPoint = latLonToCoordinates(position);
                double comparisonDistance = sqrt(pow(abs(coords.x - comparisonPoint.x), 2) +
                                                pow(abs(coords.y - comparisonPoint.y), 2));

                if (comparisonDistance < distance) {
                    
                    surroundingRegionEmpty = false;

                    distance = comparisonDistance;
                    location = comparisonPoint;
                    intersectionID = *iter;
                }
            }    
        }
    }
    
    if((bottomRightDistance < distance) && (region.right() < NEPoint.x)
            && (region.bottom() > 0)) {
        
        //we want the region to the bottom right (i.e. region + 6)
        vector<unsigned> newRegion = MapData::getNewIntersectionRegion(region, 6);
        
        if(newRegion != MapData::getREGIONVector(region)) {
            for (auto iter = newRegion.begin(); iter != newRegion.end(); iter++) {

                LatLon position = getIntersectionPosition(*iter);
                t_point comparisonPoint = latLonToCoordinates(position);
                double comparisonDistance = sqrt(pow(abs(coords.x - comparisonPoint.x), 2) +
                                                pow(abs(coords.y - comparisonPoint.y), 2));

                if (comparisonDistance < distance) {
                    
                    surroundingRegionEmpty = false;

                    distance = comparisonDistance;
                    location = comparisonPoint;
                    intersectionID = *iter;
                }
            }     
        }
    }
    
    if((bottomLeftDistance < distance) && (region.left() > 0)
            && (region.bottom() > 0)) {
        
        //we want the region to the bottom left (i.e. region + 4)
        vector<unsigned> newRegion = MapData::getNewIntersectionRegion(region, 4);
        
        if(newRegion != MapData::getREGIONVector(region)) {
            for (auto iter = newRegion.begin(); iter != newRegion.end(); iter++) {

                LatLon position = getIntersectionPosition(*iter);
                t_point comparisonPoint = latLonToCoordinates(position);
                double comparisonDistance = sqrt(pow(abs(coords.x - comparisonPoint.x), 2) +
                                                pow(abs(coords.y - comparisonPoint.y), 2));

                if (comparisonDistance < distance) {
                    
                    surroundingRegionEmpty = false;

                    distance = comparisonDistance;
                    location = comparisonPoint;
                    intersectionID = *iter;
                }
            }     
        }
    }
    
    return surroundingRegionEmpty;
}
