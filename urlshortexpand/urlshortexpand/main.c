#include<stdio.h>
#include<string.h>
#include "urlshortexpand.h"

int main(int argc, char *argv[])
{
	//test case should check for NULL
	char *short1 = urlUtility("my first URL",strlen("my first URL"));
	char *short2 = urlUtility("my second URL",strlen("my second URL"));
	char *short3 = urlUtility("really looooonnngggg URL",strlen("really looooonnngggg URL"));
	char *short4 = urlUtility("One more URL which too long",strlen("One more URL which too long"));
	char *short5 = urlUtility("long convulted url with params", strlen("long convulted url with params"));

	if (short1 == NULL || strcmp("my first URL", getLongURL(short1)))
		printf("Test case-1 failed\r\n");
	if (short2 == NULL || strcmp("my second URL", getLongURL(short2)))
		printf("Test case-2 failed\r\n");
	if (short3 == NULL || strcmp("really looooonnngggg URL", getLongURL(short3)))
		printf("Test case-3 failed\r\n");
	if (short4 == NULL || strcmp("One more URL which too long", getLongURL(short4)))
		printf("Test case-4 failed\r\n");
	if (short5 == NULL || strcmp("long convulted url with params", getLongURL(short5)))
		printf("Test case-4 failed\r\n");
	return 0;
}