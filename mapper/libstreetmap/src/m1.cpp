#include "m1.h"
#include "m2.h"
#include "m3.h"
#include <iostream>
#include <cmath>
#include <valarray>

using namespace std;

//used for comparing upper and lower case characters in sorting vectors
struct comparator {

    bool operator()(string str1, string str2) {

        for (int i = 0; i < (int) str1.size(); i++) str1[i] = tolower(str1[i]);
        for (int i = 0; i < (int) str2.size(); i++) str2[i] = tolower(str2[i]);
        return (str1.compare(str2) < 0);
    }

} Compare;


//load the map
bool load_map(string map_name) {

    bool load_success = loadStreetDatabaseBIN(map_name);
    
    if(load_success) {
        //These 2 latlon's create the boundary of the map (SouthWest and NorthEast)
        //set so that any point will alter them 
        //(SW being set to the most NE point and vice versa)
        LatLon SW(90, 180), NE(-90, -180);


        //finding number of intersections, streets, and street segments
        int numIntersections = (int) getNumberOfIntersections();
        int numStreets = (int) getNumberOfStreets();
        int numStreetSegments = (int) getNumberOfStreetSegments();
        int numFeatures = (int) getFeatureCount();
        int numPOIs = (int) getNumberOfPointsOfInterest();

        //setting up a hashtable of type (string name, int id) for intersections
        //also set old list of intersection names to have the entire list
        for(int i = 0; i < numIntersections; i++) {  
         
            string name = getIntersectionName(i);
            MapData::pushIntersectionsList(name);
            
            MapData::getIntersectionsHT().insert(make_pair(name, i));
            
            //find world coordinates
            LatLon position = getIntersectionPosition(i);
            if (position.lat > NE.lat) NE.lat = position.lat;
            if (position.lon > NE.lon) NE.lon = position.lon;
            if (position.lat < SW.lat) SW.lat = position.lat;
            if (position.lon < SW.lon) SW.lon = position.lon;
        }
        sort(MapData::getIntersectionsList().begin(), MapData::getIntersectionsList().end(), Compare);

        //setting up a hashtable of type (string name, int id) for streets
        //also set old list of s names to have the entire list
        for (int i = 0; i < numStreets; i++) {

            string name = getStreetName(i);
            MapData::getStreetsHT().insert(make_pair(name, i));
            
            MapData::pushStreetsList(name);
            
            parseStreetNames(name);
        }
        sort(MapData::getStreetsList().begin(), MapData::getStreetsList().end(), Compare);


        //find world coordinates
        for (int i = 0; i < numFeatures; i++) {

            int numPoints = (int) getFeaturePointCount(i);
            for (int j = 0; j < numPoints; j++) {

                LatLon position = getFeaturePoint(i, j);
                if (position.lat > NE.lat) NE.lat = position.lat;
                if (position.lon > NE.lon) NE.lon = position.lon;
                if (position.lat < SW.lat) SW.lat = position.lat;
                if (position.lon < SW.lon) SW.lon = position.lon;
            }
        }

        MapData::setSW(SW);
        MapData::setNE(NE);



        //add all connected intersections for each element from the hashtable above
        MapData::setIntersectionInfo(numIntersections);   
        for(int i = 0; i < numIntersections; i++) {

            //get IDs of segments at i'th intersection
            vector<unsigned> segments = find_intersection_street_segments(i);
            for (auto iter = segments.begin(); iter != segments.end(); iter++) {

                //get intersection IDs of the two segment ends
                StreetSegmentEnds segmentEnds = getStreetSegmentEnds(*iter);

                if ((int) segmentEnds.from == i)
                    MapData::getIntersectionInfo()[i].outIntersections.push_back(segmentEnds.to);
                else if (!getStreetSegmentOneWay(*iter))
                    MapData::getIntersectionInfo()[i].outIntersections.push_back(segmentEnds.from);
            }

            //set XY coordinates of each intersection
            t_point coordinate = latLonToCoordinates(getIntersectionPosition(i));
            MapData::getIntersectionInfo()[i].xyCoord = coordinate;
        }

        //set XY coordinates of curve points in street segments
        MapData::setSegmentInfo(numStreetSegments);
        //adding all street segments to their respective street as well
        MapData::setStreetInfo(numStreets);
        for (int i = 0; i < (int) numStreetSegments; i++) {

            unsigned streetID = getStreetSegmentStreetID(i);
            MapData::getStreetInfo()[streetID].streetSegments.push_back(i);

            double speedLimit = getStreetSegmentSpeedLimit(i);
            MapData::setStreetSegmentSpeedLimit(i, speedLimit);

            for (int j = 0; j < (int) getStreetSegmentCurvePointCount(i); j++) {

                LatLon curvePoint = getStreetSegmentCurvePoint(i, j);
                t_point curvePointXY = latLonToCoordinates(curvePoint);

                MapData::getSegmentInfo()[i].curvePointXYCoords.push_back(curvePointXY);                      
            }

        }

        //set up sets of x and y coordinates for feature points
        //this must be done after SW and NE coordinates are set 
        MapData::setFeatureInfo(numFeatures);
        for (int i = 0; i < numFeatures; i++) {

            int numPoints = (int) getFeaturePointCount(i);
            for (int j = 0; j < numPoints; j++) {

                LatLon position = getFeaturePoint(i, j);
                t_point XYcoords = latLonToCoordinates(position);
                MapData::pushFeaturePoint(XYcoords, i);
            }
        }

        //set up sets of x and y coordinates for points of interest
        //this must be done after SW and NE coordinates are set
        //also set old list of point of interest names to have the entire list
        MapData::setPOIInfo(numPOIs);
        for(int i = 0; i < numPOIs; i++) {

            string name = getPointOfInterestName(i);
            MapData::pushPOIList(name);
            
            //set up hashtable
            MapData::getPOIHT()[name].push_back(i);

            LatLon position = getPointOfInterestPosition(i);
            t_point XYcoords = latLonToCoordinates(position);
            MapData::pushInterestPoint(XYcoords, i); 
            
        }
        sort(MapData::getPOIList().begin(), MapData::getPOIList().end(), Compare);

       
        
        setStreetBoundaries(numStreets);
        
        t_point worldCoords = latLonToCoordinates(MapData::getNE());
        
        //setup all regions (5 rows and 5 columns to form a grid)
        //regions count in order from the top left of the map to the bottom right (row by row)
        t_bound_box region0(0, 4*(worldCoords.y)/5, (worldCoords.x)/5, worldCoords.y);
        MapData::setREGION0(region0);
        t_bound_box region1((worldCoords.x)/5, 4*(worldCoords.y)/5, 2*(worldCoords.x)/5, worldCoords.y);
        MapData::setREGION1(region1);
        t_bound_box region2(2*(worldCoords.x)/5, 4*(worldCoords.y)/5, 3*(worldCoords.x)/5, worldCoords.y);
        MapData::setREGION2(region2);
        t_bound_box region3(3*(worldCoords.x)/5, 4*(worldCoords.y)/5, 4*(worldCoords.x)/5, worldCoords.y);
        MapData::setREGION3(region3);
        t_bound_box region4(4*(worldCoords.x)/5, 4*(worldCoords.y)/5, worldCoords.x, worldCoords.y);
        MapData::setREGION4(region4);
        t_bound_box region5(0, 3*(worldCoords.y)/5, (worldCoords.x)/5, 4*(worldCoords.y)/5);
        MapData::setREGION5(region5);
        t_bound_box region6((worldCoords.x)/5, 3*(worldCoords.y)/5, 2*(worldCoords.x)/5, 4*(worldCoords.y)/5);
        MapData::setREGION6(region6);
        t_bound_box region7(2*(worldCoords.x)/5, 3*(worldCoords.y)/5, 3*(worldCoords.x)/5, 4*(worldCoords.y)/5);
        MapData::setREGION7(region7);
        t_bound_box region8(3*(worldCoords.x)/5, 3*(worldCoords.y)/5, 4*(worldCoords.x)/5, 4*(worldCoords.y)/5);
        MapData::setREGION8(region8);
        t_bound_box region9(4*(worldCoords.x)/5, 3*(worldCoords.y)/5, worldCoords.x, 4*(worldCoords.y)/5);
        MapData::setREGION9(region9);
        t_bound_box region10(0, 2*(worldCoords.y)/5, (worldCoords.x)/5, 3*(worldCoords.y)/5);
        MapData::setREGION10(region10);
        t_bound_box region11((worldCoords.x)/5, 2*(worldCoords.y)/5, 2*(worldCoords.x)/5, 3*(worldCoords.y)/5);
        MapData::setREGION11(region11);
        t_bound_box region12(2*(worldCoords.x)/5, 2*(worldCoords.y)/5, 3*(worldCoords.x)/5, 3*(worldCoords.y)/5);
        MapData::setREGION12(region12);
        t_bound_box region13(3*(worldCoords.x)/5, 2*(worldCoords.y)/5, 4*(worldCoords.x)/5, 3*(worldCoords.y)/5);
        MapData::setREGION13(region13);
        t_bound_box region14(4*(worldCoords.x)/5, 2*(worldCoords.y)/5, worldCoords.x, 3*(worldCoords.y)/5);
        MapData::setREGION14(region14);
        t_bound_box region15(0, worldCoords.y/5, (worldCoords.x)/5, 2*(worldCoords.y)/5);
        MapData::setREGION15(region15);
        t_bound_box region16((worldCoords.x)/5, worldCoords.y/5, 2*(worldCoords.x)/5, 2*(worldCoords.y)/5);
        MapData::setREGION16(region16);
        t_bound_box region17(2*(worldCoords.x)/5, worldCoords.y/5, 3*(worldCoords.x)/5, 2*(worldCoords.y)/5);
        MapData::setREGION17(region17);
        t_bound_box region18(3*(worldCoords.x)/5, worldCoords.y/5, 4*(worldCoords.x)/5, 2*(worldCoords.y)/5);
        MapData::setREGION18(region18);
        t_bound_box region19(4*(worldCoords.x)/5, worldCoords.y/5, worldCoords.x, 2*(worldCoords.y)/5);
        MapData::setREGION19(region19);
        t_bound_box region20(0, 0, (worldCoords.x)/5, (worldCoords.y)/5);
        MapData::setREGION20(region20);
        t_bound_box region21((worldCoords.x)/5, 0, 2*(worldCoords.x)/5, (worldCoords.y)/5);
        MapData::setREGION21(region21);
        t_bound_box region22(2*(worldCoords.x)/5, 0, 3*(worldCoords.x)/5, (worldCoords.y)/5);
        MapData::setREGION22(region22);
        t_bound_box region23(3*(worldCoords.x)/5, 0, 4*(worldCoords.x)/5, (worldCoords.y)/5);
        MapData::setREGION23(region23);
        t_bound_box region24(4*(worldCoords.x)/5, 0, worldCoords.x, (worldCoords.y)/5);
        MapData::setREGION24(region24);


        //now add each intersection to their respective region
        for(int i = 0; i < numIntersections; i++) {

            MapData::setIntersectionRegion(i);
        }
        
         for(int i = 0; i < numPOIs; i++) {
//            double shortestDistance = DBL_MAX;
            t_point position = MapData::getInterestPoint(i);
            unsigned intID = findClosestIntersection(position.x, position.y);
//            for(int j = 0; j < numIntersections; j++) {
//                double distance = find_distance_between_two_points(getPointOfInterestPosition(i), getIntersectionPosition(j));
//                if (distance < shortestDistance){
//                    shortestDistance  = distance;
//                    intID = j;
//                }
//            }
            
            MapData::getPOIClosestIntersection().insert(make_pair(i, intID));
            
        }
        
    }
    
    return load_success;
}

