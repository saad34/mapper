#include "MapData.h"
using namespace std;


/***** Redeclaration of variables for static properties *****/

std::vector<unsigned> MapData::path;
bool MapData::drawMarkers;
t_point MapData::fromMarker, MapData::toMarker;
MapData::Intersection *MapData::intersectionInfo;
MapData::Street *MapData::streetInfo;
MapData::StreetSegment *MapData::segmentInfo;
MapData::Feature *MapData::featureXYPoints;
MapData::InterestPoint *MapData::interestXYPoints;
MapData::IntersectionRegions *MapData::intersectionRegions;
t_bound_box MapData::REGION0, MapData::REGION1, MapData::REGION2, MapData::REGION3, 
            MapData::REGION4, MapData::REGION5, MapData::REGION6, MapData::REGION7, 
            MapData::REGION8, MapData::REGION9, MapData::REGION10, MapData::REGION11, 
            MapData::REGION12, MapData::REGION13, MapData::REGION14, MapData::REGION15, 
            MapData::REGION16, MapData::REGION17, MapData::REGION18, MapData::REGION19,
            MapData::REGION20, MapData::REGION21, MapData::REGION22, MapData::REGION23, 
            MapData::REGION24;
unordered_map<string,int> MapData::intersections;
unordered_map<string,string> MapData::shortIntersections;
unordered_map<string,int> MapData::streets;
unordered_map<string, vector<unsigned>> MapData::pointsOfInterest;
unordered_map<unsigned, unsigned> MapData::poiClosestIntersection;
LatLon MapData::SW, MapData::NE;
bool MapData::speed, MapData::travel, MapData::point, MapData::boxDrawn, MapData::rightClick,
        MapData::travelTimeDrawn, MapData::directions, MapData::searchPOI,
        MapData::searchIntersections, MapData::searchStreets;
float MapData::xclick, MapData::yclick;

//each vector contains the streets that are over a specified size
//(e.g. streetsM contains all streets size M and up)
vector<int> MapData::AllStreets;
vector<int> MapData::streetsS;
vector<int> MapData::streetsM;
vector<int> MapData::streetsL;
vector<int> MapData::streetsXL;

vector<string> MapData::newStreetNames;

bool MapData::isPOIClicked;
int MapData::clickedPOIid, MapData::searchCount;
int MapData::intersectionID1, MapData::intersectionID2;
t_point MapData::from, MapData::to;
unsigned MapData::fromIntersection, MapData::toIntersection;

vector<string> MapData::POIList, MapData::intersectionsList, MapData::streetsList;

string MapData::POISearch, MapData::intersectionSearch, MapData::streetSearch;
vector<string>::iterator MapData::startSearch, MapData::endSearch;
unsigned MapData::option1, MapData::option2, MapData::option3, MapData::option4, MapData::option5;

string MapData::directionPOI;
unsigned MapData::directionPOIIntersectionID;



/***************** Setters *****************/

void MapData::setSpeedBool(bool speed_) { speed = speed_; }
void MapData::setTravelBool(bool travel_) { travel = travel_; }
void MapData::setPointBool(bool point_) { point = point_; }
void MapData::setBoxDrawnBool(bool drawn_) { boxDrawn = drawn_; }
void MapData::setRightClickBool(bool click_) { rightClick = click_; }
void MapData::setXClick(float x) { xclick = x; }
void MapData::setYClick(float y) { yclick = y; }
void MapData::setTravelTimeDrawnBool(bool drawn_) { travelTimeDrawn = drawn_; }
void MapData::setDirections(bool directions_) { directions = directions_; }
void MapData::setSearchPOI(bool search) { searchPOI = search; }
void MapData::setSearchIntersections(bool search) { searchIntersections = search; }
void MapData::setSearchStreets(bool search) { searchStreets = search; }

