#include "common.h"


int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("input need ip address\n");
        return 0;
    }

    config_t config;

    /*加载配置*/
    if(loadConfig(&config) == ERROR)
    {
        printf("load config failed\n");
        return 0;

    }

    /*初始化线程池*/
    if(init_pthread_pool(config.thread_num) == ERROR)
    {
        
    }
    



    

}