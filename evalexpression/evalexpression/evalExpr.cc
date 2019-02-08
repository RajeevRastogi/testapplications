#include<iostream>
#include<stack>
#include "evalExpr.h"
using namespace std; 

#define OPERAND 1
#define OPERATOR 2

/*
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
 */
  
// Function to find precedence of  operators. 
char getOperPriority(char op)
{ 
    if(op == '+'||op == '-') 
    {
    	return 1; 
    }
	else if (op == '*'||op == '/') 
	{
		return 2;
	}
	
    return -1; 
} 
  
// Perform required operation. 
double applyOp(double left, double right, char oper)
{ 
    switch(oper)
	{ 
        case '+': 
			return left + right; 
        case '-': 
			return left - right; 
        case '*': 
			return left * right; 
        case '/': 
			return left / right; 
		default:
			// Will never reach here.
			printf("Unsupported operator \'%c\'\r\n",oper);
			return 0;
    } 
} 
  
/*
 * Main Interface function to evaluate given expression as per the given input.
 * Param
 * 	expr: Actual expression.
 *    a,b,c,d: Parameter used in expression, which needs to be replaced with actual value.
 * Returns:
 *	result of expression truncate to integer datatype if success
 *	otherwise INFINITY on failure.
 */
double evalExpr(char* expr, int a, int b, int c, int d)
{
	int arr[4] = {a, b, c, d};
    int i; 
	int lastField = 0;
      
    // stack to store integer values (just to reduce using built-in 
	// otherwise we can replace with actual implementation of stack. 
    stack <double> values; 
      
    // stack to store operators. 
    stack <char> ops; 
      
    for(i = 0; expr[i] != '\0'; i++)
	{           
        // ignore all whitespace
        if(expr[i] == ' ') 
            continue; 
          
        // Check if it is digit or alpha
        else if(isalpha(expr[i]) || isdigit(expr[i]))
		{ 
            int newValue = 0; //this as int ok, as it does not store any result.
			int curDigit;
			int tempDigit;

			if (lastField == OPERAND)
			{
				printf("Unexpected operand starting with \'%c\'\r\n",expr[i]);
				return INFINITY;
			}

			lastField = OPERAND;
            // Read whole value of operand
            while (expr[i] != '\0' && (isalpha(expr[i]) || isdigit(expr[i]))) 
            { 
            	if (isalpha(expr[i]))
            	{
            		if (expr[i] >= 'a' && expr[i] <= 'd')
            		{
						curDigit = arr[expr[i] - 'a'];
            		}
					else
					{
						printf("Unsupported variable name \'%c'\\r\n",expr[i]);
						return INFINITY;
					}
            	}
				else
					curDigit = expr[i] - '0';

				// Format the operand as per given parameters.
				// it may happen that a, b, c or d passed have value greated 9.
				if (curDigit <= 9)
				{
					newValue = newValue*10 + curDigit;
				}
				else
				{
					// Depending on number of digits in the argument value, we need to
					// append that many zero so that argument can be added(appended).
					// Its possible to avoid everytime calculation of number of digit
					// by computing this in begining. But it is not expected to be very
					// big so its ok to keep simple code.
					tempDigit = curDigit;
					while (tempDigit != 0)
					{
						newValue *= 10;
						tempDigit /= 10;
					}

					newValue += curDigit;
				}

				i++; 
            } 
            
			i--;
            values.push((double)newValue); 
        } 
        else
        { 			
            char curOper;

			// Check for invalid expression.
            if ((curOper = getOperPriority(expr[i])) == -1 || lastField == OPERATOR || i == 0)
			{
				printf("Invalid operator \'%c\' or invalid sequence of operand\r\n",expr[i]);
				return INFINITY;
            }

			// it's operator, process current sub-expression if 
			// priority is equal or more than last operator.
			lastField = OPERATOR;	
            while(!ops.empty() && getOperPriority(ops.top()) 
                                >= curOper)
            { 
				double right = values.top();
				values.pop();
				
				double left = values.top();				
				values.pop();
				
				char op = ops.top();
				ops.pop();
				values.push(applyOp(left, right, op));
            } 
              
            // Push current operand as 'ops'. 
            ops.push(expr[i]); 
        } 
    } 
      
    // whole expression is done, just process remaining on the stack.
    while(!ops.empty())
	{ 
        double right = values.top();
		values.pop();
		
		double left = values.top();                
		values.pop();
		
		char op = ops.top();
		ops.pop();
        values.push(applyOp(left, right, op));  
    } 
      
    // return result from top of the stack
    return values.top(); 
}   
