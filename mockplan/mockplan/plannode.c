#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "plannodes.h"
#include "testability.h"


static void
copy_rem_info_from_path(Plan *dest, Path *src)
{
	dest->startup_cost = src->startup_cost;
	dest->total_cost = src->total_cost;
	dest->rows = src->rows;
	dest->width = src->width;
}

static SeqScan *
make_seqscan(List *qptlist,
			 QualExpr *qpqual,
			 int scanrelid)
{
	SeqScan    *node = (SeqScan*)malloc(sizeof(SeqScan));
	Plan	   *plan;
	if (node == NULL)
	{
		printf("Failed at %s: %d\r\n",__FUNCTION__, __LINE__);
		return NULL;
	}

	plan = &node->plan;
	node->plan.type = T_SeqScan;
	plan->targetlist = qptlist;
	plan->qual = qpqual;
	plan->lefttree = NULL;
	plan->righttree = NULL;
	node->relId = scanrelid;

	return node;
}


SeqScan *
create_seqscan_plan(Path *best_path,
					List *tlist, QualExpr *scan_clauses)
{
	SeqScan    *scan_plan;
	int		scan_relid = best_path->parent->relId;

	scan_plan = make_seqscan(tlist,
							 scan_clauses,
							 scan_relid);

	if (scan_plan != NULL)
		copy_rem_info_from_path(&scan_plan->plan, best_path);

	return scan_plan;
}

static RemoteScan *
create_remotescan(QualExpr *clause,
			 int scanrelid)
{
	RemoteScan    *node = (RemoteScan*)malloc(sizeof(RemoteScan));	
	Plan	   *plan;
	if (node == NULL)
	{
		printf("Failed at %s: %d\r\n",__FUNCTION__, __LINE__);
		return NULL;
	}

	plan = &node->plan;
	node->plan.type = T_RemoteQuery;
	node->relId = scanrelid;


	//plan->targetlist = qptlist;
	plan->qual = clause;
	plan->lefttree = NULL;
	plan->righttree = NULL;

	return node;
}

static RemoteScan *
create_remotequery_plan(Path *best_path,
					QualExpr *clause)
{
	RemoteScan    *scan_plan;
	int			scan_relid = best_path->parent->relId;
	errno_t err = 0;
	scan_plan = create_remotescan(clause,scan_relid);
	if (scan_plan != NULL)
	{
		copy_rem_info_from_path(&scan_plan->plan, best_path);
		err = strcpy_s(scan_plan->destNodeName, MAX_NAME_LENGTH, 
						get_datanode(scan_relid, clause->oper, clause->rarg));
		if (err != 0)
		{			
			printf("Failed at %s: %d\r\n",__FUNCTION__, __LINE__);
			return NULL;
		}
	}
	
	return scan_plan;
}

void show_remotequery(RemoteScan *plan)
{
	printf("Remote Node Scan\r\n");
	printf("\tData Node Scan %s\r\n",plan->destNodeName);
}

void show_seqscan(SeqScan *plan, int tabPos)
{
	RelData *data = get_rel_data_byid(plan->relId);
	char tab[10]={'\0'};

	while(tabPos != 0)
	{
		tab[tabPos - 1] = '\t';
		tabPos--;
	}

	printf("%sSeq Scan on %s (cost %.2f..%.2f rows=%.0f width=%d)\r\n",tab, data->relName, 
		plan->plan.startup_cost, plan->plan.total_cost, plan->plan.rows, plan->plan.width);
	printf("\t%sFilter: (%s %c %s)\r\n",tab, plan->plan.qual->larg, plan->plan.qual->oper, 
						plan->plan.qual->rarg);
}

void show_agg(Agg *plan)
{
	printf("HashAggregate (cost %.2f..%.2f rows=%.0f width=%d)\r\n", 
		plan->plan.startup_cost, plan->plan.total_cost, plan->plan.rows, plan->plan.width);
	printf("\tGroup Key: %s\r\n",plan->grpCol);
	explain_plan((Plan*)plan->plan.lefttree, 2);
}

void explain_plan(Plan *plan, int tabPos)
{
	switch(plan->type)
	{
		case T_RemoteQuery:
			show_remotequery((RemoteScan*)plan);
			break;
		case T_SeqScan:
			show_seqscan((SeqScan*)plan, tabPos);
			break;
		case T_Agg:
			show_agg((Agg*)plan);
			break;
		default:
			printf("Invalid plan type\r\n");
			break;
	}
}

Agg *
make_agg(List *tlist, QualExpr *qual,
		 int numGroupCols, char *grpCol,
		 int dNumGroups, Plan *lefttree)
{
	Agg		   *node = (Agg*)malloc(sizeof(Agg));
	Plan	   *plan;
	errno_t 	err = 0;	
	if (node == NULL)
	{
		printf("Failed at %s: %d\r\n",__FUNCTION__, __LINE__);
		return NULL;
	}
	
	plan = &node->plan;
	node->plan.type = T_Agg;
	node->numCols = numGroupCols;
	err = strcpy_s(node->grpCol, MAX_NAME_LENGTH, grpCol);
	if (err != 0)
	{
		printf("Failed at %s: %d\r\n",__FUNCTION__, __LINE__);
		return NULL;
	}
	
	node->numGroups = dNumGroups;
	plan->qual = qual;
	plan->targetlist = tlist;
	plan->lefttree = lefttree;
	plan->righttree = NULL;

	return node;
}


static Agg*
create_agg_plan(Path *best_path)
{
	Agg		   *plan = NULL;
	Plan	   *subplan = NULL;
	
	subplan = create_plan_recurse(best_path->subpath);
	if (subplan != NULL)
	{
		plan = make_agg(best_path->parent->tlist, best_path->parent->qual, 
						1, best_path->groupCol, 1, subplan);
		if (plan != NULL)
			copy_rem_info_from_path(&plan->plan, (Path *) best_path);
	}
	
	return plan;
}

/*
 * create_plan_recurse
 *	  Recursive guts of create_plan().
 */
Plan *
create_plan_recurse(Path *best_path)
{
	Plan	   *plan = NULL;

	switch (best_path->pathtype)
	{
		case T_SeqScan:		
			plan = (Plan*)create_seqscan_plan(best_path, NULL, best_path->parent->qual);
			break;
		case T_RemoteQuery:
			plan = (Plan*)create_remotequery_plan(best_path, best_path->parent->qual);
			break;
		case T_Agg:
			plan = (Plan *) create_agg_plan(best_path);
			break;
		default:
			printf("Invalid path type\r\n");
			break;
	}

	return plan;
}

void mock_planner(RelInfo *relInfo, char isCN)
{	
	Path *best_path;
	Plan *plan;

	if (isCN)
	{
		best_path = (Path*)create_remotequery_path(relInfo);
	}
	else
	{
		best_path = create_single_rel_pathlist(relInfo);
		if (best_path != NULL)
			best_path = create_agg_path(relInfo, best_path, relInfo->groupCol, 
											relInfo->qual, 1);
	}	

	if (best_path != NULL)
	{
		plan = create_plan_recurse(best_path);
		if (plan != NULL)
			explain_plan(plan, 0);
	}
}

