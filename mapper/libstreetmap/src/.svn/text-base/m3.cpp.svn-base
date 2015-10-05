#include "m3.h"

using namespace std;

vector<unsigned> find_path_between_intersections(unsigned intersect_id_start, 
                            unsigned intersect_id_end) {
    
    vector<unsigned> path = searchPath(intersect_id_start, intersect_id_end);
    return path;
}


double compute_path_travel_time(const vector<unsigned>& path) {
    
    double travelTime = 0;
    
    if(path.size() > 0) {
        travelTime += find_segment_travel_time(*path.begin());

        for (auto iter = (path.begin() + 1); iter != path.end(); iter++) {

            travelTime += find_segment_travel_time(*iter);

            unsigned street1 = getStreetSegmentStreetID(*(iter-1));
            unsigned street2 = getStreetSegmentStreetID(*iter);

            if (street1 != street2) travelTime += TURN_TIME;

        }
    }
    
    return travelTime;
}

double compute_additional_path_travel_time(int previousSegment, unsigned currentSegment) {
    
    double travelTime = 0;
    

    travelTime += find_segment_travel_time(currentSegment);
    if(previousSegment < 0) return travelTime;

    unsigned street1 = getStreetSegmentStreetID(previousSegment);
    unsigned street2 = getStreetSegmentStreetID(currentSegment);

    if (street1 != street2) travelTime += TURN_TIME;

    return travelTime;
}


vector<unsigned> find_path_to_point_of_interest (unsigned 
                   intersect_id_start, string point_of_interest_name) {

    vector<unsigned> pointOfInterestIDs = MapData::getPOIHT().find(point_of_interest_name)->second;
    
    //find path to all POIs of that name for now (need to minimum travel time)
    //maybe only check intersection's region later
    vector<unsigned> finalPath;
    vector<unsigned> closestIntersections;
    double time = DBL_MAX;
    
    for(auto iter = pointOfInterestIDs.begin(); iter != pointOfInterestIDs.end(); iter++) {

        unsigned currPOIID = *iter;
        unsigned toIntersection = (*(MapData::getPOIClosestIntersection().find(currPOIID))).second;
        closestIntersections.push_back(toIntersection);

//        vector<unsigned> path = find_path_between_intersections(intersect_id_start, toIntersection);
//
//        double pathTime = time;
//        if(path.size() != 0) pathTime = compute_path_travel_time(path);
//        
//        if(pathTime < time) {
//            time = pathTime;
//            finalPath = path;
//            //pointOfInterestID = currPOIID;
//            //finalToInt = toIntersection;
//        }
    }
    
   finalPath = searchPOIPath(intersect_id_start, closestIntersections);

    return finalPath;
}

unsigned findClosestIntersection(double x, double y) {
    
    t_point coords(x,y);
    t_point intersectionCoord(x,y);
    t_bound_box region = MapData::getIntersectionRegion(coords);
    unsigned intersectionID;

    //must make it a large number to start with
    double distance = DBL_MAX;
    vector<unsigned> regionIntersections = MapData::getREGIONVector(region);
    
    for(auto iter = regionIntersections.begin(); iter != regionIntersections.end();
            iter++) {

        unsigned currIntersection = *iter;
        t_point position = latLonToCoordinates(getIntersectionPosition(currIntersection));
        double comparisonDistance = sqrt(pow(abs(coords.x - position.x), 2) +
                                    pow(abs(coords.y - position.y), 2));


        if(comparisonDistance < distance) {

            distance = comparisonDistance;
            intersectionCoord = position;
            intersectionID = currIntersection;
        }
    }
    
   
    //check if an intersection in another region is closer
    checkDistanceToBoundaries(distance, region, coords, intersectionCoord, intersectionID);
       
    return intersectionID;
}
