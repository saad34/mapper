#include "draw.h"
#include "drawHelper.h"
#include "m2.h"
#include "graphics.h"
#include <utility>
#include <limits>
#include <math.h>
#include <string>
using namespace std;

void drawStreets(vector<int> streets) {
    
    setlinewidth(0);
    
    t_bound_box vw = get_visible_world();
    double vwWidth = vw.get_width();
    double vwHeight = vw.get_height();
    
    for(int i = 0; i < (int)getNumberOfIntersections(); i++) {
        
        MapData::getIntersectionInfo()[i].drawn = false;
    }
   
    pair<double, bool> streetNameHeight[STREET_NAME_GRID_DIM];
    for(int i = 1; i < STREET_NAME_GRID_DIM; i++) {

        streetNameHeight[i].first = (vwHeight*i/STREET_NAME_GRID_DIM);
        streetNameHeight[i].second = true;
        setcolor(RED);
    }
    
    pair<double, bool> streetNameWidth[STREET_NAME_GRID_DIM];
    for(int i = 1; i < STREET_NAME_GRID_DIM; i++) {

        streetNameWidth[i].first = (vwWidth*i/STREET_NAME_GRID_DIM);
        streetNameWidth[i].second = true;
    }
    
    vector<pair<pair<int,t_point>, double>> streetNames;
    for (int i = 0; i < (int)streets.size(); i++) {
        
        //get the street segments
        vector<unsigned> currentStreetSegments = (MapData::getStreetInfo())[streets[i]].streetSegments;
        
        int numOfStreetNames = 0;
        
        //when writeName is true, it'll tell drawStreetSegment to pass back the necessary information
        int writeName = 0;
        
        //loop through all the street segments
        for(int j = 0; j<STREET_NAME_GRID_DIM; j++) {
            
            streetNameWidth[j].second = true;
            streetNameHeight[j].second = true;
        }
        
        for(auto iter = currentStreetSegments.begin(); iter != currentStreetSegments.end(); iter++) {
            
            StreetSegmentEnds segmentEnds = getStreetSegmentEnds(*iter);
            t_point from = MapData::getIntersectionInfo()[segmentEnds.from].xyCoord;
            t_point to = MapData::getIntersectionInfo()[segmentEnds.to].xyCoord;
            
            if(from.x > to.x) swap(from.x, to.x);
            if(from.y > to.y) swap(from.y, to.y);
            
            bool height = false;
            bool width = false;
            for(int j = 1; j < STREET_NAME_GRID_DIM; j++) {
                
                if(numOfStreetNames < j/3 + 1 && numOfStreetNames < STREET_NUMBER_OF_NAMES) {
                    
                    if(streetNameHeight[j].second &&
                            from.y < vw.bottom() + streetNameHeight[j].first && to.y > vw.bottom() + streetNameHeight[j].first) {
                        height = true;
                        streetNameHeight[j].second = false;
                        
                    }
                    if(streetNameWidth[j].second &&
                            from.x < vw.left() + streetNameWidth[j].first && to.x > vw.left() + streetNameWidth[j].first) {
                        width = true;
                        streetNameWidth[j].second = false;
                    }
                }
            }
            
            if(height && width) writeName = BOTH;
            else if(height) writeName = HEIGHT;
            else if(width) writeName = WIDTH;
            else writeName = NEITHER;
                
            //draw the street segment and if writeName > 1, get the necessary information to draw the street name                
            pair<pair<int,t_point>, double> name = drawStreetSegment(*iter, writeName);
            if(name.first.second.x != -1) {
                streetNames.push_back(name);
                numOfStreetNames++;
            }
        }
    }
        
    //for loop to draw all the desired street names
    setcolor(1); //black
    for(auto iter = streetNames.begin(); iter!= streetNames.end(); iter++) {
        
        //get the information that was passed back from drawStreetSegment  
        pair<pair<int,t_point>, double> info = *iter;
        int ID = info.first.first;
        string name = MapData::getNewStreetNames(ID);
        
        if(name != "(unknown)" && name.find("Southbound") == string::npos && 
            name.find("Westbound") == string::npos && name.find("Ramp") == string::npos) {
            
            std::size_t found = name.find("Northbound");
            if(found != string::npos) {
                
                name.erase(found, string::npos);
            }
            
            found = name.find("Eastbound");
            if(found != string::npos) {
                
                name.erase(found, string::npos);
            }
            
            t_point start = info.first.second;        
            double angle = info.second;

            t_bound_box bounds = MapData::getStreetInfo()[ID].streetBounds;

            //write the name on the same angle as the street
            setcolor(1); //black
            settextrotation(angle);
            setfontsize(STREET_FONT_SIZE);
            drawtext(start, name, bounds);
        }
    }
    
    settextrotation(0);

    return;
}

