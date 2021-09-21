#ifndef _COMMON_
#define _COMMON_

#include<stdio.h>
#include<stdlib.h>

#define OK (0)
#define ERROR (-1)

#define TRUE (1)
#define FALSE (0)

typedef struct _config_t
{
    int port;
    int thread_num;

}config_t;

int init_signal();

#endif


