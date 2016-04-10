/**	Student: Zhexuan Zachary Yang &  Yuanting Shao      **/
/**  X500:     yang4442      &  shaox119            **/
/**  CSELabs machine: kh4250-1                      **/


#ifndef  MAKEARGV_H
#define MAKEARGV_H

#include <errno.h>
#include <stdlib.h>
#include <string.h>

int makeargv(const char*s, const char *delimiters, char ***argvp);
void freemakeargv(char **argv);


#endif // MAKEARGV_H