void MapData::setStreetSegmentSpeedLimit(unsigned segmentID, double speedLimit_) {
 
    segmentInfo[segmentID].speedLimit = speedLimit_;
}

void MapData::setIntersectionInfo(int numIntersections) {
    
    intersectionInfo = new Intersection[numIntersections];
    intersectionRegions = new IntersectionRegions[25]; //25 regions
    return;
}

void MapData::setStreetInfo(int numStreets) {
    
    streetInfo = new Street[numStreets];
    return;
}

void MapData::setSegmentInfo(int numSegments) {
    
    segmentInfo = new StreetSegment[numSegments];
    return;
}

void MapData::setFeatureInfo(int numFeatures) {
    
    featureXYPoints = new Feature[numFeatures];
    return;
}

void MapData::setPOIInfo(int numPoints) {
    
    interestXYPoints = new InterestPoint[numPoints];
    return;
}

void MapData::setSW(LatLon _SW){ SW = _SW; }
void MapData::setNE(LatLon _NE){ NE = _NE; }

void MapData::pushFeaturePoint(t_point position, unsigned featureID) {
    
    featureXYPoints[featureID].x.push_back(position.x);
    featureXYPoints[featureID].y.push_back(position.y);
}

void MapData::pushInterestPoint(t_point position, unsigned interestID) {
 
    interestXYPoints[interestID].x = position.x;
    interestXYPoints[interestID].y = position.y;
}

void MapData::setPath(std::vector<unsigned> _path) { path = _path; }
void MapData::setDrawMarkers(bool _drawMarkers) { drawMarkers = _drawMarkers; }
void MapData::setFromMarker(t_point _from) { fromMarker = _from; }
void MapData::setToMarker(t_point _to) { toMarker = _to; }
void MapData::setPOIClicked(bool isClicked) { isPOIClicked = isClicked; }
void MapData::setClickedPOIid(int POIid) { clickedPOIid = POIid; }
void MapData::setIntersectionID1(int id) { intersectionID1 = id;}
void MapData::setIntersectionID2(int id) { intersectionID2 = id;}

void MapData::setFromPoint(t_point from_) { from.x = from_.x; from.y = from_.y; }
void MapData::setToPoint(t_point to_) { to.x = to_.x; to.y = to_.y; }
void MapData::setFromIntersection(unsigned ID) { fromIntersection = ID; }
void MapData::setToIntersection(unsigned ID) { toIntersection = ID; }

void MapData::pushPOIList(std::string name) { POIList.push_back(name); }
void MapData::pushIntersectionsList(std::string name) { intersectionsList.push_back(name); }
void MapData::pushStreetsList(std::string name) { streetsList.push_back(name); }

void MapData::setOption(unsigned ID, int optionNumber) {
 
    if(optionNumber == 1) option1 = ID;
    else if(optionNumber == 2) option2 = ID;
    else if(optionNumber == 3) option3 = ID;
    else if(optionNumber == 4) option4 = ID;
    else option5 = ID;
}

void MapData::setDirectionPOIIntersectionID(unsigned id) { directionPOIIntersectionID = id; }



/***************** Getters *****************/

std::vector<unsigned> MapData::getPath() { return path; }
bool MapData::getDrawMarkers() { return drawMarkers; }
t_point MapData::getFromMarker() { return fromMarker; }
t_point MapData::getToMarker() {return toMarker; }
bool MapData::getSpeedBool() { return speed; }
bool MapData::getTravelBool() { return travel; }
bool MapData::getPointBool() { return point; }
bool MapData::getBoxDrawnBool() { return boxDrawn; }
bool MapData::getRightClickBool() { return rightClick; }
float MapData::getXClick() { return xclick; }
float MapData::getYClick() { return yclick; }
bool MapData::getTravelTimeDrawnBool() { return travelTimeDrawn; }
bool MapData::getDirections() { return directions; }
bool MapData::getSearchPOI() { return searchPOI; }
bool MapData::getSearchIntersections() { return searchIntersections; }
bool MapData::getSearchStreets() { return searchStreets; }

