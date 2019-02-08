#ifndef TESTABILITY_H
#define TESTABILITY_H

typedef struct relData
{
	char relName[32];
	int relId;
	int rowCount;
	int npages;
	int width; 
}RelData;


typedef struct dataDist
{
	char nodeName[32];
	int relId;
	char range[100];
}DataDist;

RelData *get_rel_data_byid(int relId);
RelData *get_rel_data_byname(char *tblName);
char *get_datanode(int relId, char oper, char *dataEq);
char* get_datanode_gt(int relId, char *dataEq);
char* get_datanode_lteq(int relId, char *dataEq);

#endif /* TESTABILITY*/
