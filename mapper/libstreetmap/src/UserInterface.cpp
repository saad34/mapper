#include "UserInterface.h"
#include <cfloat>

#define PI 3.141592653589793238462643383279502884197169399


using namespace std;

void getDirections() {

    setlinewidth(0);

    if (!(MapData::getDirections())) return;

    //set up textbox information
    t_bound_box currentWorld = get_visible_world();
    double xLeft = currentWorld.left() + INFO_BOX_OFFSET * currentWorld.get_width();
    double yTop = currentWorld.top() - INFO_BOX_OFFSET * currentWorld.get_height();
    double xRight = xLeft + BOX_TO_SCREEN_RATIO_X * currentWorld.get_width();
    double yBottom = yTop - BOX_TO_SCREEN_RATIO_Y * currentWorld.get_height();
    t_bound_box textBox(xLeft, yBottom, xRight, yTop);

    //draw a rectangle in order to show instructions and display what the user is writing
    setcolor(0);
    fillrect(textBox);
    setcolor(2);
    drawrect(textBox);
    setcolor(1);

    //begin writing instructions (set bounds as high to not interfere with drawing)
    setfontsize(11);
    drawtext((xLeft + xRight) / 2, (yTop + yBottom) / 2 + INFO_BOX_OFFSET * currentWorld.get_height(),
            "Please select where you would like", 100, 100);
    drawtext((xLeft + xRight) / 2, (yTop + yBottom) / 2 - INFO_BOX_OFFSET * currentWorld.get_height(),
            "to receive directions from and to.", 100, 100);
}

void calculateDirections() {

    vector<unsigned> path = find_path_between_intersections(MapData::getFromIntersection(),
            MapData::getToIntersection());

    drawPath(path);
    parseMarker(); //redoing it here so markers draw over path
    setlinewidth(0);

    //set up textbox information
    t_bound_box currentWorld = get_visible_world();
    double xLeft = currentWorld.left() + INFO_BOX_OFFSET * currentWorld.get_width();
    double yTop = currentWorld.top() - INFO_BOX_OFFSET * currentWorld.get_height();
    double xRight = xLeft + BOX_TO_SCREEN_RATIO_X * currentWorld.get_width();
    double yBottom = yTop - BOX_TO_SCREEN_RATIO_Y * currentWorld.get_height();
    t_bound_box textBox(xLeft, yBottom, xRight, yTop);

    //draw a rectangle in order to show instructions and display what the user is writing
    setcolor(0);
    fillrect(textBox);
    setcolor(2);
    drawrect(textBox);
    setcolor(1);

    string from = getIntersectionName(MapData::getFromIntersection());
    string to = getIntersectionName(MapData::getToIntersection());

    string first;
    string second;

    if (path.size() == 0) {

        first = "No possible path between";
        second = " and ";
    } else {

        first = "Showing directions from";
        second = "to";
    }

    //explain where the path starts and ends (set bounds as high to not interfere with drawing)
    setfontsize(11);
    drawtext((xLeft + xRight) / 2, (yTop + yBottom) / 2 + 3 * INFO_BOX_OFFSET * currentWorld.get_height(),
            first, 100, 100);
    drawtext((xLeft + xRight) / 2, (yTop + yBottom) / 2 + INFO_BOX_OFFSET * currentWorld.get_height(),
            from, 100, 100);
    drawtext((xLeft + xRight) / 2, (yTop + yBottom) / 2 - INFO_BOX_OFFSET * currentWorld.get_height(),
            second, 100, 100);
    drawtext((xLeft + xRight) / 2, (yTop + yBottom) / 2 - 3 * INFO_BOX_OFFSET * currentWorld.get_height(),
            to, 100, 100);

    MapData::setPath(path);
    MapData::setDirections(false);
}

