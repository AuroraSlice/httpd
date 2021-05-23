#ifndef _EPOLL_H_
#define _EPOLL_H_

#include <sys/epoll.h>

#define MAX_EPOLL_FD 1024

struct epoll_event *events;

int init_epoll(flags);

#endif