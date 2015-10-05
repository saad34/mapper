#ifndef MAPDATA_H
#define	MAPDATA_H
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "LatLon.h"
#include "graphics.h"
#include "m2.h"

//Stores variables necessary in multiple locations
class MapData {
    
//structs for map data
public:
    typedef struct intersection {
        
        bool drawn;
        std::vector<unsigned> outIntersections;
        t_point xyCoord;

    } Intersection;

    typedef struct street {
        
        std::vector<unsigned> streetSegments;
        //double streetWidth;
        t_bound_box streetBounds;
    } Street;
    
    typedef struct streetSegment {
        
        double speedLimit;
        std::vector<t_point> curvePointXYCoords;
    } StreetSegment;

    //struct to store the XY coordinates of feature points
    typedef struct feature {
        std::vector<double> x;
        std::vector<double> y;
    } Feature;

    //struct to store the XY coordinates of points of interest
    typedef struct interestPoint {
        
        double x, y;
    } InterestPoint;
    
    //graph is divided into 25 regions (5 wide and 5 long) (numbered 0-24)
    //intersections belonging to each region are locating (in order) at the respective index
    //intersections on borders of regions will be included in multiple regions
    typedef struct intersectionRegion {
        
        std::vector<unsigned> regions;
    } IntersectionRegions;

//member variables 
private:
    
    static std::vector<unsigned> path;
    static bool drawMarkers;
    static t_point fromMarker, toMarker;
    //pointer to a vector that contains all "out" adjacent intersections
    static Intersection *intersectionInfo;

    //pointer to a vector that contains all adjacent street segments
    static Street *streetInfo;
    
    //pointer to an array of segment structs that contain curve point xy coordinates
    static StreetSegment *segmentInfo;
    
    //pointer to 2 vectors that contain x and y coordinates (respectively) of feature points
    static Feature *featureXYPoints;
    
    //pointer to x and y coordinates of each point of interest
    static InterestPoint *interestXYPoints;
    
    //pointer to a vector of vectors containing intersections in various regions that follow
    static IntersectionRegions *intersectionRegions;
    static t_bound_box REGION0, REGION1, REGION2, REGION3, REGION4,
                        REGION5, REGION6, REGION7, REGION8, REGION9, 
                        REGION10, REGION11, REGION12, REGION13, REGION14,
                        REGION15, REGION16, REGION17, REGION18, REGION19,
                        REGION20, REGION21, REGION22, REGION23, REGION24;
    

    //hashtable to find intersections by name instead of ID
    static std::unordered_map<std::string,int> intersections;

    //hashtable to help with user inputted streets or intersections
    //used to locate the "other half" of an intersection
    static std::unordered_map<std::string,std::string> shortIntersections;
    
    //hashtable to find streets by name instead of ID
    static std::unordered_map<std::string,int> streets;
    
    //hashtable to find POIs by name instead of ID
    static std::unordered_map<std::string, std::vector<unsigned>> pointsOfInterest;
    
    static std::unordered_map<unsigned, unsigned> poiClosestIntersection;

    //These 2 latlon's create the boundary of the map (SouthWest and NorthEast)
    //set so that any point will alter them 
    //(SW being set to the most NE point and vice versa)
    static LatLon SW, NE;
    
    //correspond to the 3 buttons (respectively)
    //used to see if a button is pressed or not and do a corresponding action
    static bool speed, travel, point;
    
    //used to see if a right click box is drawn or not (and it's top left corner location)
    static bool boxDrawn, rightClick;
    static float xclick, yclick;
    
    //used to see if travel time circles have already been drawn
    static bool travelTimeDrawn;
    
    //used to see if the user wants to get directions or search POI/Intersections/Streets
    static bool directions, searchPOI, searchIntersections, searchStreets;
    
