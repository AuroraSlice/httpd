#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<fcntl.h>

void *handle_request(void *arg);

typedef struct _http_request
{
    int sockfd; //关联的socket
}http_request;