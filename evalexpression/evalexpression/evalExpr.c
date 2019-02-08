#include<iostream>
#include<stack>
using namespace std; 

/*
 * NOTE: 
 * 1. Considering only +,-,* and / are allowed. Even bracket also not allowed
 */
  
// Function to find precedence of  operators. 

int getOperPriority(char op){ 
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
int applyOp(int left, int right, char oper)
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
			return -1;
    } 
} 
  
/*
 * Function to evaluate given expression as per the given input.
 */
int evalExpr(char* expr, int a, int b, int c, int d)
{
	int arr[4] = {a, b, c, d};
    int i; 
      
    // stack to store integer values. 
    stack <int> values; 
      
    // stack to store operators. 
    stack <char> ops; 
      
    for(i = 0; expr[i] != '\0'; i++){ 
          
        // skip all whitespace
        if(expr[i] == ' ') 
            continue; 
          
        // Check if it is digit or alpha
        else if(isalpha(expr[i]) || isdigit(expr[i])){ 
            int newValue = 0; 
			int curDigit;
              
            // Read whole value of operand
            while (expr[i] != '\0' && (isalpha(expr[i]) || isdigit(expr[i]))) 
            { 
            	if (isalpha(expr[i]))
					curDigit = arr[expr[i] - 'a'];
				else
					curDigit = expr[i] - '0';
				
				newValue = newValue*10 + curDigit;
				i++; 
            } 
              
            values.push(newValue); 
        } 
        else
        { 
            // Finish processing of all operators whose precedance is same or more than
            // current operator precedance.
            while(!ops.empty() && getOperPriority(ops.top()) 
                                >= getOperPriority(expr[i]))
            { 
				int right = values.top();
				values.pop();
				
				int left = values.top();				
				values.top();
				
				char op = ops.top();
				op.top();
				values.push(applyOp(left, right, op));

            } 
              
            // Push current token to 'ops'. 
            ops.push(expr[i]); 
        } 
    } 
      
    // whole expression is done, just process remaining on the stack.
    while(!ops.empty())
	{ 
        int right = values.top();
		values.pop();
		
		int left = values.top();                
		values.top();
		
		char op = ops.top();
		ops.top();
        values.push(applyOp(left, right, op));  
    } 
      
    // Top of 'values' contains result, return it. 
    return values.top(); 
}   
