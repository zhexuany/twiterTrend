/**	Student: Zhexuan Zachary Yang &  Yuanting Shao      **/
/**  X500:     yang4442      &  shaox119            **/
/**  CSELabs machine: kh4250-1                      **/
#ifndef TWITTERTREND_H
#define TWITTERTREND_H


/* max number of clients */
#define MAX_CLIENT 50

/* usefull into for printing */
#define START \
  "Thread%ld is handling client %s"
#define FINISH \
  "Thread%ld finished handling client %s"



#include <unistd.h>
#include "map.h"
#include "alloc.h"
#include "buffer.h"

/* for thread ID */
pthread_t* threads;

/* prototype goes here */
size_t twitterTrend( string path, size_t numThread, bucket* head );
string* praseClient( string path );
void createThread( bucket* head );
void* handleClients(void* buffer);
void processClient(size_t num, buffer_t* buffer);
void* addClient(buffer_t* buffer);
void write2File(string path, string cityName, string trends);
string getCity(string path);
size_t extractThreadId(pthread_t ids);

#endif // TWITTERTREND_H
