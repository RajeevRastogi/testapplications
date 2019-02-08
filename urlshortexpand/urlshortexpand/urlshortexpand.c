#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*
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
 */
// structure to store currently generated buffer (shortURL).
typedef struct appendBuf
{
	char *data;
	int curLength;
	int maxLength;
}AppendBuf;

// Each hash entry.
typedef struct urlDetails
{
	long int id;
	char *longURL;
	char *shortURL;
}URLDetails;

// Linked list definition
typedef struct node
{
	URLDetails *details;
	struct node *next;
}*Node;


#define MAX_BUCKET				100
#define INIT_BUF_LENGTH 		50

Node hash[MAX_BUCKET] = {NULL};
long int lastID = 0;	

long int getIDForShortURL(char *shortURL);
int getHashValue(int value);
Node getNewNode(URLDetails *details);

void initAppendBuf(AppendBuf *buf)
{
	if ((buf->data = (char*)malloc(INIT_BUF_LENGTH)) == NULL)
	{
		printf("memory allocation failed while init of append buf\r\n");		
	}
	
	memset(buf->data, 0, INIT_BUF_LENGTH);
	buf->maxLength = INIT_BUF_LENGTH;
	buf->curLength = 0;
}

void enlargeAppendBuf(AppendBuf *buf)
{
	buf->data = (char*)realloc(buf->data, buf->maxLength + INIT_BUF_LENGTH);
	buf->maxLength += INIT_BUF_LENGTH;
}

void copyBufData(AppendBuf *buf, char ch)
{
	if (buf->curLength == buf->maxLength)
	{
		enlargeAppendBuf(buf);
		if (buf->data == NULL)
		{
			printf("Realloc failed during copy of buffer data\r\n");
			return;
		}
	}
	
	buf->data[buf->curLength] = ch;
	(buf->curLength)++;
}

void reverseData(char *data, int length)
{
	int i = 0;
	char temp;
	while (i < length/2)
	{
		temp = *(data + i);
		*(data + i) = *(data + length - 1 - i);
		*(data + length - 1 - i) = temp;
		i++;
	}
}

char *getShortURLForID(long int id)
{
	char baseValue[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	char base = strlen(baseValue);
	AppendBuf buf;
	
	initAppendBuf(&buf);
	if (buf.data == NULL)
	{
		return NULL;
	}
	
	while(id != 0)
	{
		//COPY_TO_APPEND_BUF(buf, baseValue[id%base]);
		copyBufData(&buf, baseValue[id%base]);
		if (buf.data == NULL)
		{
			return NULL;
		}
		
		id = id / base;
	}

	reverseData(buf.data, buf.curLength);
	return buf.data;	
}

long int getIDForShortURL(char *shortURL)
{
	long int id = 0;
	int i = 0;
	while (shortURL[i] != '\0')
	{
		if (shortURL[i] >= 'a' && shortURL[i] <= 'z')
		{
			id = id*62 + shortURL[i] - 'a';
		}
		else if (shortURL[i] >= 'A' && shortURL[i] <= 'Z')
		{
			id = id*62 + shortURL[i] - 'A' + 26;
		}
		else if (shortURL[i] >= '0' && shortURL[i] <= '9')
		{
			id = id*62 + shortURL[i] - '0' + 52;
		}

		i++;
	}

	return id;
}

int getHashValue(int value)
{
	return value % MAX_BUCKET;
}

Node getNewNode(URLDetails *details)
{
	Node newNode = (Node)malloc(sizeof(struct node));
	if (newNode == NULL)
	{
		printf("Memory allocation failed\r\n");
		return NULL;
	}

	newNode->details = details;
	newNode->next = NULL;
	return newNode;
}

char insertToHash(int hashValue, URLDetails *details)
{
	// create a new node to insert in hash
	Node temp;
	Node newNode;

	if ((newNode = getNewNode(details)) == NULL)
	{
		printf("Could not create a new node for hash entry\r\n");
		return -1;
	}

	if (hash[hashValue] == NULL)
	{
		hash[hashValue] = newNode;
		return 0;
	}

	//hash collision, add the new entry at the end of the list.
	temp = hash[hashValue];
	while(temp->next != NULL)
	{	
		temp = temp->next;
	}

	temp->next = newNode;
	return 0;
}

URLDetails *searchHash(long int id)
{
	int hashValue = id%MAX_BUCKET;
	Node temp = hash[hashValue];
	while(temp != NULL)
	{
		if (temp->details->id == id)
		{
			return temp->details;
		}

		temp = temp->next;
	}

	return NULL;
}

char* urlUtility(char *longURL, int length)
{
	long int nextID = lastID + 1;
	int hashValue = getHashValue(nextID);
	URLDetails *details = NULL;
	if ((details = (URLDetails*)malloc(sizeof(URLDetails))) == NULL)
	{
		printf("Memory allocation failed\r\n");
		return NULL;
	}
	
	if ((details->longURL = (char*)malloc(length + 1)) == NULL)
	{
		printf("Memory allocation failed\r\n");
		free(details);
		return NULL;
	}

	if (strncpy_s(details->longURL, length+1, longURL, length+1) < 0)
	{
		printf("strncpy_s failed to copy long URL\r\n");
		free(details->longURL);
		free(details);
		return NULL;
	}
	
	if ((details->shortURL = getShortURLForID(nextID)) == NULL)
	{
		free(details->longURL);
		free(details);
		return NULL;
	}
	
	details->id = nextID;
	if (insertToHash(hashValue, details) < 0)
	{
		//logging already happened inside the function.
		free(details->shortURL);
		free(details->longURL);
		free(details);
		return NULL;
	}
	
	lastID += 1;
	return details->shortURL;
}

char *getLongURL(char *shortURL)
{
	long int id = getIDForShortURL(shortURL);
	URLDetails *details = searchHash(id);

	// check if the given shortURL was if at all stored in our database.
	if (details == NULL)
	{
		printf("Long URL mapping of shortURL %s could not find\r\n",shortURL);
		return NULL;
	}	
	
	return details->longURL;
}
