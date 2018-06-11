#include "config.h"
void init_conf(struct http_conf *config)
{
	default_init_conf(config);
	FILE *fp = NULL;
	char line[128];
	char key[128], value[128];
	if((fp = fopen("../etc/conf", "r")) == NULL)
	{
		perror("open conf fail!\n");
		return ;
	}
	while(fgets(line, 128, fp) != NULL)
	{
		sscanf(line, "%s %s", key, value);
		if(strcmp(key, "epoll_size") == 0)
			config -> epoll_size = atoi(value);
		else if(strcmp(key, "port") == 0)
			config -> port = atoi(value);
		else if(strcmp(key, "listen_num") == 0)
			config -> listen_num = atoi(value);
		else if(strcmp(key, "thread_num") == 0)
			config -> thread_num = atoi(value);
	}
	fclose(fp);
	return ;
}
void default_init_conf(struct http_conf *config)
{
	config -> epoll_size = default_epoll_size;
	config -> port = default_port;
	config -> listen_num = default_listen_num;
	config -> thread_num = default_thread_num;
	return ;
}
