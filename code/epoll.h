#ifndef _EPOLL_H_
#define _EPOLL_H_

#include <sys/epoll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>

struct http_request
{
	int fd;
	int alive;
	char method[32];
	char path[128];
};

void epoll_add(int, struct http_request *, int);

void epoll_mod(int ,struct http_request *, int);

//void epoll_del(int ,struct http_request *, int);

void set_nonblock(int);

#endif