void calculateDirectionsPOI(string poiName) {
    
    vector<unsigned> path = find_path_to_point_of_interest(MapData::getFromIntersection(), poiName);

    drawPath(path);
    parseMarker(); //redoing it here so markers draw over path
    setlinewidth(0);

    //set up textbox information
    t_bound_box currentWorld = get_visible_world();
    double xLeft = currentWorld.left() + INFO_BOX_OFFSET * currentWorld.get_width();
    double yTop = currentWorld.top() - INFO_BOX_OFFSET * currentWorld.get_height();
    double xRight = xLeft + BOX_TO_SCREEN_RATIO_X * currentWorld.get_width();
    double yBottom = yTop - BOX_TO_SCREEN_RATIO_Y * currentWorld.get_height();
    t_bound_box textBox(xLeft, yBottom, xRight, yTop);

    //draw a rectangle in order to show instructions and display what the user is writing
    setcolor(0);
    fillrect(textBox);
    setcolor(2);
    drawrect(textBox);
    setcolor(1);

    string from = getIntersectionName(MapData::getFromIntersection());
    string to = poiName;

    string first;
    string second;

    if (path.size() == 0) {

        first = "No possible path between";
        second = " and ";
    } else {

        first = "Showing directions from";
        second = "to";
    }

    //explain where the path starts and ends (set bounds as high to not interfere with drawing)
    setfontsize(11);
    drawtext((xLeft + xRight) / 2, (yTop + yBottom) / 2 + 3 * INFO_BOX_OFFSET * currentWorld.get_height(),
            first, 100, 100);
    drawtext((xLeft + xRight) / 2, (yTop + yBottom) / 2 + INFO_BOX_OFFSET * currentWorld.get_height(),
            from, 100, 100);
    drawtext((xLeft + xRight) / 2, (yTop + yBottom) / 2 - INFO_BOX_OFFSET * currentWorld.get_height(),
            second, 100, 100);
    drawtext((xLeft + xRight) / 2, (yTop + yBottom) / 2 - 3 * INFO_BOX_OFFSET * currentWorld.get_height(),
            to, 100, 100);

    MapData::setPath(path);
    MapData::setDirections(false);
}

void searchParser() {

    //set up textbox information
    t_bound_box currentWorld = get_visible_world();
    double xLeft = currentWorld.left() + INFO_BOX_OFFSET * currentWorld.get_width();
    double yTop = currentWorld.top() - INFO_BOX_OFFSET * currentWorld.get_height();
    double xRight = xLeft + BOX_TO_SCREEN_RATIO_X * currentWorld.get_width();
    double yBottom = yTop - BOX_TO_SCREEN_RATIO_Y * currentWorld.get_height();
    t_bound_box textBox(xLeft, yBottom, xRight, yTop);

    //draw a rectangle in order to show instructions and display what the user is writing
    setcolor(0);
    fillrect(textBox);
    setcolor(2);
    drawrect(textBox);
    setcolor(1);

    //give directions to the user and begin doing autocomplete
    setfontsize(11);

    //setup based on what the user is searching for (i.e. reset iterators, string, and count)
    //make iterators initially point to start and one past the end of the appropriate list
    //these get closer together when a character is typed or farther apart when backspace is pressed
    if (MapData::getSearchPOI()) {

        MapData::getStartSearch() = MapData::getPOIList().begin();
        MapData::getEndSearch() = MapData::getPOIList().end();
        MapData::getPOISearch() = "";
        MapData::getSearchCount() = 0;
        drawtext((xLeft + xRight) / 2, (yTop + yBottom) / 2 + INFO_BOX_OFFSET * currentWorld.get_height(),
                "Begin typing the Point of Interest you want", 100, 100);
    }

    if (MapData::getSearchIntersections()) {

        MapData::getStartSearch() = MapData::getIntersectionsList().begin();
        MapData::getEndSearch() = MapData::getIntersectionsList().end();
        MapData::getIntersectionSearch() = "";
        MapData::getSearchCount() = 0;
        drawtext((xLeft + xRight) / 2, (yTop + yBottom) / 2 + INFO_BOX_OFFSET * currentWorld.get_height(),
                "Begin typing the Intersection you want", 100, 100);
    }

    if (MapData::getSearchStreets()) {

        MapData::getStartSearch() = MapData::getStreetsList().begin();
        MapData::getEndSearch() = MapData::getStreetsList().end();
        MapData::getStreetSearch() = "";
        MapData::getSearchCount() = 0;
        drawtext((xLeft + xRight) / 2, (yTop + yBottom) / 2 + INFO_BOX_OFFSET * currentWorld.get_height(),
                "Begin typing the Street you want", 100, 100);
    }
}

