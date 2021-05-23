#include "pthread.h"

typedef struct _thread_job
{
    void *(*process)(void *arg);
    void *arg;
    struct _thread_job *next;
}thread_job;

typedef struct _pthread_pool
{
    pthread_mutex_t lock;
    pthread_cond_t  cond;
    int             pthread_size;
    int             wait_num;
    int             is_destory;
    pthread_t       *tid;
    thread_job      *queue;
}pthread_pool;