//close the map and free memory
void close_map() {

    closeStreetDatabase();
    
}

//abbreviates all of the street name types (e.g. road, avenue)
void parseStreetNames(std::string streetName) {

    unsigned position;
    
    if (streetName.find("Road") != string::npos) {

        position = streetName.find("Road");
        MapData::getNewStreetNames().push_back(streetName.replace(position, 4, "Rd"));
    } 
    else if (streetName.find("Street") != string::npos) {

        position = streetName.find("Street");
        MapData::getNewStreetNames().push_back(streetName.replace(position, 6, "St"));
    }
    else if (streetName.find("Avenue") != string::npos) {

        position = streetName.find("Avenue");
        MapData::getNewStreetNames().push_back(streetName.replace(position, 6, "Ave"));
    } 
    else if (streetName.find("Drive") != string::npos) {

        position = streetName.find("Drive");
        MapData::getNewStreetNames().push_back(streetName.replace(position, 5, "Dr"));
    } else if (streetName.find("Crescent") != string::npos) {

        position = streetName.find("Crescent");
        MapData::getNewStreetNames().push_back(streetName.replace(position, 8, "Cres"));
    } else if (streetName.find("Court") != string::npos) {

        position = streetName.find("Court");
        MapData::getNewStreetNames().push_back(streetName.replace(position, 5, "Ct"));
    } else if (streetName.find("Boulevard") != string::npos) {

        position = streetName.find("Boulevard");
        MapData::getNewStreetNames().push_back(streetName.replace(position, 9, "Blvd"));
    } else if (streetName.find("Circle") != string::npos) {

        position = streetName.find("Circle");
        MapData::getNewStreetNames().push_back(streetName.replace(position, 6, "Cir"));
    } else if (streetName.find("Garden") != string::npos) {

        position = streetName.find("Garden");
        MapData::getNewStreetNames().push_back(streetName.replace(position, 6, "Gdn"));
    } else if (streetName.find("Gardens") != string::npos) {

        position = streetName.find("Gardens");
        MapData::getNewStreetNames().push_back(streetName.replace(position, 7, "Gdns"));
    } else if (streetName.find("Heights") != string::npos) {

        position = streetName.find("Heights");
        MapData::getNewStreetNames().push_back(streetName.replace(position, 7, "Hts"));
    } else if (streetName.find("Route") != string::npos) {

        position = streetName.find("Route");
        MapData::getNewStreetNames().push_back(streetName.replace(position, 5, "Rte"));
    } else if (streetName.find("Square") != string::npos) {

        position = streetName.find("Square");
        MapData::getNewStreetNames().push_back(streetName.replace(position, 6, "Sq"));
    }  
    else //if it doesn't match anything, just put in the full name
        MapData::getNewStreetNames().push_back(streetName);
    
}