void drawFeature(unsigned featureID, string attribute) {
        
    unsigned numPoints = getFeaturePointCount(featureID);

    if(attribute == "water") setcolor(177,212,255); //light medium blue
    else if(attribute == "land") setcolor(233,229,220); //light grey
    else if(attribute == "building") setcolor(27); //firebrick (dark red)
    else if(attribute == "grass/field") setcolor(202,223,170); //green
    else if(attribute == "rock") setcolor(2); //dark grey
    else if(attribute == "beach") setcolor(19); //khaki (light yellow)
    else if(attribute == "trees") {
        
        //deal with a tree line separately
        setcolor(13); //dark green
        
        t_point *points = new t_point[numPoints];

        for (int i = 0; i < (int) numPoints; i++) {
            points[i] = MapData::getFeaturePointXY(featureID, i);
            fillarc(points[i], TREE_CIRCLE_SIZE, 0, 360);
        }
        
        for (int i = 0; i < (int) numPoints - 1; i++) {
            drawline(points[i], points[i+1]);
        }

        setcolor(1); //change back to black
        delete [] points;
        return;    
    }
    
    //check if the feature is closed
    if((getFeaturePoint(featureID, 0).lat == getFeaturePoint(featureID, numPoints-1).lat)
        && (getFeaturePoint(featureID, 0).lon == getFeaturePoint(featureID, numPoints-1).lon)) {

        t_point *points = new t_point[numPoints];

        for(int i = 0; i < (int)numPoints; i++) {
            points[i] = MapData::getFeaturePointXY(featureID, i);
        }

        fillpoly(points, numPoints);
        setcolor(1); //change back to black
        delete [] points;
    }

    //draw lines to connect each point instead (not the first and last though)
    else {
        t_point *points = new t_point[numPoints];

        for (int i = 0; i < (int) numPoints; i++) {
            points[i] = MapData::getFeaturePointXY(featureID, i);
        }
        
        for (int i = 0; i < (int) numPoints - 1; i++) {
            drawline(points[i], points[i+1]);
        }

        setcolor(1); //change back to black
        delete [] points;
    }
}

/* Works on the basis of drawing the whole street segment as a polygon 
   (instead of each curve point as a polygon) */
