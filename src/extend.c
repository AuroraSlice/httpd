#include "extend.h"
#include "common.h"

int loadConfig(config_t *config)
{
    FILE *fp;

    char line[513];

    int i;
    char *val;
    char *end;

    if((fp = fopen("../http.conf","rt")) == NULL)
    {
        printf("open file failed\n");
        return ERROR;
    }

    while (fget(line, 512, fp))
    {
        while (line[i] == " ") i++;
        if(line[i] == "\n" || line[i] == "#")
            continue;

        val = strstr(line, "=");
        if(val == NULL)
        {
            printf("not found =\n");
            continue;
        }
        val++;
        end = val;

        while (end!= "\0")
        {
            end++;
        }

        if(memcmp(line, "port", strlen("port")) == 0)
        {
            memcpy(&config->port, val, end-val);
        } 
        
        if(memcmp(line, "thread_num", strlen("thread_num")) == 0)
        {
            memcpy(&config->thread_num, val, end-val);
        } 

    }

    printf("------载入配置文件------\nport:%d\nthread num:%d\n",config->port, config->thread_num);

    close(fp);

    return OK;
    
}

int init_pthread_pool(int pthread_num)
{
    
}