//get the bounds for the streets
void setStreetBoundaries(int numStreets) {

    for (int i = 0; i < numStreets; i++) {
          
        vector<unsigned> currentStreetSegments = (MapData::getStreetInfo())[i].streetSegments;
        //find the Bounds of the street (to ensure that streets names are restricted to its respective street)
        t_point low(2*PI*EARTH_RADIUS_IN_METERS, 2*PI*EARTH_RADIUS_IN_METERS), high(0,0);
        for (auto iter = currentStreetSegments.begin(); iter != currentStreetSegments.end(); iter++) {
            
            t_point from = MapData::getIntersectionInfo()[getStreetSegmentEnds(*iter).from].xyCoord;
            t_point to = MapData::getIntersectionInfo()[getStreetSegmentEnds(*iter).to].xyCoord;

            if(from.x > to.x) swap(from.x, to.x);
            if(from.y > to.y) swap(from.y, to.y);
            
            if(to.y > high.y) high.y = to.y;
            if(to.x > high.x) high.x = to.x;
            if(from.y < low.y) low.y = from.y;
            if(from.x < low.x) low.x = from.x;

            
        }
        //add it to the vector (to be used when drawing names)
        MapData::getStreetInfo()[i].streetBounds = t_bound_box(low, high);
    }
}

