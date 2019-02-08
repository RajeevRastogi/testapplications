#ifndef PATH_H
#define PATH_H

#define MAX_NAME_LENGTH  64	
#define MAX_VALUE_LENGTH 100

typedef enum nodeType
{
	T_SeqScan			= 0,
	T_RemoteQuery		= 1,
	T_Agg				= 2,
	T_INVALID			= 501
}NodeType;

typedef struct qualExpr	
{
	char oper;
	char larg[MAX_NAME_LENGTH];
	char rarg[MAX_VALUE_LENGTH];	
}QualExpr;

typedef struct list
{
	char name[MAX_NAME_LENGTH];
	/* we can add more info here.*/
	struct list *next;
}List;


typedef struct relInfo
{
	int relId;			// containing relation
	int npages;
	double totalTup;	// Total number of tuples
	double estimTup;	// as per the plan estimation 
	double width;
	QualExpr *qual;
	List *tlist;	
	char groupCol[MAX_NAME_LENGTH];
}RelInfo;

typedef struct path
{
	NodeType	pathtype;		/* tag identifying scan/join method */

	RelInfo *parent;			/* the relation this path can build */

	/* estimated size/costs for path (see costsize.c for more info) */
	double		rows;			/* estimated number of result tuples */
	int			width;			/* tuple width */
	double		startup_cost;	/* cost expended before fetching any tuples */
	double		total_cost;		/* total cost (assuming all tuples fetched) */	
	char		groupCol[MAX_NAME_LENGTH];
	int			numGroups;
	struct path	*subpath;
} Path;

Path *create_remotequery_path(RelInfo *rel);
Path* create_single_rel_pathlist(RelInfo *rel);
Path *create_seqscan_path(RelInfo *rel);
Path* create_agg_path(RelInfo *rel,Path *subpath,
				char *groupCol, QualExpr *qual, int numGroups);
#endif
