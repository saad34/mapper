#include <float.h>

#include "m4.h"
#include "TSP.h"
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <cfloat>
#include <bits/stl_vector.h>
#include <algorithm>
#include "search.h"
#include <utility>
#include "MapData.h"

using namespace std;

unordered_multimap<unsigned,pathSection> pathsTried;
typedef unordered_multimap<unsigned,pathSection>::iterator ummIter;

vector<unsigned> traveling_salesman(vector<unsigned> intersections_to_traverse, vector<unsigned> depot_locations) {
    
    //    unordered_map<unsigned, double> segHT;
    //    map<double, unsigned> depotCosts;
    unordered_map<unsigned,pathSection> visitedIntersection;
    vector<unsigned> bestDepots = depot_locations;
    vector<pathSection> pathSections;
    int colorCount = 5;
    int optimizeAttempts = 0;

    //    cout<<"start"<<endl;

    // run through every depot twice
    //    auto iter = bestDepots.begin();  
    //    while (count < 1) {

    vector<unsigned> unvisitedDrops = intersections_to_traverse;
    auto iter = unvisitedDrops.begin();
    unsigned startID = (*iter);
    unsigned currentIntID = startID;
    unvisitedDrops.erase(iter);
    bool pathExists = true;
    while (!unvisitedDrops.empty()) {
//            setcolor(BLUE);
//        cout<<unvisitedDrops.size()<<endl;
        //            cout<<currentIntID<<
        pathSection current = searchNext(currentIntID, unvisitedDrops, colorCount, pathSections, -1, visitedIntersection, true, 0);
        currentIntID = current.endIntersection;
        pathsTried.insert(make_pair(current.startIntersection, current));
        if (currentIntID == getNumberOfIntersections() + 1) {

            pathExists = false;
            break;
        }
        // the following three lines can be made into 1. Current format is for debugging. single line method follows
         unvisitedDrops.erase(find(unvisitedDrops.begin(), unvisitedDrops.end(), currentIntID));
//        vector<unsigned>::iterator jter = find(unvisitedDrops.begin(), unvisitedDrops.end(), currentIntID);
//        if (jter == unvisitedDrops.end()) cout << "error - arrived at an delivery point that doesn't exist" << endl;
//        else unvisitedDrops.erase(jter);

        colorCount++;
        if(colorCount > LIMEGREEN) colorCount = 5;
    }

    if (pathExists && currentIntID != startID) {
        unvisitedDrops.push_back(startID);
        pathSection newPath = searchNext(currentIntID, unvisitedDrops, colorCount, pathSections, -1,visitedIntersection, false, 0);
        pathsTried.insert(make_pair(newPath.startIntersection, newPath));
        //        searchNext(currentIntID, depot_locations, useHeuristics, segHT, pathCost, currentPath, colorCount);       
    }
    
    if(pathSections.size() == 0){
        vector<unsigned> smallPath;
        if(intersections_to_traverse.size() == 0) {
            return smallPath;
        }
        else {
            double bestTime = DBL_MAX;
            vector<unsigned> bestPath;
            for(auto iter = depot_locations.begin(); iter != depot_locations.end(); iter++) {
                smallPath = searchPath(*iter, intersections_to_traverse.front());
                double newTime = compute_path_travel_time(smallPath);
                if(newTime < bestTime) {
                    bestTime = newTime;
                    bestPath = smallPath;
                }
            }
            vector<pathSection> holder;
            pathSection newSection = searchNext(intersections_to_traverse.front(), depot_locations, colorCount, holder, -2, visitedIntersection, false, 0);
            bestPath.insert(bestPath.end(), newSection.path.begin(), newSection.path.end());
            return bestPath;
        }
    }
    
    double pathCost = 0;
    for (auto iter = pathSections.begin(); iter != pathSections.end(); iter++) pathCost += iter->time;
    optimizeK_Opt(intersections_to_traverse,pathCost,pathSections);
    //-------------------
    int index;
    double bestDepotCost = DBL_MAX;
    vector<unsigned> depot_ids;
    vector<pathSection> bestPathSections;
    
    for(int i = 1; i < 6 && i < (int) pathSections.size(); i++) {
        vector<pathSection> trialPathSections = pathSections;
        pathSection worstPath = getNthWorstSection(trialPathSections, index, i, true);

        vector<unsigned> startNode;
        pathSection bestDepotPathSection;
        //vector<unsigned> startDepot;
        startNode.push_back(worstPath.endIntersection);
        
        double bestDepotPathTime = DBL_MAX;
        for (auto jter = depot_locations.begin(); jter != depot_locations.end(); jter++) {

            pathSection depotSection = searchNext((*jter), startNode, colorCount, trialPathSections, -2, visitedIntersection, false, 0);

            if (depotSection.time < bestDepotPathTime) {
                bestDepotPathTime = depotSection.time;
                bestDepotPathSection = depotSection;
            }
        }

        trialPathSections.insert(trialPathSections.begin(), bestDepotPathSection);

        searchNext(worstPath.startIntersection, depot_locations, colorCount, trialPathSections, -1, visitedIntersection, false, 0);
        
        double depotCost = 0;
        for (auto iter = trialPathSections.begin(); iter != trialPathSections.end(); iter++) {
            depotCost += iter->time;
        }
        if(depotCost < bestDepotCost) {
            bestDepotCost = depotCost;
            bestPathSections = trialPathSections;
        }

    }
    pathSections = bestPathSections;
    
    pathCost = 0;
    for (auto iter = pathSections.begin(); iter != pathSections.end(); iter++) pathCost += iter->time;
    optimizePathTrySecond(intersections_to_traverse,pathCost,pathSections);
        
    vector<unsigned> finalPath;
    for (auto iter = pathSections.begin(); iter != pathSections.end(); iter++) {

        finalPath.insert(finalPath.end(), iter->path.begin(), iter->path.end());
    }


//    depot_ids.push_back(pathSections.front().startIntersection);
//    depot_ids.push_back(pathSections.back().endIntersection);
    
//    drawCircles(depot_ids, 2);
    
    setcolor(RED);
    drawPath(finalPath);

    return finalPath;
}

