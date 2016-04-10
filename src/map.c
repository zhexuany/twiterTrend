/**	Student: Zhexuan Zachary Yang &  Yuanting Shao      **/
/**  X500:     yang4442      &  shaox119            **/
/**  CSELabs machine: kh4250-1                      **/

#include "../include/map.h"

/*open file by filename*/
FILE* openFile( string fileName ) {
  FILE* fp = fopen(fileName, "r");
  if ( fp == NULL ) {
    perror("Failed to open file");
  }

  return fp;
}

/*read contents from database file
  each line start with city name and trends
  hash city name and store trend to bucket
*/
bucket* makeMap( FILE* fp ) {
  string buffer = MALLOC( MAX_BUF, char );
  string cityName;
  bucket* head = createLinkedList( MAPSIZE );
  bucket* current = head;
  while ( fgets(buffer, MAX_BUF, fp) ) {
    string* tempBuff = CALLOC(MAX_BUF, string);
    string trends = CALLOC(MAX_BUF, char);

    /*prase city name and its associated trends*/
    makeargv(buffer, DELIMETER, &tempBuff);
    /*copy city name to cityName*/
    cityName = tempBuff[0];
    /*copy trends to trends*/
    strcpy(trends, buffer + strlen(tempBuff[0]) + 1);
    /*add cityName and its associated trneds to current node
      and move current to its next node
     */
    current = push(cityName, trends, current);
    freemakeargv( tempBuff );
  }

  /*free buffer*/
  free( buffer );
  return head;
}

/*make a linked list with size size*/
bucket* createLinkedList( size_t size )  {
  bucket* head;
  bucket* newKey;
  bucket* current;
  size_t index = 1;
  head = MALLOC( sizeof(bucket*), bucket );
  current = head;
  while (size --) {
    newKey = MALLOC( sizeof(bucket*), bucket );
    newKey -> next = NULL;
    newKey -> trends = NULL;
    newKey -> cityName = NULL;
    newKey -> nodeId = index;
    current -> next = newKey;
    current = newKey;
    index ++;
  }
  return head;
}

/*free all linked list*/
void freeBucket(bucket* head) {
  bucket* current;
  while ( (current = head)!= NULL ) {
    head = head -> next;
    free(current);
  }
}

/*add key and value to current node*/
bucket* push( string name, string record, bucket* current ) {
  /*if current -> trends is empty then use it*/
  current = current -> next;
  if (current -> trends == NULL) {
    current -> trends = MALLOC ( strlen(record) + 1, char);
    current -> cityName = MALLOC ( strlen(name) + 1, char);
    strcpy( current -> trends, record );
    strcpy( current -> cityName, name );
  }

  return current;
}

string search( string name, bucket* head ) {
  bucket* current;
  current = head -> next;
  while ( current -> cityName) {
    if (isEqual (name, current -> cityName)) {
      return current -> trends;
      break;
    }
    current = current -> next;
  }
  perror("City does not exit in list");
  return 0;
}


bool isEqual( string lefekey, string rightkey ) {
  return !strcmp(lefekey, rightkey);
}