double MapData::getStreetSegmentSpeedLimit(unsigned segmentID) {
 
    return segmentInfo[segmentID].speedLimit;
}

unordered_map<string,int>& MapData::getIntersectionsHT() { return intersections; }
unordered_map<string,string>& MapData::getShortIntersectionsHT() { return shortIntersections; }
unordered_map<string,int>& MapData::getStreetsHT() { return streets; }
unordered_map<string,vector<unsigned>>& MapData::getPOIHT(){ return pointsOfInterest; }
unordered_map<unsigned, unsigned>& MapData::getPOIClosestIntersection() { return poiClosestIntersection; }

vector<int>& MapData::getStreetsXL() { return streetsXL; }
vector<int>& MapData::getStreetsL() { return streetsL; }
vector<int>& MapData::getStreetsM() { return streetsM; }
vector<int>& MapData::getStreetsS() { return streetsS; }
vector<int>& MapData::getAllStreets() { return AllStreets; }

vector<string>& MapData::getNewStreetNames() { return newStreetNames; }
string MapData::getNewStreetNames(unsigned streetID) { return newStreetNames[streetID]; }

struct MapData::intersection*& MapData::getIntersectionInfo() { return intersectionInfo; }
struct MapData::street*& MapData::getStreetInfo() { return streetInfo; }
struct MapData::streetSegment*& MapData::getSegmentInfo() { return segmentInfo; }
LatLon MapData::getSW() { return SW; }
LatLon MapData::getNE() { return NE; };

t_point MapData::getFeaturePointXY(unsigned featureID, int index) {
    
    t_point position;
    position.x = featureXYPoints[featureID].x[index];
    position.y = featureXYPoints[featureID].y[index];
    
    return position;
}

t_point MapData::getInterestPoint(unsigned interestID) {
    
    t_point position;
    position.x = interestXYPoints[interestID].x;
    position.y = interestXYPoints[interestID].y;
    
    return position;
}

bool MapData::getPOIClicked() { return isPOIClicked; }
int MapData::getClickedPOIid() { return clickedPOIid; }
int MapData::getIntersectionID1() { return intersectionID1; }
int MapData::getIntersectionID2() { return intersectionID2; }

t_point MapData::getFromPoint() { return from; }
t_point MapData::getToPoint() { return to; }
unsigned MapData::getFromIntersection() { return fromIntersection; }
unsigned MapData::getToIntersection() { return toIntersection; }

vector<string>& MapData::getPOIList() { return POIList; }
vector<string>& MapData::getIntersectionsList() { return intersectionsList; }
vector<string>& MapData::getStreetsList() { return streetsList; }

string& MapData::getPOISearch() { return POISearch; }
string& MapData::getIntersectionSearch() { return intersectionSearch; }
string& MapData::getStreetSearch() { return streetSearch; }

vector<string>::iterator& MapData::getStartSearch() { return startSearch; }
vector<string>::iterator& MapData::getEndSearch() { return endSearch; }

std::string& MapData::getDirectionPOI() { return directionPOI; }
unsigned MapData::getDirectionPOIIntersectionID() { return directionPOIIntersectionID; }

int& MapData::getSearchCount() { return searchCount; }

unsigned MapData::getOption(int optionNumber) {
 
    if(optionNumber == 1) return option1;
    else if(optionNumber == 2) return option2;
    else if(optionNumber == 3) return option3;
    else if(optionNumber == 4) return option4;
    else return option5;
}

void MapData::clearPOIList() { POIList.empty(); }
void MapData::clearIntersectionsList() { intersectionsList.empty(); }
void MapData::clearStreetsList() { streetsList.empty(); }



/***** Below are all functions related to REGIONS *****/