//function to return intersection id for an intersection name
unsigned find_intersection_id_from_name(string intersection_name) {
    
    return MapData::getIntersectionsHT().find(intersection_name)->second;    
}

//function to return street id for a street name
unsigned find_street_id_from_name(string street_name) {

    auto street = MapData::getStreetsHT().find(street_name);

    return street->second;
}

//function to return the street segments for a given intersection name
vector<unsigned> find_intersection_street_segments(string intersection_name) {

    unsigned intersectionID = find_intersection_id_from_name(intersection_name);

    return find_intersection_street_segments(intersectionID);
}

//function to return the street segments for a given intersection id
vector<unsigned> find_intersection_street_segments(unsigned intersection_id) {

    vector<unsigned> intersectionStreetSegments;
    for (int i = 0; i < (int) getIntersectionStreetSegmentCount(intersection_id); i++) {

        intersectionStreetSegments.push_back(getIntersectionStreetSegment(intersection_id, i));
    }

    return intersectionStreetSegments;
}

//function to return street names at an intersection (name)
vector<string> find_intersection_street_names(string intersection_name) {

    unsigned intersectionID = find_intersection_id_from_name(intersection_name);

    return find_intersection_street_names(intersectionID);
}

//function to return street names at an intersection (id)
vector<string> find_intersection_street_names(unsigned intersection_id) {

    vector<string> intersectionStreetNames;
    bool addedStreet = false;

    for (int i = 0; i < (int) getIntersectionStreetSegmentCount(intersection_id); i++) {

        unsigned streetSegmentID = getIntersectionStreetSegment(intersection_id, i);
        unsigned streetID = getStreetSegmentStreetID(streetSegmentID);
        string streetName = getStreetName(streetID);

        for (int j = 0; j < (int) intersectionStreetNames.size(); j++) {
            //if street has already been added to vector, don't add again
            if (streetName == intersectionStreetNames[j]) addedStreet = true;
        }

        if (!addedStreet) intersectionStreetNames.push_back(streetName);

        addedStreet = false;

    }

    return intersectionStreetNames;
}

