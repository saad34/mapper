#ifndef SEARCHNEARESTPOI_H
#define	SEARCHNEARESTPOI_H

#include <vector>
#include <utility>
#include <unordered_map>
#include "draw.h"
#include "iNode.h"
#include "search.h"

#define AVERAGE_SPEED_LIMIT 85
typedef std::pair<double,int> Pair;


std::vector<unsigned> searchPOIPath(unsigned intersect_id_start, std::vector<unsigned> poi_intersection_ids);
iNode* getNextNodePOISearch(iNode* current, int i);


#endif	/* SEARCHNEARESTPOI_H */