pair<pair<int,t_point>, double> drawStreetSegment(int segmentID, int writeName) {

    double streetWidth = STREET_HALF_WIDTH_RATIO * get_visible_world().get_width();
    setcolor(1); //black

    vector<t_point> side1, side2;
    
    //get ends of segment and sort left and right
    StreetSegmentEnds ends = getStreetSegmentEnds(segmentID);

    t_point from = MapData::getIntersectionInfo()[ends.from].xyCoord;
    t_point to = MapData::getIntersectionInfo()[ends.to].xyCoord;
    
    t_point current = from, next;
    
    //loop through all the curve points and add draw that section
    for (int i = 0; i < (int) getStreetSegmentCurvePointCount(segmentID); i++) {

        //define the end point of the section to draw
        next  = MapData::getSegmentInfo()[segmentID].curvePointXYCoords[i];
        
        //define the cos and sin ratio's of section
        //this will keep the width equal on slanted sections
        double deltaX = (next.x - current.x);
        double deltaY = (next.y - current.y);
        double length = sqrt(deltaX * deltaX + deltaY * deltaY);
        
        if(length == 0) length = 1;
        
        double cosSlope = deltaX / length;
        double sinSlope = deltaY / length;
        
        //create the 4 corners of the street section polygon
        //sin*width is on the x section because the slope  we want for the width is the reciprocal of the actual street
        //think rectangle, edges are 90 degrees to each other, so the slopes of the edges must be reciprocal
        //same goes for y
        
        side1.push_back(t_point(current.x - streetWidth*sinSlope, current.y + streetWidth * cosSlope));
        side1.push_back(t_point(next.x - streetWidth*sinSlope, next.y + streetWidth * cosSlope));
        side2.push_back(t_point(current.x + streetWidth*sinSlope, current.y - streetWidth * cosSlope));
        side2.push_back(t_point(next.x + streetWidth*sinSlope, next.y - streetWidth * cosSlope));

        current = next;
    }

    //draw the last section (or only if there are no curve points)

    //same logic as in the for loop
    double deltaX = (to.x - current.x);
    double deltaY = (to.y - current.y);
    double length = sqrt(deltaX * deltaX + deltaY * deltaY);
    
    if(length == 0) length = 1;
        
    double cosSlope = deltaX / length;
    double sinSlope = deltaY / length;
    
    if(deltaX == 0) deltaX = SMALL_VALUE;
    side1.push_back(t_point(current.x - streetWidth*sinSlope, current.y + streetWidth * cosSlope));
    side1.push_back(t_point(to.x - streetWidth*sinSlope, to.y + streetWidth * cosSlope));
    side2.push_back(t_point(current.x + streetWidth*sinSlope, current.y - streetWidth * cosSlope));
    side2.push_back(t_point(to.x + streetWidth*sinSlope, to.y - streetWidth * cosSlope));
    
    vector<t_point> polygon;
    polygon.insert(polygon.end(), side1.begin(), side1.end());
    polygon.insert(polygon.end(), side2.rbegin(), side2.rend());
    t_point *polyArray = &polygon[0];
    
    //determine if speed limit mode is enabled or not
    //if not, then check for highways
    if(!MapData::getSpeedBool()) {
        
        string streetName = getStreetName(getStreetSegmentStreetID(segmentID));
        
        //set color orange if the street is essentially a highway
        if (streetName.find("Highway") != string::npos) setcolor(5);
        else if (streetName.find("Collectors") != string::npos) setcolor(5);
        else if (streetName.find("Expressway") != string::npos) setcolor(5);
        else if (streetName.find("Express") != string::npos) setcolor(5);
        else if (streetName.find("Freeway") != string::npos) setcolor(5);
        else if (streetName.find("Parkway") != string::npos) setcolor(5);

        else setcolor(0);
    } 
    
    else {
        
        double speedLimit = getStreetSegmentSpeedLimit(segmentID);
        
        if(speedLimit <= 30) setcolor(23); //dark slate blue
        else if(speedLimit > 30 && speedLimit <= 50) setcolor(25); //light medium blue
        else if(speedLimit > 50 && speedLimit <= 70) setcolor(6); //yellow
        else if(speedLimit > 70 && speedLimit <= 90) setcolor(5); //orange
        else if(speedLimit > 90) setcolor(4); //red
    }
    
    setlinewidth(0);
    
    fillpoly(polyArray, polygon.size());
    
    setcolor(3); //dark grey
    current = *side1.begin();
    for(auto iter = side1.begin()+1; iter != side1.end(); iter++) {
        
        next = *iter;
        drawline(current, next);
        current = next;
    }
    
    current = *side2.begin();
    for(auto iter = side2.begin()+1; iter != side2.end(); iter++) {
        
        next = *iter;
        drawline(current, next);
        current = next;
    }
       
    pair<pair<int,t_point>, double> streetName = make_pair(make_pair(-1, t_point(-1,-1)), 0);
    
    if(writeName > NEITHER && getStreetSegmentCurvePointCount(segmentID) < 2) {
 
        int ID = getStreetSegmentStreetID(segmentID);
        string name = getStreetName(ID);
        setcolor(1); //black
        double angle = atan((deltaY/deltaX))/DEG_TO_RAD;
              
        bool draw = false;    
        switch(writeName){
            case(WIDTH):
                if(abs(angle) < 45) draw = true;
                break;
            case(HEIGHT):
                if(abs(angle) > 45) draw = true;
                break;
            case(BOTH):
                draw = true;
                break;
                
        }
        if(draw && !MapData::getIntersectionInfo()[ends.from].drawn && !MapData::getIntersectionInfo()[ends.to].drawn) {
            MapData::getIntersectionInfo()[ends.from].drawn = true;
            MapData::getIntersectionInfo()[ends.to].drawn = true;
            t_point start(from.x + (to.x-from.x)/2, from.y + (to.y-from.y)/2);
            pair<int, t_point> street = make_pair(ID, start);
            streetName = make_pair(street, angle);
        }
    }
        
    return streetName;
}


void drawPointofInterest(unsigned pointOfInterestID) {
    
    if(!(MapData::getPointBool())) return;
    
    t_bound_box currentWorld = get_visible_world();

    t_point coordsXY = MapData::getInterestPoint(pointOfInterestID);

    if (MapData::getPOIClicked() && getPointOfInterestName(MapData::getClickedPOIid()) == getPointOfInterestName(pointOfInterestID)) {
        setcolor(0); //white
        fillarc(coordsXY, POI_OUT_CIR_RAD * currentWorld.get_width(), 0, 360);
        setcolor(4); //red
        fillarc(coordsXY, POI_IN_CIR_RAD * currentWorld.get_width(), 0, 360);
        setcolor(0); //white
        setfontsize(9);
        drawtext(coordsXY, POI_TEXT, currentWorld);
    } else {
        setcolor(1); //black
        fillarc(coordsXY, POI_OUT_CIR_RAD * currentWorld.get_width(), 0, 360);
        setcolor(5); //orange
        fillarc(coordsXY, POI_IN_CIR_RAD * currentWorld.get_width(), 0, 360);
        setcolor(1); //black
        setfontsize(9);
        drawtext(coordsXY, POI_TEXT, currentWorld);
    }
    setcolor(1); //set back to black
}

