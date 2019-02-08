#include "path.h"
#include "cost.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Path *
create_remotequery_path(RelInfo *rel)
{
	Path	   *pathnode = (Path*)malloc(sizeof(Path));
	if (pathnode == NULL)
	{
		printf("Failed at %s: %d\r\n",__FUNCTION__, __LINE__);
		return NULL;
	}
	
	pathnode->pathtype = T_RemoteQuery;
	pathnode->parent = rel;

	cost_seqscan(pathnode, rel);

	return pathnode;
}

Path* create_single_rel_pathlist(RelInfo *rel)
{
	return create_seqscan_path(rel);
}

Path *
create_seqscan_path(RelInfo *rel)
{
	Path	   *pathnode = (Path*)malloc(sizeof(Path));
	if (pathnode == NULL)
	{
		printf("Failed at %s: %d\r\n",__FUNCTION__, __LINE__);
		return NULL;
	}

	pathnode->pathtype = T_SeqScan;
	pathnode->parent = rel;
	pathnode->rows = (int)rel->totalTup;
	pathnode->width = (int)rel->width;
	
	cost_seqscan(pathnode, rel);
	
	return pathnode;
}

Path*
create_agg_path(RelInfo *rel,
				Path *subpath,
				char *groupCol,
				QualExpr *qual,
				int numGroups)
{
	Path	   *pathnode = (Path*)malloc(sizeof(Path));
	errno_t err = 0;
	if (pathnode == NULL)
	{		
		printf("Failed at %s: %d\r\n",__FUNCTION__, __LINE__);
		return NULL;
	}

	pathnode->pathtype = T_Agg;
	pathnode->parent = rel;

	pathnode->subpath = subpath;

	pathnode->numGroups = numGroups;
	err = strcpy_s(pathnode->groupCol, MAX_NAME_LENGTH, groupCol);	
	if (err != 0)
	{
		printf("Failed at %s: %d\r\n",__FUNCTION__, __LINE__);
		return NULL;
	}
	
	pathnode->parent->qual = qual; 
	pathnode->width = (int)rel->width;	//actually it should be calculated.

	cost_agg(pathnode,
			 1, numGroups,
			 qual,
			 subpath->startup_cost, subpath->total_cost,
			 subpath->rows);

	return pathnode;
}