//can you get from intersection1 to intersection2 using a single street segment
bool are_directly_connected(string intersection_name1, string intersection_name2) {

    if (intersection_name1 == intersection_name2) return true;

    unsigned intersectionID1 = MapData::getIntersectionsHT().find(intersection_name1)->second;

    unsigned intersectionID2 = MapData::getIntersectionsHT().find(intersection_name2)->second;

    vector<unsigned> intersections = MapData::getIntersectionInfo()[intersectionID1].outIntersections;
    
    for(auto iter = intersections.begin(); iter != intersections.end(); iter++) {
        if ((*iter) == intersectionID2) return true;
    }

    return false;
}

//find all intersections connected by one street segment from given intersection
vector<unsigned> find_adjacent_intersections(string intersection_name) {

    unsigned intersectionID = MapData::getIntersectionsHT().find(intersection_name)->second;

    vector<unsigned> adjacentIntersections = MapData::getIntersectionInfo()[intersectionID].outIntersections;

    return adjacentIntersections;
}

//for a given street, return all the street segments
vector<unsigned> find_street_street_segments(string street_name) {

    unsigned streetID = find_street_id_from_name(street_name);

    return MapData::getStreetInfo()[streetID].streetSegments;
}

//for a given street, find all the intersections
vector<unsigned> find_all_street_intersections(string street_name) {

    vector<unsigned> streetSegments = find_street_street_segments(street_name);

    set<unsigned> streetIntersections;

    //go through every segment, and find the adjacent intersections
    for (auto iter = streetSegments.begin(); iter != streetSegments.end(); iter++) {

        StreetSegmentEnds segmentEnds = getStreetSegmentEnds(*iter);

        streetIntersections.insert(segmentEnds.from);
        streetIntersections.insert(segmentEnds.to);
    }

    vector<unsigned> streetIntersectionsFinal(streetIntersections.begin(), streetIntersections.end());

    return streetIntersectionsFinal;
}

