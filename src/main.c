/**	Student: Zhexuan Zachary Yang &  Yuanting Shao      **/
/**  X500:     yang4442      &  shaox119            **/
/**  CSELabs machine: kh4250-1                      **/
#include "../include/main.h"

int main(int argc, char *argv[]) {
  string path;
  size_t numThread;
  if (argc != 3) {
    perror("Only accpet two arguments");
  }
  /* extract useful information from command line */
  path = argv[1];
  numThread = atoi(argv[2]);

  /* open data base file */
  FILE* fp = openFile( DATABASE );

  /* for imporving the efficiency, stotre data in a linked list (map) */
  bucket* head = makeMap( fp );
  /* start to handle clients */
  twitterTrend( path, numThread, head );

  return 0;

}