void MapData::setREGION0(t_bound_box region) { REGION0 = region; }
void MapData::setREGION1(t_bound_box region) { REGION1 = region; }
void MapData::setREGION2(t_bound_box region) { REGION2 = region; }
void MapData::setREGION3(t_bound_box region) { REGION3 = region; }
void MapData::setREGION4(t_bound_box region) { REGION4 = region; }
void MapData::setREGION5(t_bound_box region) { REGION5 = region; }
void MapData::setREGION6(t_bound_box region) { REGION6 = region; }
void MapData::setREGION7(t_bound_box region) { REGION7 = region; }
void MapData::setREGION8(t_bound_box region) { REGION8 = region; }
void MapData::setREGION9(t_bound_box region) { REGION9 = region; }
void MapData::setREGION10(t_bound_box region) { REGION10 = region; }
void MapData::setREGION11(t_bound_box region) { REGION11 = region; }
void MapData::setREGION12(t_bound_box region) { REGION12 = region; }
void MapData::setREGION13(t_bound_box region) { REGION13 = region; }
void MapData::setREGION14(t_bound_box region) { REGION14 = region; }
void MapData::setREGION15(t_bound_box region) { REGION15 = region; }
void MapData::setREGION16(t_bound_box region) { REGION16 = region; }
void MapData::setREGION17(t_bound_box region) { REGION17 = region; }
void MapData::setREGION18(t_bound_box region) { REGION18 = region; }
void MapData::setREGION19(t_bound_box region) { REGION19 = region; }
void MapData::setREGION20(t_bound_box region) { REGION20 = region; }
void MapData::setREGION21(t_bound_box region) { REGION21 = region; }
void MapData::setREGION22(t_bound_box region) { REGION22 = region; }
void MapData::setREGION23(t_bound_box region) { REGION23 = region; }
void MapData::setREGION24(t_bound_box region) { REGION24 = region; }

