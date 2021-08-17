#include "common.h"
#include "epoll.h"

int init_epoll(int flags)
{
    int epfd = 0;

    if((epfd = epoll_create1(flags)) < 0)
    {
        printf("epoll create failed\n");
        return ERROR;
    }

    if(!(events = (struct epoll_event *)malloc(sizeof(struct epoll_event) * MAX_EPOLL_FD)))
    {
        printf("epoll malloc failed\n");
        return ERROR;
    }

    return epfd;


}