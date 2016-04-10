/**	Student: Zhexuan Zachary Yang &  Yuanting Shao      **/
/**  X500:     yang4442      &  shaox119            **/
/**  CSELabs machine: kh4250-1                      **/

#include "../include/buffer.h"

buffer_t* initBuffer ( bucket* head ) {
  size_t rc; // used for error checking
  buffer_t* buffer = CALLOC(1, buffer_t); // allocate memory to buffer
  buffer -> buffin = 0;
  buffer -> done = 0;
  buffer -> buffout = 0;
  buffer -> occupied = 0;
  buffer -> current = head;
  rc = pthread_mutex_init(&(buffer -> mutex), NULL); // intialize mutex lock
  PthreadCheck("pthread_mutex_init", rc);
  rc = pthread_cond_init(&(buffer -> cond_empty), NULL); // intialize condition var
  PthreadCheck("pthread_cond_init", rc);
  rc = pthread_cond_init(&(buffer -> cond_full), NULL); // intialize condition var
  PthreadCheck("pthread_cond_init", rc);
  return buffer;
}

void destoryBuffer(buffer_t* buffer) {
  size_t rc;
  freeBucket( buffer -> current ); // free head, data structrue for storing all info

  rc = pthread_mutex_destroy(&(buffer -> mutex)); // destory mutex lock
  PthreadCheck("pthread_mutex_destroy", rc);


  rc = pthread_cond_destroy(&(buffer -> cond_empty)); // destroy condition var
  PthreadCheck("pthread_cond_destroy", rc);

  rc = pthread_cond_destroy(&(buffer -> cond_full)); // destory condition var
  PthreadCheck("pthread_cond_destroy", rc);

  free(buffer);
}

size_t bufferRemove(buffer_t* buffer) {
  size_t rc;
  
  rc = pthread_mutex_lock(&(buffer -> mutex)); // lock
  PthreadCheck("pthread_mutex_lock", rc);

  if (buffer -> done) { // handle if num_threads larger than clients
    rc = pthread_mutex_unlock(&(buffer -> mutex));
    PthreadCheck("pthread_mutex_unlock", rc);
    pthread_exit(NULL);
  }
  if (buffer ->  buffout >= clientSize) {  // test condtion, it true, unlock and exit
    buffer -> done = 1;
    rc = pthread_mutex_unlock(&(buffer -> mutex));
    PthreadCheck("pthread_mutex_unlock", rc);
    pthread_exit(NULL);
  }
  while (buffer -> occupied == 0) { // wait for producer add item
    rc = pthread_cond_wait(&(buffer -> cond_empty), &(buffer -> mutex));
    PthreadCheck("pthread_cond_wait", rc);
  }

  -- buffer -> occupied; // remove item
  ++ buffer -> buffout; //  increase consumer counter

  /* once the buffer is empty, wake up main thread */
  if (buffer -> occupied <= 0) {
    rc = pthread_cond_signal(&(buffer -> cond_full));
    PthreadCheck("pthread_cond_signal", rc);
  }

  rc = pthread_mutex_unlock(&(buffer -> mutex)); // unlock mutex lock
  PthreadCheck("pthread_mutex_unlock", rc);

  return buffer -> buffout;
}



void bufferAdd(buffer_t* buffer) {
  size_t rc;
  rc = pthread_mutex_lock(&(buffer -> mutex)); // lock
  PthreadCheck("pthread_mutex_lock", rc);

  while (buffer -> occupied >=  bufferSize) { // wait for empty is not full
    fprintf(stderr, WAIT);
    rc = pthread_cond_wait(&(buffer -> cond_full), &(buffer -> mutex));
    PthreadCheck("pthread_cond_wait", rc);
  }

  ++ buffer -> occupied; // add item
  ++ buffer -> buffin; // increase producer counter

  /* wake up worker threads if the buffer is not empty */
  rc = pthread_cond_signal(&(buffer -> cond_empty));
  PthreadCheck("pthread_cond_signal", rc);

  if (buffer -> buffin >= clientSize ) { // test condition, if true, unlock and exit
    rc = pthread_mutex_unlock(&(buffer -> mutex));
    PthreadCheck("pthread_mutex_unlock", rc);
    pthread_exit(NULL);
  }

  rc = pthread_mutex_unlock(&(buffer -> mutex)); // unlock mutex
  PthreadCheck("pthread_mutex_unlock", rc);
}
