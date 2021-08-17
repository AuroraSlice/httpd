#include "common.h"
#include "pthread_pool.h"
#include "epoll.h"
#include "http.h"


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

    /*加载配置*/
    if(loadConfig(&config) == ERROR)
    {
        printf("load config failed\n");
        return ERROR;

    }

    /*初始化线程池*/
    if(init_pthread_pool(config.thread_num) == ERROR)
    {
        printf("init pthread pool failed\n");
        return ERROR;
    }

    /*信号处理*/
    init_signal();

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
    
    for(;;)
    {
        ret = epoll_wait(epfd, events, MAX_EPOLL_FD, -1);
        for(i = 0; i < ret; i++)
        {
            http_request *hr = (http_request *)events[i].data.ptr;
            /*连接请求*/
            if(hr->sockfd == sockfd)
            {
                int client_fd = accept(sockfd, (struct sockaddr *)&client_addr, sizeof(client_addr));
                if(client_fd == ERROR)
                {
                        printf("accept failed\n");
                        break;
                }

                /* 初始化操作 */
				set_non_blocking(client_fd);

                http_request *request = (http_request *)malloc(sizeof(http_request));
                if(request == NULL){
                    printf("alloc memory request fail");
                    break;
                }

                init_http_request(request, client_fd, epfd, &config);
                event.data.ptr = (void *)request;
				event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;  //可读 + 边沿触发 + 避免分配给多个线程
                if(ERROR == (ret = epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &event)))
                {
					printf("epoll_ctl fail");
					break;
				}
            }
            else
            {
                if((events[i].events & EPOLLERR) || events[i].events & EPOLLHUP || (!(events[i].events & EPOLLIN))){
					close(hr->sockfd);
					continue;
				}
				if(ERROR == add_job(handle_request, events[i].data.ptr)) continue;
            }
        }
    }

    close(sockfd);
    pool_destroy();

    return OK;

}