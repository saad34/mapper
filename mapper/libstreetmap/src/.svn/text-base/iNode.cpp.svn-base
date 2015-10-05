#include "iNode.h"
#include "MapData.h"
#include "IntersectionGraph.h"
#include <vector>


using namespace std;

iNode::iNode(unsigned id, int _previousSeg, int _previousInt, double _pathCost, double _totCost) {

    intersectionID = id;
    pathCost = _pathCost;
    totCost = _totCost;
    previousSegment = _previousSeg;
    previousIntersection = _previousInt;
}

iNode::~iNode() {
    
}

//getters
unsigned iNode::getId() const{ return intersectionID; }
double iNode::getPathCost() const{ return pathCost; }
double iNode::getTotCost() const{ return totCost; }
int iNode::getPreviousSeg() const{ return previousSegment; }
int iNode::getPreviousInt() const{ return previousIntersection; }

//setters
void iNode::setCost(double newCost) { totCost = newCost; }




