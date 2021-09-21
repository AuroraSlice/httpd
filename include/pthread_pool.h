#include "pthread.h"

typedef struct _thread_job
{
	void * (*process)(void *arg);
	void *arg;
	struct _thread_job *next;
}thread_job;

typedef struct _pthread_pool
{
	thread_job		manager;		//管理任务
	thread_job		*taskQueue;		//任务队列
	pthread_t			*tid;		//线程号队列
	pthread_mutex_t	lock;		//锁住整个数据结构
	pthread_mutex_t	taskLock;		//锁住任务队列
	pthread_cond_t	notFull;	//线程池不满信号
	pthread_cond_t	notEmpty;	//线程池不为空
	int				pthread_size;	//线程池线程大小
	int				current_num;		//当前正在处理的任务个数
	int				wait_num;			//正在等待的任务个数
	int				is_destory;	//线程池销毁标志
	
}pthread_pool;

int add_job(void* (*process)(void *), void *arg);
int init_pthread_pool(int pthread_size);
int destory_thread_pool();