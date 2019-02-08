========================================================================
    Mock Plan: Overview
========================================================================
Problem Statement:
Implement a mock query optimizer that generates an exec plan for a partitioned
/sharded database. Assume data is sharded by CUST_ID which is part of 
incoming predicate.  The mock optimizer should accept the incoming SQL query "
SELECT product_id, sum(sales) from T WHERE cust_id='literal' group by 
product_id" and it should produce an execution plan that just looks like an 
explain plan. I would like to see scans, predicate and aggregate pushdowns 
shown on the plan.  Anything that is not implemented should be discussed in 
the design document. 

Solution Implementation:
This is the mock planner, by no means close to actual planner. Here I 
have covered only basic parts to depict concept of planner and it planner works. 
Only following basic functionality implemented:
1. Only one kind of scan (i.e. SeqScan)
2. Only one kind of aggregation (Hash Aggregate)
3. Selectivity value is hard-coded instead of calculating.
4. Focus was only on planner so no parsing/analyze phase covered.
5. User needs to pass argument in the below format using which get passed to 
   planner.
    isCN, relname, selectivity, targetname, qual op, qual left, qual right, groupcolname

 	e.g. to execute on CN (master node)
 	0, "tbl1", 50, "product_id", "=", "cust_id", "literal", "product_id" -- 

 	e.g. to execute on other data node
 	0, "tbl1", 50, "product_id", "=", "cust_id", "literal", "product_id" 
 	
6.  Any table name can be passed but corresponding update should be done in 
    testability.* 
    
8.  Target and qual is fixed data type. Only one qual supported for now.
9.  Memory free is not done intentionally as it is like embedded application, so 
    once it goes down it will be freed.

Files Meaning:
Cost.* 			- Function to calculate cost of supported plan nodes.
path.* 			- Function to create supported nodes path.
plannodes.* 	- Functions to create plan from paths. Also explain functionality.
testability.* 	- Some of the testability related data (like replacement of stats 
										collection, relation metadata etc).
main.c 			- Application entry point to call the mock plan.										


/////////////////////////////////////////////////////////////////////////////
