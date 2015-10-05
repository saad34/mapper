#ifndef INODE_H
#define	INODE_H

#include <vector>

//intersections Nodes
class iNode {
private:
  
    //id of current intersection
    unsigned intersectionID;
    double pathCost;
    //cost of path = length of path to here + heuristic values (e.g. magnitude of distance to dest)
    //best paths have lowest cost
    double totCost;
    //ids of all segments taken to get to this node
    int previousSegment;
    int previousIntersection;

public:
    
    //constructor
    iNode(unsigned id, int _previousSeg, int _previousInt, double _pathCost, double _totCost);
    //destructor
    ~iNode();

    //getters
    unsigned getId() const;
    double getPathCost() const;
    double getTotCost() const;
    int getPreviousSeg() const;
    int getPreviousInt() const;
    
    //setters
    void setCost(double newCost);
    
};

#endif	/* INODE_H */

