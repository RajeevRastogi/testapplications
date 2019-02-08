#include "plannodes.h"

#define DEFAULT_SEQ_PAGE_COST  1.0
#define DEFAULT_RANDOM_PAGE_COST  4.0
#define DEFAULT_CPU_TUPLE_COST	0.01
#define DEFAULT_CPU_OPERATOR_COST  0.0025


double		seq_page_cost = DEFAULT_SEQ_PAGE_COST;
double		random_page_cost = DEFAULT_RANDOM_PAGE_COST;
double		cpu_tuple_cost = DEFAULT_CPU_TUPLE_COST;
double		cpu_operator_cost = DEFAULT_CPU_OPERATOR_COST;


double get_restriction_qual_cost()
{
	//considering all restriction clause takes the same cost.
	return DEFAULT_CPU_OPERATOR_COST;	
}

double get_tablespace_page_costs()
{
	return DEFAULT_SEQ_PAGE_COST;
}

int length(List *list)
{
	int len = 0;
	while(!list)
	{
		len += 1;
		list = list->next;
	}

	return len;
}

void cost_seqscan(Path *path, RelInfo *baserel)
{
	double		cpu_per_tuple;
	double 		disk_run_cost;
	double 		spc_seq_page_cost;
	double		per_tuple;
	double 		cpu_run_cost;
	double		startup_cost = 0;

	/* fetch estimated page cost for tablespace containing table */
	spc_seq_page_cost = get_tablespace_page_costs();

	/*
	 * disk costs
	 */
	disk_run_cost = spc_seq_page_cost * baserel->npages;

	/* CPU costs */
	per_tuple = get_restriction_qual_cost();

	startup_cost += 0;  
	cpu_per_tuple = cpu_tuple_cost + per_tuple;
	cpu_run_cost = cpu_per_tuple * baserel->totalTup;

	/* target list cost as per cpu cost */	
	cpu_run_cost += cpu_per_tuple*length(baserel->tlist)* path->rows;
	
	path->startup_cost = startup_cost;
	path->total_cost = startup_cost + cpu_run_cost + disk_run_cost;
}

void
cost_agg(Path *path, 
		 int numGroupCols, int numGroups,
		 QualExpr *quals,
		 double input_startup_cost, double input_total_cost,
		 double input_tuples)
{
	double		output_tuples;
	double		startup_cost;
	double		total_cost;

	/* Here we are able to deliver output on-the-fly */
	startup_cost = input_startup_cost;
	total_cost = input_total_cost;
	
	total_cost += (cpu_operator_cost * numGroupCols) * input_tuples;
	total_cost += cpu_tuple_cost * numGroups;
	output_tuples = numGroups;
	
	path->rows = output_tuples;
	path->startup_cost = startup_cost;
	path->total_cost = total_cost;
}


