#ifndef _COST_H
#define _COST_H
#include "path.h"

void cost_seqscan(Path *path, RelInfo *baserel);

void
cost_agg(Path *path, 
		 int numGroupCols, int numGroups,
		 QualExpr *quals,
		 double input_startup_cost, double input_total_cost,
		 double input_tuples);
#endif /*_COST_H*/