void showPOIInfo() {
   if(MapData::getPointBool()) {
            t_bound_box currentWorld = get_visible_world();

            double xLeft = currentWorld.left() + INFO_BOX_OFFSET*currentWorld.get_width();
            double yTop = currentWorld.top() - INFO_BOX_OFFSET*currentWorld.get_height();
            double xRight = xLeft + BOX_TO_SCREEN_RATIO_X*currentWorld.get_width();
            double yBottom = yTop - BOX_TO_SCREEN_RATIO_Y*currentWorld.get_height();
            t_bound_box textBox(xLeft, yBottom, xRight, yTop);
            
            if(MapData::getPOIClicked()) {
                setcolor(0);
                fillrect(textBox);
                setcolor(2);
                drawrect(textBox);
                setcolor(1);
                setfontsize(11);
                drawtext_in(textBox, getPointOfInterestName(MapData::getClickedPOIid()));
            }
            else {
                setcolor(0);
                fillrect(textBox);
                setcolor(2);
                drawline(xLeft, yBottom, xRight,yBottom);
                setcolor(2);
                setfontsize(11);
                drawtext_in(textBox, "Select POI");
            }
    }   
}

void drawPathFinding(int segmentID, int color) {
    
    t_color oldColor = getcolor();
    setcolor(color);
    int id1 = getStreetSegmentEnds(segmentID).from;
    int id2 = getStreetSegmentEnds(segmentID).to;
    drawline(MapData::getIntersectionInfo()[id1].xyCoord,
                MapData::getIntersectionInfo()[id2].xyCoord);
    setcolor(oldColor);
    
    return;
}

void drawPath(vector<unsigned> segments) {
    
    setlinewidth(5);
    for(auto iter = segments.begin(); iter != segments.end(); iter++) {
        
        t_point currentPoint, nextPoint, endPoint;
        
        //set currentPoint to the starting intersection
        currentPoint = MapData::getIntersectionInfo()[getStreetSegmentEnds(*iter).from].xyCoord;
        
        //set endPoint to the ending intersection
        endPoint = MapData::getIntersectionInfo()[getStreetSegmentEnds(*iter).to].xyCoord;

        //loop through all the curve points and add the distance from currentPoint to nextPoint
        for (int i = 0; i < (int) getStreetSegmentCurvePointCount(*iter); i++) {
            nextPoint = latLonToCoordinates(getStreetSegmentCurvePoint(*iter, i));
            drawline(currentPoint, nextPoint);
            currentPoint = nextPoint;
        }

        //add the distance from the last curve point (or starting intersection if there are 0 curve points) to the end
        drawline(currentPoint, endPoint);
    }
    
        
//    for(int i = 0; i<5000000000; i++);
    return;
}

