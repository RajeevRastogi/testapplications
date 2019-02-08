#ifndef PLANNODES_H
#define PLANNODES_H

#include "plannodes.h"
#include "path.h"

typedef struct Plan
{
	NodeType		type;

	/* cost for this plan*/
	double		startup_cost;  /*cost for initial set-up work*/
	double		total_cost;	   /*total cost for whole execution*/	 

	/* expected numbers of rows and width*/
	double		rows;
	int			width;
	
	/* common data needed by each plan*/
	List	   *targetlist;
	QualExpr   *qual;			/*where clause*/
	struct Plan *lefttree;
	struct Plan *righttree;
}Plan;

typedef struct seqScanNode
{
	Plan plan;
	int relId;
}SeqScan;

typedef struct AggNode
{
	Plan plan;
	int  numCols;
	int numGroups;
	char grpCol[MAX_NAME_LENGTH];
}Agg;

typedef struct RemoteScan
{
	Plan plan;
	// additional detail
	int relId;
	char destNodeName[MAX_NAME_LENGTH];
}RemoteScan;

void mock_planner(RelInfo *relInfo, char isCN);
static RemoteScan *create_remotequery_plan(Path *best_path, QualExpr *clause);
Plan *create_plan_recurse(Path *best_path);
void explain_plan(Plan *plan, int tabPos);
Agg *make_agg(List *tlist, QualExpr *qual,
		 int numGroupCols, char *grpCol,
		 int dNumGroups, Plan *lefttree);


#endif /*PLANNODES_H*/
