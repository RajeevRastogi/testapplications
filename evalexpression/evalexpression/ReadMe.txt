========================================================================
    Evaluate Expression : evalexpression Project Overview
========================================================================
Problem Statement:
Implement a function that takes in an equation and parameters and return the 
result of the equation. Limit the function to handle additions, substractions, 
multiplication and division. The function header: int f (char*, int, …) where 
first parameter string is the input equation, second through fifth parameters 
are values for a, b, c, and d values that are used in the equation. 
Caller example: f(“2a*3b+4c-5d”, 1, 2, 3, 4);

Solution Description:
 * NOTE: 
 * 1. Considering only +,-,* and / are allowed. Even bracket also not allowed
 * 2. Overflow of result is not handled.
 * 3. I have slightly modified the prototype compared to given in question as 
 *    the result may be in fraction also as division operation there. So now 
 *	  modified prototype to returns 'double'.
 * 4. I am assuming apart from expression, 4 arguments allowed, whose value
 *    simply needs to be replaced. i.e. 
 *      suppose value of a = 1, then 2a = 21 (not 2*1)
 *      suppose value of b = 397, then 2b = 2397 (not 2*397)

File Meaning:
evalExpr.* - Main file to implementation expression evaluation.
main.c - test application entry point to test functionality.

/////////////////////////////////////////////////////////////////////////////
