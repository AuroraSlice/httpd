#include<pthread.h>
#include "common.h"
#include "pthread_pool.h"

static pthread_pool *pool = NULL;

void * pthread_routine()
{
	thread_job job, *tmp = NULL;

	while(TRUE)
	{
		pthread_mutex_lock(&pool->lock);
		
		if(!pool->wait_num)//没有任务在等待，等待条件变量
		{
			pthread_cond_wait(&pool->notEmpty, &pool->lock);
		}	

		if(pool->taskQueue)
		{
			tmp = pool->taskQueue;
		
			job.arg = tmp->arg;
			job.process = tmp->process;
			pool->taskQueue = tmp->next;


			tmp->next = NULL;
			free(tmp);
		}

		pool->current_num++;
		pool->wait_num--;
		
		pthread_mutex_unlock(&pool->lock);

		//执行
		job.process(job.arg);
		
	}

	pthread_exit(NULL);
	
}

int pthread_manage()
{
	
}

int destory_thread_pool()
{

	printf("destory thread pool.......\n");

	pool->is_destory = TRUE;

	//销毁线程池
	pthread_cond_destroy(&pool->notFull);
	pthread_cond_destroy(&pool->notEmpty);
	
	int i = 0;
	for(i = 0; i < pool->pthread_size; i++){
		pthread_join(pool->tid[i], NULL);
	}

	thread_job *head = NULL;

	pthread_mutex_lock(&pool->lock);

	while(pool->taskQueue != NULL){
		head = pool->taskQueue;
		pool->taskQueue = head->next;
		free(head);
	}
	
	if(pool->tid)
	{
		free(pool->tid);
	}

	pthread_mutex_unlock(&pool->lock);

	pthread_mutex_destroy(&pool->lock);
	pthread_mutex_destroy(&pool->taskLock);

	if(pool)
	{
		free(pool);
	}

	pool = NULL;

	printf("destory pool finished\n");
	return OK;
}

int init_pthread_pool(int pthread_size)
{
	int i = 0;

	do{
		pool = (pthread_pool *)malloc(sizeof(pthread_pool));
		if(pool == NULL)
		{
			printf("pthread pool malloc failed\n");
			break;
		}

		pool->tid = (pthread_t *)malloc(sizeof(pthread_t) * pthread_size);
		if(pool->tid == NULL)
		{
			printf("pthread tid malloc failed\n");
			break;
		}

		//pthread_create(&pool->manager, 0, pthread_manage, NULL);

		for(i = 0; i <pthread_size; i++)
		{
			pthread_create(&pool->tid[i], 0, pthread_routine, NULL);
		}
			printf("2222\n");
		
		//数据初始化
		pool->current_num = 0;
		pool->is_destory =FALSE;
		pool->pthread_size = pthread_size;
		pool->wait_num = 0;


		//初始化锁与条件
		pthread_mutex_init(&pool->lock, NULL);
		pthread_mutex_init(&pool->taskLock, NULL);


		pthread_cond_init(&pool->notEmpty, NULL);
		pthread_cond_init(&pool->notFull, NULL);

			printf("1111\n");

		return OK;

	}while(0);

	printf("create pool failed, destory.......\n");
	
	//销毁线程池
	pthread_cond_destroy(&pool->notFull);
	pthread_cond_destroy(&pool->notEmpty);

	pthread_mutex_destroy(&pool->lock);
	pthread_mutex_destroy(&pool->taskLock);
	if(pool->tid)
	{
		free(pool->tid);
	}

	if(pool->taskQueue)
	{
		free(pool->taskQueue);
	}

	if(pool)
	{
		free(pool);
	}

	printf("destory pool finished\n");

	return ERROR;
}


int add_job(void* (*process)(void *), void *arg)
{
	thread_job *new_job = NULL;

	if(pool->is_destory)
	{
		printf("pool destory, reject\n");
		return ERROR;
	}
	
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

	job = pool->taskQueue;

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

	pthread_cond_signal(&(pool->notEmpty));

	return OK;
}