void search(t_bound_box currentWorld, double xLeft, double xRight, double yTop,
        double yBottom) {

    int displaySearch = 0; //number of options currently listed (max 5)
    unsigned numberOf; //number of POIS, Intersections, or Streets, depending on which bool is on

    setcolor(3); //light grey
    fillrect(xLeft, yTop - 3 * BOX_TO_SCREEN_RATIO_Y * currentWorld.get_height(), xRight, yBottom);
    setcolor(1);
    drawrect(xLeft, yTop - 3 * BOX_TO_SCREEN_RATIO_Y * currentWorld.get_height(), xRight, yBottom);

    int searchIndex = MapData::getSearchCount() - 1;
    char searchChar;

    //must get the right char depending on what we're searching for
    //index in at specific characters to reduce searching time
    if (MapData::getSearchPOI()) searchChar = (MapData::getPOISearch())[searchIndex];
    else if (MapData::getSearchIntersections()) searchChar = (MapData::getIntersectionSearch())[searchIndex];
    else searchChar = (MapData::getStreetSearch())[searchIndex];
    
    
    //avoid overlaps
    if(MapData::getEndSearch() == MapData::getStartSearch()) {
     
        MapData::getEndSearch() = MapData::getStartSearch() + 1;
    }  

    bool startFound = false;
    for (auto iter = MapData::getStartSearch(); iter != MapData::getEndSearch(); iter++) {

        char c = (*iter)[searchIndex];
        
        //convert upper case to lower case for searching purposes
        //the list was initially sorted to be insensitive to case
        if (c >= 'A' && c <= 'Z') c = c - 'A' + 'a';
        
        if (c == searchChar && !startFound) {

            MapData::getStartSearch() = iter;
            startFound = true;
        }

        if (c > searchChar) {

            MapData::getEndSearch() = iter;
            break;
        }
    }
    
    //avoid overlaps
    if(!startFound) MapData::getEndSearch() = MapData::getStartSearch() + 1;
   
    
    if (MapData::getSearchPOI()) numberOf = getNumberOfPointsOfInterest();
    else if (MapData::getSearchIntersections()) numberOf = getNumberOfIntersections();
    else numberOf = getNumberOfStreets();
    
    string display1, display2, display3, display4, display5; //used to check for doubles

    //only want to display options if there is at least one matching string
    if (startFound) {
        for (auto iter = MapData::getStartSearch(); (iter != MapData::getEndSearch()) && (displaySearch < 5); iter++) {

            unsigned ID;
            if(MapData::getSearchPOI()) {

                auto POI_ID = MapData::getPOIHT().find(*iter);
                if(displaySearch < (int) (POI_ID->second).size()) ID = (POI_ID->second)[displaySearch];
                else ID = (POI_ID->second)[0];
            }
            else if(MapData::getSearchIntersections()) {
                
                auto intersectionID = MapData::getIntersectionsHT().find(*iter);
                ID = (intersectionID->second);
            }
            else {
             
                //for streets, bring user to "from" intersection of first street segment
                auto streetID = MapData::getStreetsHT().find(*iter);
                unsigned firstStreetSegmentID = *(MapData::getStreetInfo()[streetID->second].streetSegments.begin());
                ID = getStreetSegmentEnds(firstStreetSegmentID).from; //first intersection
            }

            
            if (displaySearch == 0 && numberOf >= 1) {

                display1 = *iter;
                
                drawtext((xLeft + xRight) / 2 - INFO_BOX_OFFSET * currentWorld.get_width(),
                        (yTop + yBottom) / 2 - SUGGESTION_BOX_OFFSET * currentWorld.get_height(),
                        "1. " + display1, xRight - xLeft, 100);
                
                MapData::setOption(ID, 1);
                displaySearch++;
            }
            else if ((displaySearch == 1) && (numberOf >= 2) && (*iter != display1)) {

                display2 = *iter;
                
                drawtext((xLeft + xRight) / 2 - INFO_BOX_OFFSET * currentWorld.get_width(),
                        (yTop + yBottom) / 2 - 1.5 * SUGGESTION_BOX_OFFSET * currentWorld.get_height(),
                        "2. " + display2, xRight - xLeft, 100);
                
                MapData::setOption(ID, 2);
                displaySearch++;
            }
            else if ((displaySearch == 2) && (numberOf >= 3) && (*iter != display1) &&
                        (*iter != display2)) {

                display3 = *iter;
                
                drawtext((xLeft + xRight) / 2 - INFO_BOX_OFFSET * currentWorld.get_width(),
                        (yTop + yBottom) / 2 - 2 * SUGGESTION_BOX_OFFSET * currentWorld.get_height(),
                        "3. " + display3, xRight - xLeft, 100);
                
                MapData::setOption(ID, 3);
                displaySearch++;
            }
            else if ((displaySearch == 3) && (numberOf >= 4) && (*iter != display1) &&
                        (*iter != display2) && (*iter != display3)) {

                display4 = *iter;
                
                drawtext((xLeft + xRight) / 2 - INFO_BOX_OFFSET * currentWorld.get_width(),
                        (yTop + yBottom) / 2 - 2.5 * SUGGESTION_BOX_OFFSET * currentWorld.get_height(),
                        "4. " + display4, xRight - xLeft, 100);
                
                MapData::setOption(ID, 4);
                displaySearch++;
            }
            else if ((displaySearch == 4) && (numberOf >= 5) && (*iter != display1) &&
                        (*iter != display2) && (*iter != display3) && (*iter != display4)) {

                display5 = *iter;
                
                drawtext((xLeft + xRight) / 2 - INFO_BOX_OFFSET * currentWorld.get_width(),
                        (yTop + yBottom) / 2 - 3 * SUGGESTION_BOX_OFFSET * currentWorld.get_height(),
                        "5. " + display5, xRight - xLeft, 100);
                
                MapData::setOption(ID, 5);
                displaySearch++;
            }
        }
    }    
}

