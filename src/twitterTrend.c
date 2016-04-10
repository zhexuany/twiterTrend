/**	Student: Zhexuan Zachary Yang &  Yuanting Shao      **/
/**  X500:     yang4442      &  shaox119            **/
/**  CSELabs machine: kh4250-1                      **/


#include "../include/twitterTrend.h"

/* main function in twitterTrend file */
size_t twitterTrend(string path, size_t numThread, bucket* head) {
  twitter = praseClient(path);
  bufferSize = numThread;
  createThread( head );
  return 0;
}

/*
 * main - main thread and will create as many threads as specified from terminal
 */
void createThread(bucket* head) {
  /* create a shared buffer */
  buffer_t* buffer;
  size_t rc;

  /* initialize shared queue */
  buffer = initBuffer( head );
  size_t i;
  /*
   * create threads for clients as numThread said
   */
  threads = MALLOC(bufferSize, pthread_t);
  for ( i = 0; i < bufferSize; i++ ) {
    /* run handleClient concurrently */
    rc = pthread_create(&threads[i],
                        NULL,
                        handleClients,
                        buffer);
    PthreadCheck("pthread_create", rc);
  }

  /* run producer in main thread */
  addClient(buffer);

  /* wait thread till it finish */
  for ( i = 0; i < bufferSize; ++i ) {
    rc = pthread_join(threads[i], NULL);
    PthreadCheck("pthread_join", rc);
  }

  destoryBuffer(buffer);   /*destory shared buffer*/
}



/* This is comsumer function */
void* handleClients( void* buffer ) {
  size_t item;
  while (1) {
    item = bufferRemove(buffer);
    processClient(item, buffer);
  }
}

/* this is producer function */
void* addClient(buffer_t* buffer) {
  while (1) {
    bufferAdd(buffer);
  }
}

/* process clients */
void processClient(size_t num, buffer_t* buffer) {
  string client;
  string cityName;
  string trends;

  size_t threadId;
  /* get client info */
  client = twitter[num];
  threadId = extractThreadId(pthread_self());


  fprintf(stderr, START, threadId, client); // print start info to stdout

  /* get city name */
  cityName = getCity(client);

  /* retrive trends info associated with cith name */
  trends = search(cityName, buffer -> current);

  /* write contents to *.result file */
  write2File( client, cityName, trends );

  fprintf(stderr, FINISH, threadId, client); // print finisded info to stdout
}




/* helper function */
string getCity(string path) {
  FILE* fp;
  string buffer = MALLOC(MAX_CITY, char);
  string newPath = MALLOC(MAX_BUF, char);
  strcpy(newPath, PREFIXSTR);
  strcat(newPath, path);

  /* remove new line from the string */
  newPath[strlen(newPath) - 1] = NULLEND;
  if ((fp = fopen(newPath, "r")) == NULL) {
    perror("Failed to open file");
  }
  fgets(buffer, MAX_CITY, fp);

  /* remove new line from the string */
  buffer[strlen(buffer) - 1] = NULLEND;
  return buffer;
}

/* write result to result file */ 
void write2File( string path, string cityName, string trends ) {
  FILE* fp;
  string newPath = MALLOC(MAX_BUF, char);
  strcpy (newPath, PREFIXSTR);
  strcat (newPath, path);

  /* remove newline from newPath */
  newPath[strlen(newPath) - 1] = NULLEND;
  strcat (newPath, APPENDSTR);

  if ((fp = fopen(newPath, "w")) == NULL) {
    perror("Failed to open file");
  }

  fwrite(cityName, sizeof(char), strlen(cityName), fp); // write city
  fputs(COLON, fp); // write colon
  fwrite(trends, sizeof(char), strlen(trends), fp); // write trends

  if( ferror( fp ) != 0 )      { // error check
    perror( "I/O error when writing:" );
  }
}


/* extract client info */
string* praseClient( string path ) {
  FILE* fp;
  size_t i = 1;
  string buffer;
  buffer = MALLOC(MAX_CITY, char);
  if ((fp = fopen(path, "r")) == NULL) {
    perror("Failed to open file");
  }
  string* twitter = CALLOC(MAX_CLIENT, string);
  while ( fgets(buffer, MAX_CITY, fp) ) {
    twitter[i] = MALLOC(MAX_BUF, char);
    strcpy(twitter[i], buffer);
    i++;
  }

  /* counter for number of clients */
  clientSize = i - 1;
  free(buffer);
  return twitter;
}

/* extract thread ID */
size_t extractThreadId(pthread_t id) {
  size_t i;
  for (i = 0; i < bufferSize; i++) {
    if (id == threads[i]) {
      return i + 1;
      break;
    }
  }
  return 0;
}
