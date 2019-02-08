#include "testability.h"
#include<string.h>

RelData relDatas[]=
{
	{"tbl1", 100, 99, 5, 10}
};


DataDist relDatasDist[]=
{	
	{"node1", 100, "abc"},
	{"node2", 100, "light"},
	{"node3", 100, "z"}
};

RelData *get_rel_data_byid(int relId)
{
	int size = sizeof(relDatas)/sizeof(relDatas[0]);
	int i;
	for (i = 0; i < size; i++)
	{
		if (relDatas[i].relId == relId)
		{
			return &(relDatas[i]);
		}
	}

	return NULL;
}

RelData *get_rel_data_byname(char *tblName)
{
	int size = sizeof(relDatas)/sizeof(relDatas[0]);
	int i;
	for (i = 0; i < size; i++)
	{
		if (!strcmp(relDatas[i].relName, tblName))
		{
			return &(relDatas[i]);
		}
	}

	return NULL;
}

char* get_datanode_lteq(int relId, char *dataEq)
{
	int size = sizeof(relDatasDist)/sizeof(relDatasDist[0]);
	int i;
	for (i = 0; i < size; i++)
	{
		if (relDatasDist[i].relId == relId && strcmp(dataEq, relDatasDist[i].range) <= 0)
		{
			return relDatasDist[i].nodeName;
		}
	}

	return "unknown";
}

char* get_datanode_gt(int relId, char *dataEq)
{
	int size = sizeof(relDatasDist)/sizeof(relDatasDist[0]);
	int i;
	int lastIdx = -1;
	for (i = 0; i < size; i++)
	{
		if (relDatasDist[i].relId == relId && strcmp(dataEq, relDatasDist[i].range) > 0)
		{
			lastIdx = i;
		}
		else if (lastIdx != -1)
		{
			return relDatasDist[i].nodeName;
		}
			
	}

	return "unknown";
}

char *get_datanode(int relId, char oper, char *dataEq)
{
	if (oper == '<' || oper == '=')
	{
		return get_datanode_lteq(relId, dataEq);
	}
	else
	{
		return get_datanode_gt(relId, dataEq);
	}

	//should not come here.
	return NULL;
}
