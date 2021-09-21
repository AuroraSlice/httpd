#include <signal.h>
#include "common.h"
#include "pthread_pool.h"
#include "epoll.h"
#include "http.h"
#include "extend.h"

void* print(void *args)
{
	printf("thread print\n");
}


int main(int argc, char *argv[])
{
	int sockfd = 0;
	struct epoll_event event;

	if(argc < 2)
	{
		printf("input need ip address\n");
		return ERROR;
	}

	config_t config;

#if 0
	/*加载配置*/
	if(loadConfig(&config) == ERROR)
	{
		printf("load config failed\n");
		return ERROR;

	}
#endif
	config.thread_num = 10;

	/*初始化线程池*/
	if(init_pthread_pool(config.thread_num) == ERROR)
	{
		printf("init pthread pool failed\n");
		return ERROR;
	}

	printf("33333\n");

	/*信号处理*/
	init_signal();

	int a = 0;

	add_job(print, &a);
	add_job(print, &a);
	add_job(print, &a);
	add_job(print, &a);


	while (1)
	{
		sleep(100);
	}
#if 0
	if(ERROR == (sockfd = init_server_socket(config.port)))
	{
		printf("init server socket failed\n");
		pool_destory();
		return ERROR;
	}

	int epfd;
	/*epoll初始化*/
	if(ERROR == (epfd = (init_epoll(0))))
	{
		printf("init server socket failed\n");
		pool_destory();
		close(sockfd);
		return ERROR;
	}

	http_request *request = (http_request *)malloc(sizeof(http_request));
	if(!request){	
		close(sockfd);
		pool_destroy();
	}

	init_http_request(request, sockfd, epfd, &config);

	event.data.ptr = (void *)request;
	event.events  = EPOLLIN | EPOLLET;

	int ret =  epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &event);
	if(ERROR == ret)
	{
		printf("epoll add failed\n");
	}

	int i = 0;
	struct sockaddr_in client_addr = {0};
	int client_addr_len = sizeof(client_addr);
	int client_sock;

	for(;;)
	{
		ret = epoll_wait(epfd, events,MAX_EPOLL_FD, -1);
		if(ret < 0)
		{
			printf("epoll_wait failed\n");
			/*how to handle this?*/
		}

		for(i = 0; i < ret; i++)
		{
			http_request *hr = (http_request *)events[i].data.ptr;

			if(hr.sockfd == sockfd)  //新的连接
			{
				client_sock = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);

				http_request *req = (http_request *)malloc(sizeof(http_request));
				event.data.ptr = (void *)req;
				event.events  = EPOLLIN | EPOLLET | EPOLLONESHOT;  //可读 + 边沿触发 + 避免分配给多个线程;

				ret =  epoll_ctl(epfd, EPOLL_CTL_ADD, client_sock, &event);
				if(ERROR == ret)
				{
					printf("epoll add failed\n");
					break;
				}
			}
			else
			{
				if((events[i].events & EPOLLERR) || events[i].events & EPOLLHUP || (!(events[i].events & EPOLLIN))){
					close(hr->sockfd);
					continue;
				}
				
				if(ERROR == add_job(handle_request, events[i].data.ptr))
				{
					continue;
				}
			}
		}
	}

	close(sockfd);
	destory_thread_pool();
#endif
	return OK;

}