int searchOffset(t_bound_box currentWorld, double xLeft, double xRight, double yTop,
        double yBottom, char c) {
    
    t_bound_box textBound(xLeft, yBottom, xRight, yTop);
    setcolor(WHITE);
    fillrect(xLeft + 0.1 * INFO_BOX_OFFSET * currentWorld.get_width(), 
            (yTop + yBottom)/2 - 3*INFO_BOX_OFFSET * currentWorld.get_height(), 
            xRight - 0.1 * INFO_BOX_OFFSET * currentWorld.get_width(), 
            (yTop + yBottom)/2 - INFO_BOX_OFFSET * currentWorld.get_height());
    
    //backspace
    if(c == '\b') {

        setcolor(0); 

        //initial check to not decrement past zero
        if (MapData::getSearchCount() <= 0) {
            
            //call search just to clear the grey search box of any prior options listed
            search(currentWorld, xLeft, xRight, yTop, yBottom); 
            return 0;
        }

        MapData::getSearchCount() -= 1;
        int searchIndex = MapData::getSearchCount() - 1;
        

        if (MapData::getSearchPOI()) {

            string copy = MapData::getPOISearch();
            MapData::getPOISearch() = "";
            MapData::getSearchCount() = 0;
            
            //reset iterators
            MapData::getStartSearch() = MapData::getPOIList().begin();
            MapData::getEndSearch() = MapData::getPOIList().end();
            
            
            for(int i = 0; i <= searchIndex; i++) {
                
                MapData::getPOISearch() += copy[i];
                MapData::getSearchCount()++;
                search(currentWorld, xLeft, xRight, yTop, yBottom);
            }  
            
            
            drawtext(t_point((xLeft + xRight) / 2,
                    (yTop + yBottom) / 2 - 2 * INFO_BOX_OFFSET * currentWorld.get_height()),
                    MapData::getPOISearch(), textBound);
            
            return 0;
        }
        
        else if (MapData::getSearchIntersections()) {

            string copy = MapData::getIntersectionSearch();
            MapData::getIntersectionSearch() = "";
            MapData::getSearchCount() = 0;
            
            //reset iterators
            MapData::getStartSearch() = MapData::getIntersectionsList().begin();
            MapData::getEndSearch() = MapData::getIntersectionsList().end(); 
            
            
            for(int i = 0; i <= searchIndex; i++) {
                
                MapData::getIntersectionSearch() += copy[i];
                MapData::getSearchCount()++;
                search(currentWorld, xLeft, xRight, yTop, yBottom);              
            }
            
            
            drawtext(t_point((xLeft + xRight) / 2,
                    (yTop + yBottom) / 2 - 2 * INFO_BOX_OFFSET * currentWorld.get_height()),
                    MapData::getIntersectionSearch(), textBound);
            
            return 0;
        }
        
        else {
                
            string copy = MapData::getStreetSearch();
            MapData::getStreetSearch() = "";
            MapData::getSearchCount() = 0;
            
            //reset iterators
            MapData::getStartSearch() = MapData::getStreetsList().begin();
            MapData::getEndSearch() = MapData::getStreetsList().end(); 
            
            
            for(int i = 0; i <= searchIndex; i++) {
                
                MapData::getStreetSearch() += copy[i];
                MapData::getSearchCount()++;
                search(currentWorld, xLeft, xRight, yTop, yBottom);
            }

                
            drawtext(t_point((xLeft + xRight) / 2,
                    (yTop + yBottom) / 2 - 2 * INFO_BOX_OFFSET * currentWorld.get_height()),
                    MapData::getStreetSearch(), textBound);
            
            return 0;
        }
    }
    

    //check the appropriate search to see if " and " is in it -> replace with " & "
    if (c == ' ') {
        
        if (MapData::getSearchPOI() && (MapData::getPOISearch()).find(" and") != string::npos) {
                
            MapData::getSearchCount() -= 3;
            int searchIndex = MapData::getSearchCount() - 1;
            
            string copy = MapData::getPOISearch();
            MapData::getPOISearch() = "";
            MapData::getSearchCount() = 0;
            
            //reset iterators
            MapData::getStartSearch() = MapData::getPOIList().begin();
            MapData::getEndSearch() = MapData::getPOIList().end();
            
            
            for(int i = 0; i <= searchIndex; i++) {
                
                MapData::getPOISearch() += copy[i];
                MapData::getSearchCount()++;
                search(currentWorld, xLeft, xRight, yTop, yBottom);
            }
            
            
            MapData::getPOISearch() += '&';
            MapData::getSearchCount()++;
            search(currentWorld, xLeft, xRight, yTop, yBottom);
            
            MapData::getPOISearch() += ' ';
            MapData::getSearchCount()++;
            search(currentWorld, xLeft, xRight, yTop, yBottom);          
            
            
            drawtext(t_point((xLeft + xRight) / 2,
                    (yTop + yBottom) / 2 - 2 * INFO_BOX_OFFSET * currentWorld.get_height()),
                    MapData::getPOISearch(), textBound);
            
            return 0;
        }
        
        else if (MapData::getSearchIntersections() && 
                (MapData::getIntersectionSearch()).find(" and") != string::npos) {
                
            MapData::getSearchCount() -= 3;
            int searchIndex = MapData::getSearchCount() - 1; 

            string copy = MapData::getIntersectionSearch();
            MapData::getIntersectionSearch() = "";
            MapData::getSearchCount() = 0;
            
            //reset iterators
            MapData::getStartSearch() = MapData::getIntersectionsList().begin();
            MapData::getEndSearch() = MapData::getIntersectionsList().end(); 
            
            
            for(int i = 0; i <= searchIndex; i++) {
                
                MapData::getIntersectionSearch() += copy[i];
                MapData::getSearchCount()++;
                search(currentWorld, xLeft, xRight, yTop, yBottom);              
            }
            
            
            MapData::getIntersectionSearch() += '&';
            MapData::getSearchCount() ++;
            search(currentWorld, xLeft, xRight, yTop, yBottom);
            
            MapData::getIntersectionSearch() += ' ';
            MapData::getSearchCount()++;
            search(currentWorld, xLeft, xRight, yTop, yBottom);
            
            
            drawtext(t_point((xLeft + xRight) / 2,
                    (yTop + yBottom) / 2 - 2 * INFO_BOX_OFFSET * currentWorld.get_height()),
                    MapData::getIntersectionSearch(), textBound);
            
            return 0; 
        }
        
        else if ((MapData::getStreetSearch()).find(" and") != string::npos) {
                
            MapData::getSearchCount() -= 3;
            int searchIndex = MapData::getSearchCount() - 1;
            
            string copy = MapData::getStreetSearch();
            MapData::getStreetSearch() = "";
            MapData::getSearchCount() = 0;
            
            //reset iterators
            MapData::getStartSearch() = MapData::getStreetsList().begin();
            MapData::getEndSearch() = MapData::getStreetsList().end(); 
            
            
            for(int i = 0; i <= searchIndex; i++) {
                
                MapData::getStreetSearch() += copy[i];
                MapData::getSearchCount()++;
                search(currentWorld, xLeft, xRight, yTop, yBottom);
            }
            
            
            MapData::getStreetSearch() += '&';
            MapData::getSearchCount() ++;
            search(currentWorld, xLeft, xRight, yTop, yBottom);
            
            MapData::getStreetSearch() += ' ';
            MapData::getSearchCount()++;
            search(currentWorld, xLeft, xRight, yTop, yBottom);

                
            drawtext(t_point((xLeft + xRight) / 2,
                    (yTop + yBottom) / 2 - 2 * INFO_BOX_OFFSET * currentWorld.get_height()),
                    MapData::getStreetSearch(), textBound);
            
            return 0;     
        }
    }
    
    
    //the following 5 statements deal with a specific (listed) option that the user wants
    /*13 is the ASCII value for the return key:
           - user wants to search what they typed in
           - due to the sorting of the vectors at the start, option1 should contain their string
             (if it exists), otherwise option1 is still the closest name 
           - thus always return option1 when the user presses enter */
    if ((c == '1') || ( c == 13)) return 1;
    if (c == '2') return 2;
    if (c == '3') return 3;
    if (c == '4') return 4;
    if (c == '5') return 5;
    
    return -1;
}

