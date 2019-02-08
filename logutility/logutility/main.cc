// This file is only test file to test Log utility
//

#include<iostream>
#include "Logger.h"
#include <windows.h>
#include "test.h"

#define THREADCOUNT 2

DWORD WINAPI logWorker(LPVOID lpParam)
{
	multitest(*(int*)lpParam);
	return 0;
}

int concTest()
{
	HANDLE aThread[THREADCOUNT];
    DWORD ThreadID;
    int i;
	int a[THREADCOUNT];

	for( i=0; i < THREADCOUNT; i++ )
    {
		a[i]=i+1;
        aThread[i] = CreateThread( 
                     NULL,       
                     0,          
                     (LPTHREAD_START_ROUTINE) logWorker, 
                     a+i,       
                     0,          
                     &ThreadID); 

        if( aThread[i] == NULL )
        {
            printf("CreateThread error: %d\n", GetLastError());
            return 1;
        }
    }

    // Wait for all threads to terminate
    WaitForMultipleObjects(THREADCOUNT, aThread, TRUE, INFINITE);

    // Close thread and mutex handles
    for( i=0; i < THREADCOUNT; i++ )
        CloseHandle(aThread[i]);

	return 0;
}

int main(int argc, char *argv[])
{
	//acceptance test
	LOG(DEBUG, "Server initialized successfully"); 

	// single thread related testing.
	planetest();

	//multithread test
	concTest();
	Logger::release();
	return 0;
}