void MapData::setIntersectionRegion(unsigned intersectionID) {
    
    LatLon latloncoords = getIntersectionPosition(intersectionID);
    t_point coords = latLonToCoordinates(latloncoords);
    
    if((coords.x >= REGION0.left()) && (coords.x <= REGION0.right()) &&
       (coords.y >= REGION0.bottom()) && (coords.y <= REGION0.top())) {
        
        intersectionRegions[0].regions.push_back(intersectionID);
    }
    if((coords.x >= REGION1.left()) && (coords.x <= REGION1.right()) &&
       (coords.y >= REGION1.bottom()) && (coords.y <= REGION1.top())) {
        
        intersectionRegions[1].regions.push_back(intersectionID);
    }
    if((coords.x >= REGION2.left()) && (coords.x <= REGION2.right()) &&
       (coords.y >= REGION2.bottom()) && (coords.y <= REGION2.top())) {
        
        intersectionRegions[2].regions.push_back(intersectionID);
    }
    if((coords.x >= REGION3.left()) && (coords.x <= REGION3.right()) &&
       (coords.y >= REGION3.bottom()) && (coords.y <= REGION3.top())) {
        
        intersectionRegions[3].regions.push_back(intersectionID);
    }
    if((coords.x >= REGION4.left()) && (coords.x <= REGION4.right()) &&
       (coords.y >= REGION4.bottom()) && (coords.y <= REGION4.top())) {
        
        intersectionRegions[4].regions.push_back(intersectionID);
    }
    if((coords.x >= REGION5.left()) && (coords.x <= REGION5.right()) &&
       (coords.y >= REGION5.bottom()) && (coords.y <= REGION5.top())) {
        
        intersectionRegions[5].regions.push_back(intersectionID);
    }
    if((coords.x >= REGION6.left()) && (coords.x <= REGION6.right()) &&
       (coords.y >= REGION6.bottom()) && (coords.y <= REGION6.top())) {
        
        intersectionRegions[6].regions.push_back(intersectionID);
    }
    if((coords.x >= REGION7.left()) && (coords.x <= REGION7.right()) &&
       (coords.y >= REGION7.bottom()) && (coords.y <= REGION7.top())) {
        
        intersectionRegions[7].regions.push_back(intersectionID);
    }
    if((coords.x >= REGION8.left()) && (coords.x <= REGION8.right()) &&
       (coords.y >= REGION8.bottom()) && (coords.y <= REGION8.top())) {
        
        intersectionRegions[8].regions.push_back(intersectionID);
    }
    if((coords.x >= REGION9.left()) && (coords.x <= REGION9.right()) &&
       (coords.y >= REGION9.bottom()) && (coords.y <= REGION9.top())) {
        
        intersectionRegions[9].regions.push_back(intersectionID);
    }
    if((coords.x >= REGION10.left()) && (coords.x <= REGION10.right()) &&
       (coords.y >= REGION10.bottom()) && (coords.y <= REGION10.top())) {
        
        intersectionRegions[10].regions.push_back(intersectionID);
    }
    if((coords.x >= REGION11.left()) && (coords.x <= REGION11.right()) &&
       (coords.y >= REGION11.bottom()) && (coords.y <= REGION11.top())) {
        
        intersectionRegions[11].regions.push_back(intersectionID);
    }
    if((coords.x >= REGION12.left()) && (coords.x <= REGION12.right()) &&
       (coords.y >= REGION12.bottom()) && (coords.y <= REGION12.top())) {
        
        intersectionRegions[12].regions.push_back(intersectionID);
    }
    if((coords.x >= REGION13.left()) && (coords.x <= REGION13.right()) &&
       (coords.y >= REGION13.bottom()) && (coords.y <= REGION13.top())) {
        
        intersectionRegions[13].regions.push_back(intersectionID);
    }
    if((coords.x >= REGION14.left()) && (coords.x <= REGION14.right()) &&
       (coords.y >= REGION14.bottom()) && (coords.y <= REGION14.top())) {
        
        intersectionRegions[14].regions.push_back(intersectionID);
    }
    if((coords.x >= REGION15.left()) && (coords.x <= REGION15.right()) &&
       (coords.y >= REGION15.bottom()) && (coords.y <= REGION15.top())) {
        
        intersectionRegions[15].regions.push_back(intersectionID);
    }
    if((coords.x >= REGION16.left()) && (coords.x <= REGION16.right()) &&
       (coords.y >= REGION16.bottom()) && (coords.y <= REGION16.top())) {
        
        intersectionRegions[16].regions.push_back(intersectionID);
    }
    if((coords.x >= REGION17.left()) && (coords.x <= REGION17.right()) &&
       (coords.y >= REGION17.bottom()) && (coords.y <= REGION17.top())) {
        
        intersectionRegions[17].regions.push_back(intersectionID);
    }
    if((coords.x >= REGION18.left()) && (coords.x <= REGION18.right()) &&
       (coords.y >= REGION18.bottom()) && (coords.y <= REGION18.top())) {
        
        intersectionRegions[18].regions.push_back(intersectionID);
    }
    if((coords.x >= REGION19.left()) && (coords.x <= REGION19.right()) &&
       (coords.y >= REGION19.bottom()) && (coords.y <= REGION19.top())) {
        
        intersectionRegions[19].regions.push_back(intersectionID);
    }
    if((coords.x >= REGION20.left()) && (coords.x <= REGION20.right()) &&
       (coords.y >= REGION20.bottom()) && (coords.y <= REGION20.top())) {
        
        intersectionRegions[20].regions.push_back(intersectionID);
    }
    if((coords.x >= REGION21.left()) && (coords.x <= REGION21.right()) &&
       (coords.y >= REGION21.bottom()) && (coords.y <= REGION21.top())) {
        
        intersectionRegions[21].regions.push_back(intersectionID);
    }
    if((coords.x >= REGION22.left()) && (coords.x <= REGION22.right()) &&
       (coords.y >= REGION22.bottom()) && (coords.y <= REGION22.top())) {
        
        intersectionRegions[22].regions.push_back(intersectionID);
    }
    if((coords.x >= REGION23.left()) && (coords.x <= REGION23.right()) &&
       (coords.y >= REGION23.bottom()) && (coords.y <= REGION23.top())) {
        
        intersectionRegions[23].regions.push_back(intersectionID);
    }
    if((coords.x >= REGION24.left()) && (coords.x <= REGION24.right()) &&
       (coords.y >= REGION24.bottom()) && (coords.y <= REGION24.top())) {
        
        intersectionRegions[24].regions.push_back(intersectionID);
    }
}