void centerTheMap(t_point coords) {

    t_bound_box currentWorld = get_visible_world();
    double xLeft = coords.x - currentWorld.get_width() / 2;
    double xRight = coords.x + currentWorld.get_width() / 2;
    double yTop = coords.y + currentWorld.get_height() / 2;
    double yBottom = coords.y - currentWorld.get_height() / 2;
    
    t_bound_box world(xLeft, yBottom, xRight, yTop);
    set_world(world);
    drawscreen();
}

void printDirections(vector<unsigned> path) {
    
    auto iter = path.begin();
    if(path.empty()) cout << "You're already there" << endl;
    else {
        cout << "Directions: " << endl;

        unsigned currStreetID = getStreetSegmentStreetID(*iter);
        unsigned nextStreetID = 0;

        unsigned startIntID = MapData::getFromIntersection();
        unsigned currentIntID;
        unsigned nextIntID;


        if(startIntID == getStreetSegmentEnds(*iter).from) currentIntID = getStreetSegmentEnds(*iter).to;
        else currentIntID = getStreetSegmentEnds(*iter).from;


        t_point startPos = latLonToCoordinates(getIntersectionPosition(startIntID));
        t_point currentPos = latLonToCoordinates(getIntersectionPosition(currentIntID));

        double deltaX = (currentPos.x - startPos.x);
        double deltaY = (currentPos.y - startPos.y);
        double length = sqrt(deltaX * deltaX + deltaY * deltaY);

        if(length == 0) length = 0.001;

        if(deltaX == 0) deltaX = 0.001;

        if(deltaY == 0) deltaY = 0.001;

        t_point prevVec((deltaX/length),(deltaY/length));

        double distance = find_street_segment_length(*iter);

        iter++;

        while(iter != path.end()) {

            nextStreetID = getStreetSegmentStreetID(*iter);

            if(currentIntID == getStreetSegmentEnds(*iter).from) nextIntID = getStreetSegmentEnds(*iter).to;
            else nextIntID = getStreetSegmentEnds(*iter).from;

            distance += find_street_segment_length(*iter);

            t_point nextPos = latLonToCoordinates(getIntersectionPosition(nextIntID));

            deltaX = (nextPos.x - currentPos.x);
            deltaY = (nextPos.y - currentPos.y);
            length = sqrt(deltaX * deltaX + deltaY * deltaY);



            if(length == 0) length = 0.001;

            if(deltaX == 0) deltaX = 0.001;

            if(deltaY == 0) deltaY = 0.001;

            t_point nextVec((deltaX/length),(deltaY/length));

            double angle = atan2(nextVec.y,nextVec.x) - atan2(prevVec.y,prevVec.x);

            if(angle < -PI) angle += 2*PI;
            if(angle > PI) angle -= 2*PI;


            string leftOrRight;        
            if(angle >  0) leftOrRight = "left";
            else if (angle < 0 ) leftOrRight = "right";
            else leftOrRight = "straight";

            if(currStreetID != nextStreetID) {

                cout << "Drive down " << getStreetName(currStreetID) << " for " 
                        << (int)distance << "m" << endl;
                distance = 0;


                cout << "Turn " << leftOrRight << " onto " << getStreetName(nextStreetID) << endl;
            }
            currStreetID = nextStreetID;
            currentIntID = nextIntID;
            prevVec.x = nextVec.x;
            prevVec.y = nextVec.y;
            currentPos.x = nextPos.x;
            currentPos.y = nextPos.y;

            iter++;
        }
        cout << "Drive down " << getStreetName(nextStreetID) << " for " << (int)distance << "m" << endl;

        cout << "Destination reached" << endl;
    }
}
