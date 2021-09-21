#include <signal.h>
#include "common.h"
#include "pthread_pool.h"

/* 信号处理, 成功返回1, 失败返回-1 */
int init_signal()
{
	struct sigaction act;
	act.sa_handler = (void *)destory_thread_pool;
	sigemptyset(&act.sa_mask);

	if(-1 == sigaction(SIGINT, &act, NULL)){
		printf("register signal failed\n");
		return ERROR;
	}

	return OK;
}
