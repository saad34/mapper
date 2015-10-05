#include "graphics.h"
#include "m1.h"
#include "MapData.h"
#include "draw.h"
#include "drawHelper.h"
#include <vector>


using namespace std;

//find the streets that should be drawn at the appropriate zoom
vector<int> getStreetsToDraw() {
    
    t_bound_box currentWorld = get_visible_world();
    vector<int> streetsToDraw;
        
    if(currentWorld.get_width() > XL*STREET_TO_WORLD) streetsToDraw = MapData::getStreetsXL();
    else if(currentWorld.get_width() > L*STREET_TO_WORLD) streetsToDraw = MapData::getStreetsL();
    else if(currentWorld.get_width() > M*STREET_TO_WORLD) streetsToDraw = MapData::getStreetsM();
    else if(currentWorld.get_width() > S*STREET_TO_WORLD) streetsToDraw = MapData::getStreetsS();
    else streetsToDraw = MapData::getAllStreets();
        
    return streetsToDraw;
}

//classify the streets by how long they are (highways are automatically considered the longest)
void classifyStreets() {
    
    int numStreets = (int)getNumberOfStreets();
    bool foundUnknown = false;
    
    for(int i = 0; i < numStreets; i++) {
        
        string streetName = getStreetName(i);
        
        //check to see if the group of segments called unknown
        //necessary since this agglomeration will be very large, but each section is very small
        if(!foundUnknown && streetName == "(unknown)"){
            
            foundUnknown = true;
            MapData::getAllStreets().push_back(i);
        }
        
        else{
            
            bool include = streetName.find("Highway") != string::npos || 
                           streetName.find("Collectors") != string::npos ||
                           streetName.find("Expressway") != string::npos ||
                           streetName.find("Express") != string::npos || 
                           streetName.find("Freeway") != string::npos;
            double distance = find_street_length(i)*METRES_TO_KM;

            //put streets into classes. Each vector contains all streets over specified size
            //S is for all streets S and up not just streets of size S.aciot
            MapData::getAllStreets().push_back(i);
            if(distance > S || include) MapData::getStreetsS().push_back(i);
            if(distance > M || include) MapData::getStreetsM().push_back(i);
            if(distance > L || include) MapData::getStreetsL().push_back(i);
            if(distance > XL || include) MapData::getStreetsXL().push_back(i);
        }
    }
}

double visibleWorldFunction() {
    
    double vW = (get_visible_world().get_width()+get_visible_world().get_height())/2;
    double function = STREET_NAME_DENSITY*sqrt(vW);
    return function;
}

//parse through each feature do draw accordingly
void parseFeature(unsigned featureID) {

    if(getFeatureAttribute(featureID, "water") != "") drawFeature(featureID, "water");
    
    else if(getFeatureAttribute(featureID, "natural") == "water" || 
            getFeatureAttribute(featureID, "natural") == "coastline" ||
            getFeatureAttribute(featureID, "natural") == "bay") {
        
        drawFeature(featureID, "water");
    }
    
    else if(getFeatureAttribute(featureID, "waterway") == "river" ||
            getFeatureAttribute(featureID, "waterway") == "riverbank" ||
            getFeatureAttribute(featureID, "waterway") == "canal" ||
            getFeatureAttribute(featureID, "waterway") == "dock") {
        
        drawFeature(featureID, "water");
    }
    
    else if(getFeatureAttribute(featureID, "land") != "") drawFeature(featureID, "land");
    
    else if(getFeatureAttribute(featureID, "landuse") == "grass" ||
            getFeatureAttribute(featureID, "landuse") == "meadow" ||
            getFeatureAttribute(featureID, "landuse") == "orchard" ||
            getFeatureAttribute(featureID, "landuse") == "recreation_ground" ||
            getFeatureAttribute(featureID, "landuse") == "village_green" ||
            getFeatureAttribute(featureID, "landuse") == "conservation" ||
            getFeatureAttribute(featureID, "landuse") == "park" ||
            getFeatureAttribute(featureID, "landuse") == "recreation_ground" ||
            getFeatureAttribute(featureID, "landuse") == "forest") {
        
        drawFeature(featureID, "grass/field");
    }
    
    else if(getFeatureAttribute(featureID, "natural") == "peak" ||
            getFeatureAttribute(featureID, "natural") == "ridge" ||
            getFeatureAttribute(featureID, "natural") == "cliff" ||
            getFeatureAttribute(featureID, "natural") == "rock" ||
            getFeatureAttribute(featureID, "natural") == "stone") {
        
        drawFeature(featureID, "rock");
    }
    
    else if(getFeatureAttribute(featureID, "natural") == "beach") drawFeature(featureID, "beach");
    else if(getFeatureAttribute(featureID, "natural") == "tree_row") drawFeature(featureID, "trees");
    
    else if(getFeatureAttribute(featureID, "leisure") == "garden" ||
            getFeatureAttribute(featureID, "leisure") == "golf_course" ||
            getFeatureAttribute(featureID, "leisure") == "park" ||
            getFeatureAttribute(featureID, "leisure") == "pitch") {
        
        drawFeature(featureID, "grass/field");
    }
}

//converts XY to LatLon
LatLon XYToLatLon(double x, double y) {
    
    LatLon SW = MapData::getSW();
    t_point point(x,y);
    
    /* y coordinate was determined by finding distance between
       SW.lat and the latitude we are looking for (b/c Earth_y = latitude):
       y = EARTH_RADIUS * (lat*DEG_TO_RAD - SW.lat*DEG_TO_RAD) */
    double lat = abs(y/METRES_TO_KM/EARTH_RADIUS_IN_METERS/DEG_TO_RAD) + SW.lat;
    
    //average latitude
    double latAvg = DEG_TO_RAD * (SW.lat + lat) / 2;
    
    /* x coordinate is determined by finding distance between
       SW.lon and the longitude we need:
       x = EARTH_RADIUS * (lon*DEG_TO_RAD*cos(latAvg) - SW.lon*DEG_TO_RAD*cos(latAvg)) */ 
    double lon = abs(x/METRES_TO_KM/EARTH_RADIUS_IN_METERS/DEG_TO_RAD/cos(latAvg)) + SW.lon;
    
    LatLon coordinate(lat,lon);
    return coordinate;
}

bool clickedOnPOI(double x, double y, string& name, int& id) {
    
    string closestPOI = find_closest_point_of_interest(XYToLatLon(x,y));
    bool clicked = false;
    
    for(int i = 0; i < (int)getNumberOfPointsOfInterest() && !clicked; i++) {
        if(getPointOfInterestName(i) == closestPOI) {
            
            t_point POICentre = MapData::getInterestPoint(i);
            t_bound_box currentWorld = get_visible_world();
            double POIRadius = POI_OUT_CIR_RAD*currentWorld.get_width();

            //Pythagorean theorem
            bool insideCircle = POIRadius*POIRadius >= (pow(x - POICentre.x, 2) + pow(y - POICentre.y, 2));
            if(insideCircle) {
                
                id = i;
                name = closestPOI;
                clicked = true;
            }
        }
    }
    
    return clicked;
}
