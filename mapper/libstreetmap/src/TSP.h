#ifndef TSP_H
#define	TSP_H

#include <vector>
#include <utility>
#include <unordered_map>
#include "draw.h"
#include "iNode.h"
#include "search.h"
#include <queue>

#define AVERAGE_SPEED_LIMIT 85
typedef std::pair<double,int> Pair;



typedef struct path {

    double time;
    unsigned startIntersection;
    unsigned endIntersection;
    std::vector<unsigned> path;
    std::vector<unsigned> pathInt;
    
} pathSection;

pathSection searchNext(unsigned intersect_id_start, std::vector<unsigned> &delivery_intersection_ids, int color, std::vector<pathSection> &sections, int index, std::unordered_map<unsigned,pathSection>& visitedIntersection, bool recurse, int optimizeAttempts);
iNode* getNextNodeTSP(iNode* current, int i, std::vector<unsigned> &destIDs, int color);
double heuristics(unsigned segID, unsigned currentIntID, std::vector<unsigned> &destIDs);
std::vector<unsigned> getFullPathIntersections(unsigned finalID, std::unordered_map<unsigned, iNode*> &nodes);
void optimizeUncross(pathSection sectionOld, pathSection sectionNew, std::vector<pathSection> &pathSections, std::unordered_map<unsigned,pathSection> &visitedIntersections, int optimizeAttempts);



#endif	/* TSP_H */

