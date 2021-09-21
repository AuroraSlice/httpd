#include "common.h"
#include "http.h"

#define MAX_CONN_NUM 10

int init_server_socket(int port)
{
	int sock = 0;

	struct sockaddr_in server_addr = {0};

	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);
	server_addr.sin_family = AF_INET;

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sock < 0)
	{
		printf("socket create failed\n");
		return ERROR;
	}

	if(ERROR == bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)))
	{
		printf("socket create failed\n");
		close(sock);
		return ERROR;
	}

	if(ERROR == listen(sock, MAX_CONN_NUM))
	{
		printf("listen socket failed\n");
		close(sock);
		return ERROR;
	}

	return sock;
	
}


/* 处理http请求 */
void *handle_request(void *arg)
{

}

int init_http_request(http_request *request , int sock, int epfd, config_t* config)
{
	request->sockfd = sock;

	return OK;
}