/**	Student: Zhexuan Zachary Yang &  Yuanting Shao      **/
/**  X500:     yang4442      &  shaox119            **/
/**  CSELabs machine: kh4250-1                      **/
#ifndef _BUFFER_H
#define _BUFFER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include "alloc.h"
#include "map.h"

/* used for open/write file and print to stdout */
#define APPENDSTR ".result"
#define PREFIXSTR "test/"
#define COLON     " : "
#define NEWLINE   "\n"
#define NULLEND   0
#define WAIT                                    \
  "Waiting to add clients to the full queue\n"

/*define error checking function*/
#define COND_CHECK(func, cond, retv, errv)                              \
  if ( (cond) )                                                         \
    {                                                                   \
      fprintf(stderr, "\n[CHECK FAILED at %s:%d]\n| %s(...)=%ld (%s)\n\n", \
              __FILE__,__LINE__,func,retv,strerror(errv));              \
      exit(EXIT_FAILURE);                                               \
    }

#define ErrnoCheck(func,cond,retv)  COND_CHECK(func, cond, retv, errno)
#define PthreadCheck(func,rc) COND_CHECK(func,(rc!=0), rc, rc)



typedef struct buffer {
  int done; // handle threads larger than clients
  size_t buffin; /* counter for main thread*/
  size_t buffout; /* counter for consumer thread */
  size_t occupied; /*how many elements in shared buffer */
  bucket* current; /* head of linked list which store all info trends */
  pthread_mutex_t mutex; /* lock for protecting crtical section */
  pthread_cond_t cond_full; /* condition variable for checking buffer is full */
  pthread_cond_t cond_empty; /* condition variable for checking buffer empty */
}buffer_t;

/*define global variables for threads*/
string* twitter; // used for store all client info
size_t bufferSize; // bounded buffer size
size_t clientSize; // how many client in this run

/* prototypes for shared buffer */

buffer_t* initBuffer ( bucket* head); // initialize buffer 
void destoryBuffer(buffer_t* buffer); // destroy buffer
size_t bufferRemove(buffer_t* buffer); // remove item from buffer
void bufferAdd(buffer_t* buffer); // add item to buffer

#endif // _BUFFER_H