void optimizePathTrySecond(vector<unsigned> &intersections_to_traverse, double &pathCost, vector<pathSection> &pathSections)  {
    
    bool notDone = true;
    while(notDone){
        int count = 0;
        for(auto iter = pathSections.begin(); iter != pathSections.end(); iter++) {
            notDone = false;
            
            vector<pathSection> trialPathSectionOrder;
            trialPathSectionOrder.insert(trialPathSectionOrder.begin(), pathSections.begin(), pathSections.end());
            auto currIter = trialPathSectionOrder.begin();
            for(; currIter != trialPathSectionOrder.end(); currIter++) {

                if(iter->startIntersection == currIter->startIntersection &&
                        iter->endIntersection == currIter->endIntersection) {
                    
                    break;
                }
            }
            vector<unsigned> allOtherInt;
            unsigned originalEnd = currIter->endIntersection;
            unsigned originalStart = currIter->startIntersection;
            for(auto jter = intersections_to_traverse.begin(); jter != intersections_to_traverse.end(); jter++) {

                if(*jter != originalEnd && *jter != originalStart) allOtherInt.push_back(*jter);
            }
            trialPathSectionOrder.erase(currIter);
            unsigned newNext;
            
            
            pathSection newNextSection;
            pathSection oldToNewNext;
            pathSection oldFromNewNext;
            
            unordered_map<unsigned,pathSection> visitedInts;
            
            if(!allOtherInt.empty()) {                
                newNextSection = searchNext(originalStart,allOtherInt,4,trialPathSectionOrder,0,visitedInts,false,0);
                
                pathsTried.insert(make_pair(newNextSection.startIntersection, newNextSection));
                
                newNext = newNextSection.endIntersection;
                trialPathSectionOrder.erase(trialPathSectionOrder.begin());
            }
            bool validAttemptFrom = false;
            bool validAttemptTo = false;
            vector<pathSection>::iterator toDelete;
            for(auto jter = trialPathSectionOrder.begin(); jter != trialPathSectionOrder.end(); jter++) {
                if(jter->startIntersection == newNext) {
                    validAttemptFrom = true;
                    oldFromNewNext = *jter;
                    trialPathSectionOrder.erase(jter);
                    break;
                }
            }
            for(auto jter = trialPathSectionOrder.begin(); jter != trialPathSectionOrder.end(); jter++) {
                if(jter->endIntersection == newNext) {
                    validAttemptTo = true;
                    oldToNewNext = *jter;
                    trialPathSectionOrder.erase(jter);
                    break;
                }
            }
            if(validAttemptFrom && validAttemptTo) {
                
                vector<pathSection> toInsert;
                
                toInsert.push_back(newNextSection);
                
                vector<unsigned> originalEnding;
                originalEnding.push_back(originalEnd);
                
                bool stillLooking = true;
                pair<ummIter,ummIter> haveTried = pathsTried.equal_range(newNext);
                for(;haveTried.first != haveTried.second; haveTried.first++) {
                    if(haveTried.first->second.endIntersection == originalEnd) {
                        stillLooking = false;
                        toInsert.push_back(haveTried.first->second);
                    }
                }
                
                if(stillLooking){
                    pathSection newNextSection = searchNext(newNext,originalEnding,4,toInsert,-1,visitedInts,false,0);
                    pathsTried.insert(make_pair(newNextSection.startIntersection, newNextSection));
                }
                
                
                vector<unsigned> fromOldToToOldFrom;
                fromOldToToOldFrom.push_back(oldFromNewNext.endIntersection);
                
                stillLooking = true;
                pair<ummIter,ummIter> haveTried2 = pathsTried.equal_range(oldToNewNext.startIntersection);
                for(;haveTried2.first != haveTried2.second; haveTried2.first++) {
                    if(haveTried2.first->second.endIntersection == oldFromNewNext.endIntersection) {
                        stillLooking = false;
                        toInsert.push_back(haveTried2.first->second);
                    }
                }
                if(stillLooking) {
                    pathSection newNextSection = searchNext(oldToNewNext.startIntersection,fromOldToToOldFrom,4,toInsert,-1,visitedInts,false,0);
                    pathsTried.insert(make_pair(newNextSection.startIntersection, newNextSection));
                }
                
                unsigned end = pathSections.back().endIntersection;
                unsigned prevEnd = pathSections.front().startIntersection;
                int i = 0;

                do{
                    unsigned currStart = getNumberOfIntersections() + 1;
                    if(i < trialPathSectionOrder.size())
                        currStart = trialPathSectionOrder.at(i).startIntersection;

                    if(currStart != prevEnd) {
                        for(auto jter = toInsert.begin(); jter != toInsert.end(); jter++) {
                            if(jter->startIntersection == prevEnd){
                                auto kter = trialPathSectionOrder.begin();
                                for(int j = 0; j < i; j++) kter++;
                                trialPathSectionOrder.insert(kter, *jter);
                            }
                        }
                    }
                    prevEnd = trialPathSectionOrder.at(i).endIntersection;
                    i++;
                }while(prevEnd != end);

                                
                double trialPathCost = 0;
    
                for (auto jter = trialPathSectionOrder.begin(); jter != trialPathSectionOrder.end(); jter++) {
                    
                    trialPathCost += jter->time;
                }
                

                if(trialPathCost < pathCost) {
//                    cout<<trialPathCost<<endl;
                    pathSections = trialPathSectionOrder;
                    pathCost = trialPathCost;
                    notDone = true;
                    break;
                }
            }
            count++;
        }
    }
}



