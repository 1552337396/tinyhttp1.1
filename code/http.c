#include "http.h"
#include "config.h"
#include "http_accept.h"
#include "epoll.h"

struct http_conf config;

int startup(int port, int listen_num)
{
	struct sockaddr_in addr;
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd == -1)
	{
		perror("socket fail!\n");
		return -1;
	}
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	int opt = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
	if(bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		perror("bind fail!\n");
		return -1;
	}
	if(listen(fd, listen_num) == -1)
	{
		perror("listen fail\n");
		return -1;
	}
	return fd;
}
int main()
{
	printf("pid :%d\n", getpid());
	init_conf(&config);
	int *epfds = NULL;
	pthread_t *threads = NULL;
	epfds = (int *)malloc(sizeof(int) * config.thread_num);
	threads = (pthread_t *)malloc(sizeof(pthread_t) * config.thread_num);
	if(epfds == NULL)
	{
		perror("epfd malloc fail!\n");
		return -1;
	}
	if(threads == NULL)
	{
		perror("threads malloc fail!\n");
		free(epfds);
		return -1;
	}
	int serverfd = startup(config.port, config.listen_num);
	for(int i = 0; i < config.thread_num; ++i)
	{
		if((epfds[i] = epoll_create(config.epoll_size)) == -1)
		{
			perror("epoll create fail!\n");
			return -1;
		}
	}
	for(int i = 0; i < config.thread_num; ++i)
	{
		if(pthread_create(&threads[i], NULL, wait_task, (void *)&epfds[i]) == -1)
		{
			perror("thread create fail!\n");
			return -1;
		}
	}
	while(1)
	{
		for(int i = 0; i < config.thread_num; ++i)
		{
			int client = accept(serverfd, NULL, 0);
			if(client == -1)
			{
				perror("server accept fail!\n");
				continue;
			}
			//printf("client %d\n", client);
			struct http_request *ptr = (struct http_request *)malloc(sizeof(struct http_request));
			ptr -> fd = client;
			epoll_add(epfds[i], ptr, EPOLLIN);
		}
	}
	return 0;
}