vector<unsigned> MapData::getREGIONVector(t_bound_box region) { 
    
    if(region == REGION0) return intersectionRegions[0].regions;
    else if(region == REGION1) return intersectionRegions[1].regions;
    else if(region == REGION2) return intersectionRegions[2].regions; 
    else if(region == REGION3) return intersectionRegions[3].regions; 
    else if(region == REGION4) return intersectionRegions[4].regions; 
    else if(region == REGION5) return intersectionRegions[5].regions; 
    else if(region == REGION6) return intersectionRegions[6].regions; 
    else if(region == REGION7) return intersectionRegions[7].regions; 
    else if(region == REGION8) return intersectionRegions[8].regions; 
    else if(region == REGION9) return intersectionRegions[9].regions; 
    else if(region == REGION10) return intersectionRegions[10].regions;
    else if(region == REGION11) return intersectionRegions[11].regions;
    else if(region == REGION12) return intersectionRegions[12].regions;
    else if(region == REGION13) return intersectionRegions[13].regions;
    else if(region == REGION14) return intersectionRegions[14].regions;
    else if(region == REGION15) return intersectionRegions[15].regions;
    else if(region == REGION16) return intersectionRegions[16].regions;
    else if(region == REGION17) return intersectionRegions[17].regions;
    else if(region == REGION18) return intersectionRegions[18].regions;
    else if(region == REGION19) return intersectionRegions[19].regions;
    else if(region == REGION20) return intersectionRegions[20].regions;
    else if(region == REGION21) return intersectionRegions[21].regions;
    else if(region == REGION22) return intersectionRegions[22].regions;
    else if(region == REGION23) return intersectionRegions[23].regions;
    else return intersectionRegions[24].regions;
}

