#include "epoll.h"

void epoll_add(int epfd, struct http_request *request, int stat)
{
	struct epoll_event event;
	memset(&event, 0, sizeof(event));
	event.data.ptr = request;
	event.events = stat;
	set_nonblock(request -> fd);
	if(epoll_ctl(epfd, EPOLL_CTL_ADD, request -> fd, &event) == -1)
	{
		perror("epoll add fail!\n");
	}
	return ;
}
void epoll_mod(int epfd, struct http_request *request, int stat)
{
	struct epoll_event event;
	memset(&event, 0, sizeof(event));
	event.data.ptr = request;
	event.events = stat;
	if(epoll_ctl(epfd, EPOLL_CTL_MOD, request -> fd, &event) == -1)
	{
		perror("epoll mod fail!\n");
	}
	return ;
}
/*void epoll_del(int epfd, struct http_request *request, int stat)
{
	struct epoll_event event;
	memset(&event, 0, sizeof(event));
	event.data.ptr = request;
	event.events = stat;
	if(epoll_ctl(epfd, EPOLL_CTL_DEL, request -> fd, &event) == -1)
	{
		perror("epoll del fail!\n");
	}
	return ;
}*/
void set_nonblock(int fd)
{
	int flag = fcntl(fd, F_GETFL, 0);
	if(flag == -1)
	{
		perror("get flag fail!\n");
		return ;
	}
	if(fcntl(fd, F_SETFL, flag | O_NONBLOCK) == -1)
		perror("set nonblock fail!\n");
	return ;
}