void optimizeK_Opt(vector<unsigned> &intersections_to_traverse, double &pathCost, vector<pathSection> &pathSections)  {
    unordered_map<unsigned,pathSection> filler;
    bool notDone = true;
    while(notDone){
        int count = 0;
        for(auto iter = pathSections.begin(); iter != pathSections.end(); iter++) {
//            cout<<count<<endl;
            notDone = false;
            bool valid = false;
            vector<pathSection> trialPathSectionOrder;
            trialPathSectionOrder.insert(trialPathSectionOrder.begin(), pathSections.begin(), pathSections.end());
            auto currIter = trialPathSectionOrder.begin();
            for(; currIter != trialPathSectionOrder.end(); currIter++) {

                if(iter->startIntersection == currIter->startIntersection &&
                        iter->endIntersection == currIter->endIntersection) {
                    
                    break;
                }
            }
            
            vector<unsigned> toFind;
            vector<pathSection> toInsert;
            auto nextIter = currIter + 1;
            int num = 1;
            //find direction vector of path
//            t_point from = MapData::getIntersectionInfo()[iter->startIntersection].xyCoord;
//            t_point to = MapData::getIntersectionInfo()[iter->endIntersection].xyCoord;    
//            
//            double deltaX = (to.x - from.x);
//            double deltaY = (to.y - from.y);
//            
//            double length = sqrt(deltaX*deltaX + deltaY*deltaY);
//            
//            if(length == 0) length = 1;
//            
//            deltaX = deltaX/length;
//            deltaY = deltaY/length;

            for(; nextIter != trialPathSectionOrder.end() && num < ceil(200/pathSections.size()); nextIter++, num++) {
                
                //find direction vector to next nodes
//                t_point nextFrom = MapData::getIntersectionInfo()[nextIter->startIntersection].xyCoord;
//                t_point nextTo = MapData::getIntersectionInfo()[nextIter->endIntersection].xyCoord; 
//                
//                double nextDeltaX = (nextTo.x - nextFrom.x);
//                double nextDeltaY = (nextTo.y - nextFrom.y);
//
//                double nextLength = sqrt(nextDeltaX*nextDeltaX + nextDeltaY*nextDeltaY);
//
//                if(nextLength == 0) nextLength = 1;
//
//                nextDeltaX = nextDeltaX/nextLength;
//                nextDeltaY = nextDeltaY/nextLength;
//                
//                if(abs(nextDeltaX - deltaX) > 1.1 || abs(nextDeltaY - deltaY) > 1.1) {
//                    cout<<"big change"<<endl;
                    //switch 2 segments
                    pathSection new1;
                    bool stillLooking = true;
                    pair<ummIter,ummIter> haveTried = pathsTried.equal_range(currIter->startIntersection);
                    for(;haveTried.first != haveTried.second; haveTried.first++) {
                        if(haveTried.first->second.endIntersection == nextIter->startIntersection) {
                            stillLooking = false;
                            new1 = haveTried.first->second;
                        }
                    }
                    if(stillLooking) {
                        toFind.push_back(nextIter->startIntersection);
                        new1 = searchNext(currIter->startIntersection, toFind, 4, trialPathSectionOrder,-2,filler,false,0);
                        pathsTried.insert(make_pair(new1.startIntersection, new1));
                    }
                    
                    toFind.clear();
                    
                    pathSection new2;
                    stillLooking = true;
                    pair<ummIter,ummIter> haveTried2 = pathsTried.equal_range(currIter->endIntersection);
                    for(;haveTried2.first != haveTried2.second; haveTried2.first++) {
                        if(haveTried2.first->second.endIntersection == nextIter->endIntersection) {
                            stillLooking = false;
                            new2 = haveTried2.first->second;
                        }
                    }
                    if(stillLooking) {
                        toFind.push_back(nextIter->endIntersection);
                        new2 = searchNext(currIter->endIntersection, toFind, 4, trialPathSectionOrder,-2,filler,false,0);
                        pathsTried.insert(make_pair(new2.startIntersection, new2));
                    }
                    toFind.clear();
                    //if it's better, check to see if it's legal
                    if(new1.time + new2.time < currIter->time + nextIter->time) {
                        valid = true;
                        //delete old paths
    //                    setcolor(RED);
    //                    drawPath(nextIter->path);
    //                    drawPath(currIter->path);
    //                    setcolor(BLUE);
    //                    for(auto jter = toInsert.begin(); jter!= toInsert.end();jter++) {
    //                        drawPath(jter->path);
    //                    }
                        for(auto kter = currIter; kter + 1 != nextIter && kter != trialPathSectionOrder.end(); kter++) {
    //                        cout<<kter->endIntersection<<endl;
                            toFind.push_back(kter->endIntersection);
                        }
                        trialPathSectionOrder.erase(currIter,nextIter + 1);
    //                    cout<<currIter->time<<endl;
                        trialPathSectionOrder.insert(currIter, new1);
                        unsigned startID = new1.endIntersection;
                        int j = 1;
                        while(!toFind.empty()){
                            vector<unsigned> findNext;
                            findNext.push_back(toFind.back());
                            toFind.pop_back();
                            
                            stillLooking = true;
                            pair<ummIter,ummIter> haveTried3 = pathsTried.equal_range(startID);
                            for(;haveTried3.first != haveTried3.second; haveTried3.first++) {
                                if(haveTried3.first->second.endIntersection == findNext.front()) {
                                    stillLooking = false;
                                    auto kter = trialPathSectionOrder.begin();
                                    for(int k = 0; k < count + j; k++) kter++;
                                    trialPathSectionOrder.insert(kter, haveTried3.first->second);
                                }
                            }
                            if(stillLooking) {
                            
                                pathSection next = searchNext(startID, findNext, 4, trialPathSectionOrder,count+j,filler,false,0);
                                pathsTried.insert(make_pair(next.startIntersection, next));
                            }
                            
                            unsigned nextID = findNext.front();
                            startID = nextID;
                            j++;
                        }
                        trialPathSectionOrder.insert(nextIter, new2);
                        double trialCost = 0;
                        for (auto kter = trialPathSectionOrder.begin(); kter != trialPathSectionOrder.end(); kter++) {

                            trialCost += kter->time;
    //                        cout<<"start "<<kter->startIntersection<<" - end "<<kter->endIntersection<<endl;
                        }

    //                    cout<<trialCost<<endl;
                        if(trialCost < pathCost) {
    //                        cout<<trialPathSectionOrder.size()<<endl;
                            pathSections.clear();
                            pathSections.insert(pathSections.begin(),trialPathSectionOrder.begin(),trialPathSectionOrder.end());
                            pathCost = trialCost;
                            notDone = true;
                        } else valid = false;

                        break;


    //                    valid = true;
    //                    
    //                    unsigned end = pathSections.back().endIntersection;
    //                    unsigned prevEnd = pathSections.front().startIntersection;
    //                    int i = 0;
    //                    do{
    //                        unsigned currStart = getNumberOfIntersections() + 1;
    //                        if(i < trialPathSectionOrder.size())
    //                            currStart = trialPathSectionOrder.at(i).startIntersection;
    //                        
    //                        if(currStart != prevEnd) {
    //                            cout<<"1"<<endl;
    //                            bool foundNew = false;
    //                            for(auto jter = toInsert.begin(); jter != toInsert.end(); jter++) {
    //                                if(jter->startIntersection == prevEnd){
    //                                    cout<<"2"<<endl;
    //                                    foundNew = true;
    //                                    auto kter = trialPathSectionOrder.begin();
    //                                    for(int j = 0; j < i; j++) kter++;
    //                                    trialPathSectionOrder.insert(kter, *jter);
    //                                }
    //                            }
    //                            if(!foundNew) {
    //                                cout<<"3"<<endl;
    //                                valid = false;
    //                                break;
    //                            }
    //                        }
    //                        prevEnd = trialPathSectionOrder.at(i).endIntersection;
    //    //                    cout<<"prevEnd "<<prevEnd<<endl;
    //                        i++;
    //                    }while(prevEnd != end && valid);
    //                    
    //                    if(i != pathSections.size()) {
    //                        cout<<"correct "<<pathSections.size()<<" - actual "<<i<<endl;
    //                        valid = false;
    //                    }
                    }
//                }
            }
        if(valid) break;
        count++;
        }
    }
}
    
    

