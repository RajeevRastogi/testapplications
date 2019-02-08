========================================================================
    CONSOLE APPLICATION : urlshortexpand Project Overview
========================================================================
Problem Statement:
Implement an URL shortener and expander. The function header:  
char* urlUtility(char* inputUrl);. Caller example: urlUtility(“long convulted url with params”);

Solution Description:
 * Following functionalities are defined:
 *  1. Application can use URL expander/shortener to store the long URL.
 *     Currently we are using one simple hash to store below details
 *       a. Unique incremental ID
 *       b. Long URL
 *       c. Short URL
 *  2. urlUtility function generates an ID, find the hash bucket corresponding
 *     to this ID. Also using this ID, one short URL is generated using base-62.
 *     Finally all these are stored in hash.
 *  3. Application can request for long URL corresponding to given short URL, which
 *     is expected to be  major use-case.
 *  4. There is no interface currently implemented to get ID or shortURL corresponding
 *     to long URL. Also I thin that should not be use-case also.
 *     If at all this is required, we can easily implement but it wont be efficient as it
 *     will not have access all buckets in sequence till expected item is found.
 
File Meaning:
 urlshortexpand.*: Main file for implementation of functionality.
 main.c: Entry point for test application.
/////////////////////////////////////////////////////////////////////////////
