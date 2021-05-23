#include "common.h"

static pthread_pool *pool = NULL;

int pthread_routine()
{
    pool->pthread_size++;

    while (TRUE)
    {
        pthread_mutex_lock(&(pool->lock));
        while( 0 == pool->wait_num && FALSE == pool->is_destory)
        {
            pthread_cond_wait(&(pool->cond), &(pool->lock));
        }

        if(TRUE == pool->is_destory)
        {
            pthread_mutex_unlock(&(pool->lock));
            pthread_exit(NULL);
        }

        pool->wait_num--;
        thread_job *job = pool->queue;
        pool->queue = job->next;
        pthread_mutex_unlock(&(pool->lock));
        (*(job->process))(job->arg);
        free(job);
        job = NULL;
    }

    pthread_exit(NULL);
    
}

int pool_destory()
{
    if(pool->is_destory) return ERROR;

    pool->is_destory = TRUE;

    pthread_mutex_lock(&(pool->lock));
    pthread_cond_broadcast(&(pool->cond));
    pthread_mutex_unlock(&(pool->lock));

    int i = 0；

    for(i = 0; i < pool->pthread_size; i++)
    {
        pthread_join(pool->tid[i], NULL);
    }

    thread_job *job = NULL;

    while(pool->queue != NULL)
    {
        job = pool->queue;
        pool->queue = job->next;
        free(job);
    }

    free(pool->tid);
    pthread_mutex_destory(&(pool->lock));
    pthread_cond_destory(&(pool->cond));
    free(pool)
    pool = NULL;
    return OK;


}

int init_pthread_pool(int pthread_size)
{
    if(pthread_size <= 0)
    {
        printf("input pthread_size error: %d\n", pool_size);
        return ERROR;
    }

    if((pool = (pthread_pool *)malloc(sizeof(pthread))) == NULL )
    {
        printf("malloc pthread pool failed\n");
        return ERROR;
    }

    if(0 != pthread_mutex_init(&(pool->lock), NULL))
    {
        printf("init mutex failed\n");
        free(pool);
        return ERROR;
    }

    if(0 != pthread_cond_init(&(pool->cond), NULL))
    {
        printf("init cond failed\n");
        pthread_mutex_destory(pool->lock);
        free(pool);
        return ERROR;
    }

    if(NULL == (pool->tid = (pthread_t *)malloc(pthread_size * sizeof(pthread_t))))
    {
        printf("malloc pthread failed\n");
        pthread_mutex_destory(pool->lock);
        pthread_cond_destory(pool->cond);
        free(pool);
        return ERROR;   
    }

    pool->pthread_size = 0;
    pool->wait_num = 0;
    pool->is_destory = FALSE;
    pool->queue = NULL;

    int i = 0;
    for(i = 0; i < pthread_size; i++)
    {
        if(0 != pthread_create(&pool->tid[i], NULL, pthread_routine, NULL))
        {
            printf("pthread_create failed\n");
            pool_destory();
            return ERROR;
        }
    }

    /*wait for pthread all created */
    while(pthread_size != pool->pthread_size){}

    return OK;
}


int add_job(void *(*process)(void *arg), void *arg)
{
    thread_job *new_job = NULL;
    if(NULL == (new_job = (thread_job *)malloc(sizeof(thread_job)) ))
    {
        printf("malloc thread_job failed\n");
        return ERROR;
    }

    new_job->process = process;
    new_job->arg = arg;
    new_job->next = NULL;

    thread_job *job = NULL;

    pthread_mutex_lock(&(pool->lock));

    job = pool->queue;

    if(job == NULL)
    {
        job = new_job;
    }
    else
    {
        while (job->next != NULL)
        {
            job = job->next;
        }

        job->next = job;
        
    }

    pool->wait_num++;
    pthread_mutex_unlock(&(pool->lock));

    pthread_cond_signal(&(pool->cond));

    return OK;
}