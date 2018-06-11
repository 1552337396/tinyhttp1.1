#ifndef _CONFIG_H_
#define _CONFIG_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define default_port 8888
#define default_thread_num 3
#define default_listen_num 1024
#define default_epoll_size 2048

struct http_conf
{
	int port;
	int thread_num;
	int listen_num;
	int epoll_size;
};
void init_conf(struct http_conf *);
void default_init_conf(struct http_conf *);


#endif