t_bound_box MapData::getIntersectionRegion(t_point coords) {
    
    if((coords.x >= REGION0.left()) && (coords.x <= REGION0.right()) &&
       (coords.y >= REGION0.bottom()) && (coords.y <= REGION0.top())) {
        
        return REGION0;
    }
    else if((coords.x >= REGION1.left()) && (coords.x <= REGION1.right()) &&
       (coords.y >= REGION1.bottom()) && (coords.y <= REGION1.top())) {
        
        return REGION1;
    }
    else if((coords.x >= REGION2.left()) && (coords.x <= REGION2.right()) &&
       (coords.y >= REGION2.bottom()) && (coords.y <= REGION2.top())) {
        
        return REGION2;
    }
    else if((coords.x >= REGION3.left()) && (coords.x <= REGION3.right()) &&
       (coords.y >= REGION3.bottom()) && (coords.y <= REGION3.top())) {
        
        return REGION3;
    }
    else if((coords.x >= REGION4.left()) && (coords.x <= REGION4.right()) &&
       (coords.y >= REGION4.bottom()) && (coords.y <= REGION4.top())) {
        
        return REGION4;
    }
    else if((coords.x >= REGION5.left()) && (coords.x <= REGION5.right()) &&
       (coords.y >= REGION5.bottom()) && (coords.y <= REGION5.top())) {
        
        return REGION5;
    }
    else if((coords.x >= REGION6.left()) && (coords.x <= REGION6.right()) &&
       (coords.y >= REGION6.bottom()) && (coords.y <= REGION6.top())) {
        
        return REGION6;
    }
    else if((coords.x >= REGION7.left()) && (coords.x <= REGION7.right()) &&
       (coords.y >= REGION7.bottom()) && (coords.y <= REGION7.top())) {
        
        return REGION7;
    }
    else if((coords.x >= REGION8.left()) && (coords.x <= REGION8.right()) &&
       (coords.y >= REGION8.bottom()) && (coords.y <= REGION8.top())) {
        
        return REGION8;
    }
    else if((coords.x >= REGION9.left()) && (coords.x <= REGION9.right()) &&
       (coords.y >= REGION9.bottom()) && (coords.y <= REGION9.top())) {
        
        return REGION9;
    }
    else if((coords.x >= REGION10.left()) && (coords.x <= REGION10.right()) &&
       (coords.y >= REGION10.bottom()) && (coords.y <= REGION10.top())) {
        
        return REGION10;
    }
    else if((coords.x >= REGION11.left()) && (coords.x <= REGION11.right()) &&
       (coords.y >= REGION11.bottom()) && (coords.y <= REGION11.top())) {
        
        return REGION11;
    }
    else if((coords.x >= REGION12.left()) && (coords.x <= REGION12.right()) &&
       (coords.y >= REGION12.bottom()) && (coords.y <= REGION12.top())) {
        
        return REGION12;
    }
    else if((coords.x >= REGION13.left()) && (coords.x <= REGION13.right()) &&
       (coords.y >= REGION13.bottom()) && (coords.y <= REGION13.top())) {
        
        return REGION13;
    }
    else if((coords.x >= REGION14.left()) && (coords.x <= REGION14.right()) &&
       (coords.y >= REGION14.bottom()) && (coords.y <= REGION14.top())) {
        
        return REGION14;
    }
    else if((coords.x >= REGION15.left()) && (coords.x <= REGION15.right()) &&
       (coords.y >= REGION15.bottom()) && (coords.y <= REGION15.top())) {
        
        return REGION15;
    }
    else if((coords.x >= REGION16.left()) && (coords.x <= REGION16.right()) &&
       (coords.y >= REGION16.bottom()) && (coords.y <= REGION16.top())) {
        
        return REGION16;
    }
    else if((coords.x >= REGION17.left()) && (coords.x <= REGION17.right()) &&
       (coords.y >= REGION17.bottom()) && (coords.y <= REGION17.top())) {
        
        return REGION17;
    }
    else if((coords.x >= REGION18.left()) && (coords.x <= REGION18.right()) &&
       (coords.y >= REGION18.bottom()) && (coords.y <= REGION18.top())) {
        
        return REGION18;
    }
    else if((coords.x >= REGION19.left()) && (coords.x <= REGION19.right()) &&
       (coords.y >= REGION19.bottom()) && (coords.y <= REGION19.top())) {
        
        return REGION19;
    }
    else if((coords.x >= REGION20.left()) && (coords.x <= REGION20.right()) &&
       (coords.y >= REGION20.bottom()) && (coords.y <= REGION20.top())) {
        
        return REGION20;
    }
    else if((coords.x >= REGION21.left()) && (coords.x <= REGION21.right()) &&
       (coords.y >= REGION21.bottom()) && (coords.y <= REGION21.top())) {
        
        return REGION21;
    }
    else if((coords.x >= REGION22.left()) && (coords.x <= REGION22.right()) &&
       (coords.y >= REGION22.bottom()) && (coords.y <= REGION22.top())) {
        
        return REGION22;
    }
    if((coords.x >= REGION23.left()) && (coords.x <= REGION23.right()) &&
       (coords.y >= REGION23.bottom()) && (coords.y <= REGION23.top())) {
        
        return REGION23;
    }
    else if((coords.x >= REGION24.left()) && (coords.x <= REGION24.right()) &&
       (coords.y >= REGION24.bottom()) && (coords.y <= REGION24.top())) {
        
        return REGION24;
    }
    
    
    t_bound_box nullBox(0,0,0,0); //just in case
    return nullBox;
}