    //contains the streets that are equal or greater in size
    static std::vector<int> AllStreets;
    static std::vector<int> streetsS;
    static std::vector<int> streetsM;
    static std::vector<int> streetsL;
    static std::vector<int> streetsXL;
    
    //has the new street names after adjusting the "type" of a street
    //if not one of the types I parsed for, it will be the original string
    static std::vector<std::string> newStreetNames;
    
    //keep track of clicked POI
    static bool isPOIClicked;
    static int clickedPOIid;
    
    static int intersectionID1, intersectionID2;
    
    //used when finding directions for the user
    static t_point from, to;
    static unsigned fromIntersection, toIntersection; //IDs of intersections
    
    //used for searching and displaying POI names when searching
    //the list constantly gets overwritten to have the shortest search possible
    //(as the user types in more characters)
    //the stringstream variable is a running tally of the characters the user is entering
    static std::vector<std::string> POIList;
    static std::string POISearch;
    
    //same idea as POI searching, except now for Intersection searching
    static std::vector<std::string> intersectionsList;
    static std::string intersectionSearch;
    
    //same idea as POI searching, except now for Street searching
    static std::vector<std::string> streetsList;
    static std::string streetSearch;
    
    //number of characters currently entered by the user when searching
    static int searchCount;

	//start and end interator positions to make searching faster
	static std::vector<std::string>::iterator startSearch, endSearch;
    
    //the following relate to the 5 search options displayed (ID's of whatever is being searched)
    static unsigned option1, option2, option3, option4, option5;
    
    static std::string directionPOI;
    static unsigned directionPOIIntersectionID;

//getter and setter functions
public:
    
    //setters
    static void setPath(std::vector<unsigned> _path);
    static void setDrawMarkers(bool _drawMarkers);
    static void setFromMarker(t_point _from);
    static void setToMarker(t_point _to);
    static void setSpeedBool(bool speed_);
    static void setTravelBool(bool travel_);
    static void setPointBool(bool point_);
    static void setBoxDrawnBool(bool drawn_);
    static void setRightClickBool(bool click_);
    static void setXClick(float x);
    static void setYClick(float y);
    static void setTravelTimeDrawnBool(bool drawn_);
    static void setDirections(bool directions_);
    static void setSearchPOI(bool search);
    static void setSearchIntersections(bool search);
    static void setSearchStreets(bool search);
    static void setStreetSegmentSpeedLimit(unsigned segmentID, double speedLimit_);
    static void setIntersectionInfo(int numIntersections);
    static void setStreetInfo(int numStreets);
    static void setSegmentInfo(int numSegments);
    static void setFeatureInfo(int numFeatures);
    static void setPOIInfo(int numPoints);
    static void setSW(LatLon _SW);
    static void setNE(LatLon _NE);
    static void pushFeaturePoint(t_point position, unsigned featureID);
    static void pushInterestPoint(t_point position, unsigned interestID);
    static void setPOIClicked(bool isClicked);
    static void setClickedPOIid(int POIid);
    static void setIntersectionID1(int id);
    static void setIntersectionID2(int id);
    static void setFromPoint(t_point from_);
    static void setToPoint(t_point to_);
    static void setFromIntersection(unsigned ID);
    static void setToIntersection(unsigned ID);
    static void pushPOIList(std::string name);
    static void pushIntersectionsList(std::string name);
    static void pushStreetsList(std::string name);
    
    static void setOption(unsigned ID, int optionNumber);
    
    static void setDirectionPOIIntersectionID(unsigned id);
    
    
    //get pointer to intersections and streets hashtables for unit tests
    static std::unordered_map<std::string,int>& getIntersectionsHT();
    static std::unordered_map<std::string,std::string>& getShortIntersectionsHT();
    static std::unordered_map<std::string,int>& getStreetsHT();
    static std::unordered_map<std::string,std::vector<unsigned>>& getPOIHT();
    static std::unordered_map<unsigned, unsigned>& getPOIClosestIntersection();
    static std::vector<int>& getStreetsXL();
    static std::vector<int>& getStreetsL();
    static std::vector<int>& getStreetsM();
    static std::vector<int>& getStreetsS();
    static std::vector<int>& getAllStreets();
    