//find distance between two coordinates
double find_distance_between_two_points(LatLon point1, LatLon point2) {
    double distance;

    //average latitude
    double latAvg = DEG_TO_RAD * (point1.lat + point2.lat) / 2;

    //finding x and y coordinates of 2 points (in radians)
    //the cosine is to account for the fact that longitudes have different separations dependent on the latitude
    double x1 = DEG_TO_RAD * (point1.lon) * cos(latAvg);
    double x2 = DEG_TO_RAD * (point2.lon) * cos(latAvg);
    double y1 = DEG_TO_RAD * (point1.lat);
    double y2 = DEG_TO_RAD * (point2.lat);

    distance = distance_function(x1, x2, y1, y2);

    return distance;
}

//helper function (equation)
double distance_function(double x1, double x2, double y1, double y2) {

    return EARTH_RADIUS_IN_METERS * sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

//find the length of a given street segments
double find_street_segment_length(unsigned street_segment_id) {

    double distance = 0;
    LatLon currentPoint, nextPoint, endPoint;
    //set currentPoint to the starting intersection
    currentPoint = getIntersectionPosition(getStreetSegmentEnds(street_segment_id).from);
    //set endPoint to the ending intersection
    endPoint = getIntersectionPosition(getStreetSegmentEnds(street_segment_id).to);

    //loop through all the curve points and add the distance from currentPoint to nextPoint
    for (int i = 0; i < (int) getStreetSegmentCurvePointCount(street_segment_id); i++) {
        nextPoint = getStreetSegmentCurvePoint(street_segment_id, i);
        distance += find_distance_between_two_points(currentPoint, nextPoint);
        currentPoint = nextPoint;
    }

    //add the distance from the last curve point (or starting intersection if there are 0 curve points) to the end
    distance += find_distance_between_two_points(currentPoint, endPoint);

    return distance;
}

double find_street_length(int street_id) {

    string streetName = getStreetName(street_id);
    return find_street_length(streetName);
}

//find the length of a whole street
double find_street_length(string street_name) {

    double distance = 0;
    vector<unsigned> segments = find_street_street_segments(street_name);

    //add the distance of each segment
    for (auto iter = segments.begin(); iter < segments.end(); iter++) {
        distance += find_street_segment_length(*iter);
    }

    return distance;
}

//find the travel time to drive a street segment (time(minutes) = distance(Km)/speed_limit(Km/h)*60)
double find_segment_travel_time(unsigned street_segment_id) {

    return HOURS_TO_MINUTES * METRES_TO_KM * find_street_segment_length(street_segment_id) / getStreetSegmentSpeedLimit(street_segment_id);
}

//find the nearest point of interest (by name) to a given position
string find_closest_point_of_interest(LatLon my_position) {

    double leastDistance = PI*EARTH_RADIUS_IN_METERS;
    unsigned currentClosestPoI = 0;

    //loop through every point of interest and calculate the distance from our position
    //save the id of the one with the least distance (closest)
    for (int i = 0; i < (int) getNumberOfPointsOfInterest(); i++) {

        double distance = find_distance_between_two_points(my_position, getPointOfInterestPosition(i));
        if (distance < leastDistance) {
            leastDistance = distance;
            currentClosestPoI = i;
        }
    }

    return getPointOfInterestName(currentClosestPoI);
}