//returns path section with greatest cost and removes it from the list

pathSection getNthWorstSection(vector<pathSection> &pathSections, int &index, unsigned n, bool reorganize) {
    map<double, int> orderedSections;
//    cout<<"SIZE: "<<pathSections.size()<<" n: "<<n<<endl;
    vector<pathSection>::iterator worst = pathSections.begin();
    int count = 0;
    
    for (auto iter = pathSections.begin(); iter != pathSections.end(); iter++) {

        orderedSections.insert(make_pair((-1)*(iter->time), count));
//        cout<<"cost - actual order "<<iter->time<<endl;
        count++;
    }


//    for (auto iter = orderedSections.begin(); iter != orderedSections.end(); iter++) {
//
//        cout<<"cost - actual order "<<iter->first<<endl;
//    }
    
    for (int i = 0; i < (int) n - 1; i++) {
//        cout<<i<<" n: "<<n<<" size: "<<orderedSections.size()<<endl;
        orderedSections.erase(orderedSections.begin());
        
    }

    
    index = orderedSections.begin()->second;

//    cout<<"index "<<index<<endl;
    for (int i = 0; i<index; i++) {
        worst++;
        if(worst == pathSections.end()){
        }
    }
    pathSection worstSection = *worst;
    
    if(reorganize){
        vector<pathSection> newOrder;
        vector<pathSection>::iterator newOrderIter = worst + 1;
        if(newOrderIter == pathSections.end()) newOrderIter = pathSections.begin();
        while(newOrderIter != worst) {

            newOrder.push_back(*newOrderIter);
            newOrderIter++;
            if(newOrderIter == pathSections.end()) newOrderIter = pathSections.begin();
        }
    
        pathSections = newOrder;
    }else {
        pathSections.erase(worst);
    }
    return worstSection;
}