    static std::vector<std::string>& getNewStreetNames();
    static std::string getNewStreetNames(unsigned streetID);
    
    static struct intersection*& getIntersectionInfo();
    static struct street*& getStreetInfo();
    static struct streetSegment*& getSegmentInfo();
    
    //getters
    static std::vector<unsigned> getPath();
    static bool getDrawMarkers();
    static t_point getFromMarker();
    static t_point getToMarker();
    static bool getSpeedBool();
    static bool getTravelBool();
    static bool getPointBool();
    static bool getBoxDrawnBool();
    static bool getRightClickBool();
    static float getXClick();
    static float getYClick();
    static bool getTravelTimeDrawnBool();
    static bool getDirections();
    static bool getSearchPOI();
    static bool getSearchIntersections();
    static bool getSearchStreets();
    static double getStreetSegmentSpeedLimit(unsigned segmentID);
    static LatLon getSW();
    static LatLon getNE();
    static t_point getFeaturePointXY(unsigned featureID, int index);
    static t_point getInterestPoint(unsigned interestID);
    static bool getPOIClicked();
    static int getClickedPOIid();
    static int getIntersectionID1();
    static int getIntersectionID2();
    static unsigned getFromIntersection();
    static unsigned getToIntersection();
    static std::vector<std::string>& getPOIList();
    static std::vector<std::string>& getIntersectionsList();
    static std::vector<std::string>& getStreetsList();
    
    static std::string& getPOISearch();
    static std::string& getIntersectionSearch();
    static std::string& getStreetSearch();

    static std::vector<std::string>::iterator& getStartSearch();
    static std::vector<std::string>::iterator& getEndSearch();
    
    static void clearPOIList();
    static void clearIntersectionsList();
    static void clearStreetsList();
    
    static int& getSearchCount();
    static unsigned getOption(int optionNumber);
    
    //these last 2 functions correspond to the "from" and "to" for directions
    static t_point getFromPoint();
    static t_point getToPoint();
    
    //get pointer to directionPOI
    static std::string& getDirectionPOI();
    static unsigned getDirectionPOIIntersectionID();
    
    
    //all functions dealing with regions (partitioning of the map)
    static std::vector<unsigned> getREGIONVector(t_bound_box region);
    static t_bound_box getIntersectionRegion(t_point coords); 
    static std::vector<unsigned> getNewIntersectionRegion(t_bound_box region, int operation);
    
    static void setIntersectionRegion(unsigned intersectionID);
    
    static void setREGION0(t_bound_box region);
    static void setREGION1(t_bound_box region);
    static void setREGION2(t_bound_box region);
    static void setREGION3(t_bound_box region);
    static void setREGION4(t_bound_box region);
    static void setREGION5(t_bound_box region);
    static void setREGION6(t_bound_box region);
    static void setREGION7(t_bound_box region);
    static void setREGION8(t_bound_box region);
    static void setREGION9(t_bound_box region);
    static void setREGION10(t_bound_box region);
    static void setREGION11(t_bound_box region);
    static void setREGION12(t_bound_box region);
    static void setREGION13(t_bound_box region);
    static void setREGION14(t_bound_box region);
    static void setREGION15(t_bound_box region);
    static void setREGION16(t_bound_box region);
    static void setREGION17(t_bound_box region);
    static void setREGION18(t_bound_box region);
    static void setREGION19(t_bound_box region);
    static void setREGION20(t_bound_box region);
    static void setREGION21(t_bound_box region);
    static void setREGION22(t_bound_box region);
    static void setREGION23(t_bound_box region);
    static void setREGION24(t_bound_box region);
    
};

#endif	/* MAPDATA_H */

