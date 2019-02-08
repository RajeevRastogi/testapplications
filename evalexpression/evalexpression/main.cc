#include<iostream>
#include<stdio.h>
#include "evalExpr.h"
#include<math.h>

/* TEST CODE*/
int main()
{
	double ret;
	if ((ret = floor(evalExpr("21a*53b/5c*9d",1,2,3,4))) != 199088)
		printf("test case-1 failed expected %lf: retuned %lf\r\n", 199088, ret);
	if ((ret = floor(evalExpr("21a*53b/5c*9d*10c",1,2,3,4))) != 20506110)
		printf("test case-2 failed expected %lf: retuned %lf\r\n", 20506110, ret);
	if ((ret = floor(evalExpr("12a*53b+5c-5d*8c+3a",1,2,3,4))) != 59974)
		printf("test case-3 failed expected %lf: retuned %lf\r\n", 59974, ret);
	if ((ret = floor(evalExpr("12a*53b+5c-5d+4c-3b-3b-3a+2b+2d*3a*3b/4a/4b*8c+3a",1,2,3,4))) != 65519)
		printf("test case-4 failed expected %lf: retuned %lf\r\n", 65519, ret);
	if ((ret = floor(evalExpr("12a*53b+5c-5d+4c-3b-3b-3a+2b+2d*3a*3b/4a/4b*8c+3a",5,3,9,8))) != 68119)
		printf("test case-4 failed expected %lf: retuned %lf\r\n", 68119, ret);
	if ((ret = floor(evalExpr("12a*53b+5c-5d+4c-3b-3b-3a+2b+2d*3a*3b/4a/4b*8c+3a",5,3,9,18))) != 77753)
		printf("test case-5 failed expected %lf: retuned %lf\r\n", 77753, ret);
	if ((ret = floor(evalExpr("12a*53b+5c-5d+4c-3b-3b-3a+2b+2d*3a*3b/4a/4b*8c+3a",125,23,9,0))) != 64542039)
		printf("test case-6 failed expected %lf: retuned %lf\r\n", 64542039, ret);
	if ((ret = floor(evalExpr("53b+5d+4c",125,23,9,0))) != 5422)
		printf("test case-6 failed expected %lf: retuned %lf\r\n", 5422, ret);
	if ((ret = floor(evalExpr("3b-3a-5c",125,23,9,0))) != -2861)
		printf("test case-6 failed expected %lf: retuned %lf\r\n", -2861, ret);
	if ((ret = floor(evalExpr("2a*3b+4c-5d", 1, 2, 3, 4))) != 661)
		printf("test case-6 failed expected %lf: retuned %lf\r\n", 661, ret);
	if ((ret = floor(evalExpr("2*3+4-5", 1, 2, 3, 4))) != 5)
		printf("test case-6 failed expected %lf: retuned %lf\r\n", 5, ret);
	if ((ret = floor(evalExpr("2a*3+4-5d", 1, 2, 3, 4))) != 13)
		printf("test case-6 failed expected %lf: retuned %lf\r\n", 13, ret);
	if ((ret = floor(evalExpr("0a+0b", 1, 298, 3, 4))) != 299)
		printf("test case-6 failed expected %lf: retuned %lf\r\n", 299, ret);

	// negative case:
	if ((ret = evalExpr("21a* + 53b/5c*9d",1,2,3,4)) != INFINITY)
		printf("test case-n1 failed expected expression parse to fail\r\n");
	if ((ret = evalExpr("21a*53b/5c*9d*2p",1,2,3,4)) != INFINITY)
		printf("test case-n2 failed expected expression parse to fail\r\n");
	if ((ret = evalExpr("21a 99*53b/5c*9d*2p",1,2,3,4)) != INFINITY)
		printf("test case-n3 failed expected expression parse to fail\r\n");
	if ((ret = evalExpr("21a%53b/5c*9d*2p",1,2,3,4)) != INFINITY)
		printf("test case-n4 failed expected expression parse to fail\r\n");
	if ((ret = evalExpr("*21a-53b/5c*9d*2p",1,2,3,4)) != INFINITY)
		printf("test case-n5 failed expected expression parse to fail\r\n");
	return 0;
}