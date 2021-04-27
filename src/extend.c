#include "extend.h"

int loadConfig(config_t *config)
{
    FILE *fp;

    char line[513];

    int i;
    char *val;

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
        if(memcpy(line, "port", strlen("port")) == 0)
        {
            config.p
        } 
        
    }
    
}