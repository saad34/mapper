#include "searchNearestPOI.h"
#include "search.h"
#include "iNode.h"
#include "StreetsDatabaseAPI.h"
#include "m3.h"
#include "m1.h"
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

//comparator - necessary for heap

class compare2 {
public:

    bool operator()(iNode* lhs, iNode* rhs) {

        return lhs->getPathCost() > rhs->getPathCost();
    }
};

/* NOTE
 * To fix optimality (current best guess):
 * when you get to the destination, save the best path score (pointer to the best final node)
 * keep looping through remainder of nodes in wavefront*/ 

typedef priority_queue<iNode*, std::vector<iNode*>, compare2> myHeap;
vector<unsigned> searchPOIPath(unsigned intersect_id_start, vector<unsigned> poi_intersection_ids) {

    /*LOGIC
     * initilize wavefront and visitedNodes.
     * Add start node to both
     * LOOP:
     * Pop lowest cost path off the heap
     * If this is the end node, find path, free all memory, return
     * else
     * Check to see if we've been to this node before
     * If we haven't visited it, add it to the visited list 
     * If we have, check if the new path is better or worse
     * If the path is better, update the node's path.
     * If haven't visited it, or the path is better, add all out intersections to wavefront
     * loop
     */
    iNode* startNode = new iNode(intersect_id_start, -1, -1, 0, 0);

    //set up cost set, indicating what comparator to use
    //I can probably change this to a heap now
    myHeap waveFront;
    waveFront.push(startNode);
    unordered_set<unsigned> poiIntersectionIDs(poi_intersection_ids.begin(), poi_intersection_ids.end());

    //set up visitedNodes hash table (unordered_map). 
    //First object is the id of the node, second is a pair of cost and segment that got them there    
    unordered_map<unsigned, iNode*> visitedNodes;

    //while there's still possibilities to find the destination
    while (!waveFront.empty()) {
        
        //get at the smallest value of the set, then remove it from wavefront
        bool deleteCurrent = false;
        iNode* current = waveFront.top();
        waveFront.pop();

        auto jter = visitedNodes.find((current->getId()));

        if (jter == visitedNodes.end()) {

            visitedNodes.insert(make_pair(current->getId(), current));


            //loop through all the paths you can take from the previous nodes (all the out segments)
            double numSegments = getIntersectionStreetSegmentCount(current->getId());
            for (int i = 0; i < numSegments; i++) {

                iNode* newNode = getNextNodePOISearch(current, i);
                //if the node is valid (i.e. it's not a one way going the wrong way)
                if (newNode != NULL) {
                    waveFront.push(newNode);
                }
            }
        } 
        
        else {

            iNode* existingNode = (*jter).second;

            //if new path to this node is better than the old path
            //delete the old path, and create the new path in the set
            //update the best values into the set
            if (current->getPathCost() < existingNode->getPathCost()) {

                //delete old path in wavefront, and add new
                //get all the nodes in waveFront that have the same cost
                delete (*jter).second;
                (*jter).second = current;
                
                //loop through all the paths you can take from the previous nodes (all the out segments)
                double numSegments = getIntersectionStreetSegmentCount(current->getId());
                for (int i = 0; i < numSegments; i++) {

                    iNode* newNode = getNextNodePOISearch(current, i);
                    //if the node is valid (i.e. it's not a one way going the wrong way)
                    if (newNode != NULL) {
                        waveFront.push(newNode);
                    }
                }
            } else deleteCurrent = true;
        }
        
        //reached destination
        if(poiIntersectionIDs.find(current->getId()) != poiIntersectionIDs.end()) {
            
           // cout<<"best int: "<<current->getId()<<endl;
            MapData::setDirectionPOIIntersectionID(current->getId());
            //get the full path and return
            vector<unsigned> path = getFullPath(current->getId(), visitedNodes);

            //free memory
            for (unordered_map<unsigned, iNode*>::iterator kter = visitedNodes.begin();
                    kter != visitedNodes.end();
                    kter++) {
                delete (*kter).second;
            }

            while (!waveFront.empty()) {
                iNode* toDelete = waveFront.top();
                waveFront.pop();
                delete toDelete;
            }

            return path;
        }

        if (deleteCurrent) delete current;

    }

    //if it gets here, it means no path was found :(
    for (unordered_map<unsigned, iNode*>::iterator kter = visitedNodes.begin();
            kter != visitedNodes.end();
            kter++) {
        delete (*kter).second;
    }

    while (!waveFront.empty()) {
        iNode* toDelete = waveFront.top();
        waveFront.pop();
        delete toDelete;
    }
    vector<unsigned> noPathFound;
    return noPathFound;
}

//gets the next node along a certain path

iNode* getNextNodePOISearch(iNode* current, int i) {

    unsigned currentID = current->getId();

    //get id of the segment we will follow to the next node
    unsigned currSegment = getIntersectionStreetSegment(currentID, i);

    //if it's a one way street that you cannot travel, return a dummy node
    if (getStreetSegmentOneWay(currSegment) && currentID != getStreetSegmentEnds(currSegment).from) return NULL;
    
    //get the next intersection id (i.e. nextNodeID)
    unsigned nextNodeID;
    StreetSegmentEnds adjIntersections = getStreetSegmentEnds(currSegment);

    if (adjIntersections.from == adjIntersections.to) return NULL;
    else if (currentID == adjIntersections.to) nextNodeID = adjIntersections.from;
    else nextNodeID = adjIntersections.to;

    //calculate the cost of this new path
    double pathCost = current->getPathCost() + getAdditionalPathCost(current->getPreviousSeg(), currSegment);

    //calculate the total cost of this path
    iNode* nextNode = new iNode(nextNodeID, currSegment, currentID, pathCost, 0);
    
    return nextNode;
}