//returns a new region based on where you are currently at and where you want to go
//+1 = right, -1 = left, +5 = below, -5 = top
vector<unsigned> MapData::getNewIntersectionRegion(t_bound_box region, int operation) {
    
    if(region == REGION0 && operation >= 0 && operation < 25) return intersectionRegions[0 + operation].regions;
    else if(region == REGION1 && (1 + operation) >= 0 && (1 + operation) < 25) return intersectionRegions[1 + operation].regions;
    else if(region == REGION2 && (2 + operation) >= 0 && (2 + operation) < 25) return intersectionRegions[2 + operation].regions;
    else if(region == REGION3 && (3 + operation) >= 0 && (3 + operation) < 25) return intersectionRegions[3 + operation].regions;
    else if(region == REGION4 && (4 + operation) >= 0 && (4 + operation) < 25) return intersectionRegions[4 + operation].regions;
    else if(region == REGION5 && (5 + operation) >= 0 && (5 + operation) < 25) return intersectionRegions[5 + operation].regions;
    else if(region == REGION6 && (6 + operation) >= 0 && (6 + operation) < 25) return intersectionRegions[6 + operation].regions;
    else if(region == REGION7 && (7 + operation) >= 0 && (7 + operation) < 25) return intersectionRegions[7 + operation].regions;
    else if(region == REGION8 && (8 + operation) >= 0 && (8 + operation) < 25) return intersectionRegions[8 + operation].regions;
    else if(region == REGION9 && (9 + operation) >= 0 && (9 + operation) < 25) return intersectionRegions[9 + operation].regions;
    else if(region == REGION10 && (10 + operation) >= 0 && (10 + operation) < 25) return intersectionRegions[10 + operation].regions;
    else if(region == REGION11 && (11 + operation) >= 0 && (11 + operation) < 25) return intersectionRegions[11 + operation].regions;
    else if(region == REGION12 && (12 + operation) >= 0 && (12 + operation) < 25) return intersectionRegions[12 + operation].regions;
    else if(region == REGION13 && (13 + operation) >= 0 && (13 + operation) < 25) return intersectionRegions[13 + operation].regions;
    else if(region == REGION14 && (14 + operation) >= 0 && (14 + operation) < 25) return intersectionRegions[14 + operation].regions;
    else if(region == REGION15 && (15 + operation) >= 0 && (15 + operation) < 25) return intersectionRegions[15 + operation].regions;
    else if(region == REGION16 && (16 + operation) >= 0 && (16 + operation) < 25) return intersectionRegions[16 + operation].regions;
    else if(region == REGION17 && (17 + operation) >= 0 && (17 + operation) < 25) return intersectionRegions[17 + operation].regions;
    else if(region == REGION18 && (18 + operation) >= 0 && (18 + operation) < 25) return intersectionRegions[18 + operation].regions;
    else if(region == REGION19 && (19 + operation) >= 0 && (19 + operation) < 25) return intersectionRegions[19 + operation].regions;
    else if(region == REGION20 && (20 + operation) >= 0 && (20 + operation) < 25) return intersectionRegions[20 + operation].regions;
    else if(region == REGION21 && (21 + operation) >= 0 && (21 + operation) < 25) return intersectionRegions[21 + operation].regions;
    else if(region == REGION22 && (22 + operation) >= 0 && (22 + operation) < 25) return intersectionRegions[22 + operation].regions;
    else if(region == REGION23 && (23 + operation) >= 0 && (23 + operation) < 25) return intersectionRegions[23 + operation].regions;
    else if((24 + operation) >= 0 && (24 + operation) < 25) return intersectionRegions[24 + operation].regions;
    else return MapData::getREGIONVector(region);
}
