#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "plannodes.h"
#include "testability.h"

/*
 * example to pass arguments
 * isCN, relname, selectivity, targetname, qual op, qual left, qual right, groupcolname
 * e.g.
 * 0, "tbl1", 50, "product_id", "=", "cust_id", "literal", "product_id"
 */
int main(int argc, char *argv[])
{
	QualExpr qual;
	List tlist;
	RelInfo relInfo;
	RelData *rel;
	errno_t err = 0;
	if (argc < 9)
	{
		printf("please enter arguments in the format"
			" isCN, relname, selectivity, targetname," 
			" qual op, qual left, qual right, groupcolname");
		return -1;
	}
	
	rel = get_rel_data_byname(argv[2]);
	if (rel == NULL)
	{
		printf("Relation not found\r\n");
		return -1;
	}
	
	// Form the rel info to be passed to parse. Ideally this come from analyzer but here
	// we are taking as application input.
	relInfo.relId = rel->relId;
	relInfo.totalTup = rel->rowCount;
	relInfo.estimTup = atoi(argv[3])/100; // can be replace this with actual selectivity calculation.
	relInfo.width = rel->width;
	relInfo.npages = rel->npages;
	relInfo.qual = &qual;
	relInfo.tlist = &tlist;

	err = strcpy_s(tlist.name, MAX_NAME_LENGTH, argv[4]);	
	if (err != 0)
	{
		printf("Failed at %s: %d\r\n",__FUNCTION__, __LINE__);
		return -1;
	}
	qual.oper = argv[5][0];
	err = strcpy_s(qual.larg, MAX_NAME_LENGTH, argv[6]);
	if (err != 0)
	{
		printf("Failed at %s: %d\r\n",__FUNCTION__, __LINE__);
		return -1;
	}
	err = strcpy_s(qual.rarg, MAX_VALUE_LENGTH, argv[7]);
	if (err != 0)
	{
		printf("Failed at %s: %d\r\n",__FUNCTION__, __LINE__);
		return -1;
	}
	err = strcpy_s(relInfo.groupCol, MAX_NAME_LENGTH, argv[8]);	
	if (err != 0)
	{
		printf("Failed at %s: %d\r\n",__FUNCTION__, __LINE__);
		return -1;
	}

	mock_planner(&relInfo, atoi(argv[1]));
	return 0;
}
