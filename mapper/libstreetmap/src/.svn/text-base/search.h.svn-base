#ifndef SEARCH_H
#define	SEARCH_H

#include <vector>
#include <utility>
#include <unordered_map>
#include "draw.h"
#include "iNode.h"

#define AVERAGE_SPEED_LIMIT 85
typedef std::pair<double,int> Pair;


std::vector<unsigned> searchPath(unsigned intersect_id_start, unsigned intersect_id_end);
//double getCostOfPath (unsigned currentIntersection, std::vector<unsigned> path, unsigned destIntersection);
double getAdditionalPathCost(unsigned prevSegment, unsigned currSegment);
double getHeuristicCost(unsigned currIntersection, unsigned destIntersection);
iNode* getNextNode(iNode* current, int i, unsigned dest);
std::vector<unsigned> getFullPath(unsigned destID, std::unordered_map<unsigned,iNode*> &nodes);
bool path_is_legal(unsigned start_intersection, unsigned end_intersection, const std::vector<unsigned>& path);


#endif	/* SEARCH_H */

