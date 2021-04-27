#include "common.h"


int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("input need ip address\n");
        return 0;
    }

    config_t config;

    if(load(&config) == ERROR)
    {
        printf("load config failed\n");
        return 0;

    }

    

}