/**	Student: Zhexuan Zachary Yang &  Yuanting Shao      **/
/**  X500:     yang4442      &  shaox119            **/
/**  CSELabs machine: kh4250-1                      **/

#ifndef _MAP_H__
#define _MAP_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "makeargv.h"
#include "alloc.h"

#define MAX_BUF 100 // Every line in TwitterDB.txt is less than 100 characters.
#define DELIMETER "," //for distinguish city and trends
#define MAX_CITY 15 // cityName are less than 15 characters
#define MAPSIZE 50 // max clients number

/*define new data type*/
typedef char* string;
typedef int   bool;
/* define a node structure */
typedef struct bucket{
  struct bucket* next;
  string trends;
  string cityName;
  size_t nodeId;
}bucket;


/*define prototypes here*/
FILE* openFile( string fileName );
bucket* makeMap( FILE* fp );
bool isEqual(string lefekey, string rightkey);
bucket* createLinkedList( size_t size );
bucket* push(string name, string record, bucket* current);
string search( string name, bucket* head );
bucket* createLinkedList( size_t size );
void  freeBucket(bucket* head);


#endif // _MAP_H__
