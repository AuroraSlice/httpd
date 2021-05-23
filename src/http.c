#include "common.h"

#define MAX_CONN_NUM 10

int init_server_socket(int port)
{
    int sock = 0;

    struct sockaddr_in server_addr = {0};

    server_addr.sin_addr.s_addr = htnol(INADDR_ANY);
    server_addr.sin_port = htnol(port);
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