void parseMarker() {
    
    //first check if we need to draw anything for directions    
    if(MapData::getDirections() || MapData::getDrawMarkers()) {
    
        vector<unsigned> path = MapData::getPath();
        if(path.size() != 0 && MapData::getDrawMarkers()) drawPath(path);

        //set up textbox information
        t_bound_box currentWorld = get_visible_world();
        double xLeft = currentWorld.left() + INFO_BOX_OFFSET * currentWorld.get_width();
        double yTop = currentWorld.top() - INFO_BOX_OFFSET * currentWorld.get_height();
        double xRight = xLeft + BOX_TO_SCREEN_RATIO_X * currentWorld.get_width();
        double yBottom = yTop - BOX_TO_SCREEN_RATIO_Y * currentWorld.get_height();
        t_bound_box textBox(xLeft, yBottom, xRight, yTop);

        //draw a rectangle in order to show instructions and display what the user is writing
        setcolor(0);
        setlinewidth(0);
        fillrect(textBox);
        setcolor(2);
        drawrect(textBox);
        setcolor(1);

        string from = "";
        if(MapData::getFromMarker().x != -1) from = getIntersectionName(MapData::getFromIntersection());
        string to = "";
        if(MapData::getToMarker().x != -1) to = getIntersectionName(MapData::getToIntersection());

        string first = "";
        string second = "";

        if(path.size() == 0 && MapData::getToMarker().x != -1 && MapData::getFromMarker().x != -1){

            first = "No possible path between";
            second = " and ";
        } else {

            first = "Showing directions from";
            if(MapData::getToMarker().x != -1) second = "to";       
        }

        //explain where the path starts and ends (set bounds as high to not interfere with drawing)
        setfontsize(11);
        drawtext((xLeft + xRight)/2, (yTop + yBottom)/2 + 3*INFO_BOX_OFFSET*currentWorld.get_height(),
                    first, 100, 100);
        drawtext((xLeft + xRight)/2, (yTop + yBottom)/2 + INFO_BOX_OFFSET*currentWorld.get_height(),
                    from, 100, 100);
        drawtext((xLeft + xRight)/2, (yTop + yBottom)/2 - INFO_BOX_OFFSET*currentWorld.get_height(),
                    second, 100, 100);
        drawtext((xLeft + xRight)/2, (yTop + yBottom)/2 - 3*INFO_BOX_OFFSET*currentWorld.get_height(),
                    to, 100, 100);  
    }

    //check if we need to draw the from point yet
    if(MapData::getFromMarker().x != -1) {

        t_point location(MapData::getFromMarker().x, MapData::getFromMarker().y);
        setcolor(7);//green
        drawMarker(location);
    }

    if(MapData::getToMarker().x != -1) {

        t_point location(MapData::getToMarker().x, MapData::getToMarker().y);
        setcolor(4);//red
        drawMarker(location);
    }
    if(MapData::getDirectionPOI() != "") {
        t_point location = latLonToCoordinates(getIntersectionPosition(MapData::getDirectionPOIIntersectionID()));
        centerTheMap(location);
        setcolor(5); //orange
        drawMarker(location);
    }
        
}    

void parseMarker(t_point searchLocation) {
    
    if(MapData::getSearchPOI() || MapData::getSearchIntersections() ||
            MapData::getSearchStreets()) {

        centerTheMap(searchLocation);
        setcolor(5); //orange
        drawMarker(searchLocation);
    }
    
}

void drawMarker(t_point location) {
    
    t_bound_box vw = get_visible_world();
    double vwWidth = vw.get_width();
    double markerOffset = MARKER_OFFSET*vwWidth;
    double outerRadius = OUTER_CIRCLE_RADIUS*vwWidth;
    double innerRadius = INNER_CIRCLE_RADIUS*vwWidth;
    t_point circleCentre(location.x + markerOffset/30, location.y + markerOffset);
    t_point triangle[3] = {location, t_point(location.x - outerRadius, location.y + markerOffset), t_point(location.x + outerRadius, location.y + markerOffset)};
    
    fillpoly(triangle, 3);
    fillarc(circleCentre, outerRadius, 0, 180);
    setlinewidth(2);
    drawline(t_point(location.x - outerRadius, location.y + markerOffset), t_point(location.x + outerRadius, location.y + markerOffset));
    setcolor(1); //black
    fillarc(circleCentre, innerRadius, 0, 360);
    drawline(location, t_point(location.x - outerRadius, location.y + 11*markerOffset/10));
    drawline(location, t_point(location.x + outerRadius, location.y + 11*markerOffset/10));
    drawarc(circleCentre.x, circleCentre.y, outerRadius, 0, 180);
    setlinewidth(0);
}

void drawText(string message) {
    t_bound_box currentWorld = get_visible_world();

    double xLeft = currentWorld.left() + INFO_BOX_OFFSET*currentWorld.get_width();
    double yTop = currentWorld.top() - INFO_BOX_OFFSET*currentWorld.get_height();
    double xRight = xLeft + BOX_TO_SCREEN_RATIO_X*currentWorld.get_width();
    double yBottom = yTop - BOX_TO_SCREEN_RATIO_Y*currentWorld.get_height();
    t_bound_box textBox(xLeft, yBottom, xRight, yTop);
    
    setcolor(0);
    fillrect(textBox);
    setcolor(2);
    drawrect(textBox);
    setcolor(1);
    setfontsize(11);
    drawtext_in(textBox, message);
    
    return;
}


void drawCircles(vector<unsigned> circles, int kind) {
    
    if(kind == 0) setcolor(GREEN);
    else if(kind == 1) setcolor(BLUE);
    else setcolor(RED);
    
    for(auto iter = circles.begin(); iter != circles.end(); iter++) {
        
        fillarc(MapData::getIntersectionInfo()[*iter].xyCoord, 0.5, 0, 360);
    }

}