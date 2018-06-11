#include "http_accept.h"
#include <pthread.h>
void *wait_task(void *arg)
{
	int epfd = *(int *)arg;
	struct epoll_event *job = NULL;
	job = (struct epoll_event *)malloc(sizeof(struct epoll_event) * 1024);
	if(job == NULL)
	{
		perror("malloc job list fail!\n");
		pthread_exit(NULL);
	}
	while(1)
	{
		int n;
		if((n = epoll_wait(epfd, job, 1024, -1)) < 0)
		{
			perror("epoll wait failed!\n");
			continue;
		}
		for(int i = 0; i < n; ++i)
		{
			struct http_request *ptr = (struct http_request *)job[i].data.ptr;
			if(job[i].events == EPOLLIN)
			{
				http_recv(ptr, epfd);
			}
			else if(job[i].events == EPOLLOUT)
			{
				http_send(ptr, epfd);
			}
		}
	}
	free(job);
	pthread_exit(NULL);
}
void http_recv(struct http_request *ptr, int epfd)
{
	char buf[10240];
	int fd = ptr -> fd;
	size_t n = recv(fd, buf, 10240, 0);
	//printf("%s", buf);
	if(n < 0)
	{
		perror("recv fail\n");
		free(ptr);
		close(fd);
	}
	else if(n == 0)
	{
		perror("connect close!\n");
		free(ptr);
		close(fd);
	}
	else 
	{
		int i = 0, j = 0;
		while(buf[i] == ' ' && buf[i] != '\0')
			i++;
		while(buf[i] != ' ' && buf[i] != '\0')
		{
			ptr -> method[j] = buf[i];
			i++;
			j++;
		}
		ptr -> method[j] = '\0';
		strcpy(ptr -> path, "../page");
		while(buf[i] == ' ' && buf[i] != '\0')
			i++;
		j = 7; 
		while(buf[i] != ' ' && buf[i] != '\0')
		{
			ptr -> path[j] = buf[i];
			i++;
			j++;
		}
		ptr -> path[j] = '\0';
		if(strstr(buf, "keep-alive") != NULL || strstr(buf, "Keep-Alive") != NULL)
			ptr -> alive = 1;
		//http_send(ptr, epfd);
		epoll_mod(epfd, ptr, EPOLLOUT);
		//printf("method:%s\n", ptr -> method);
		//printf("path:%s\n", ptr -> path);
		//printf("alive:%d\n", ptr -> alive);
	}
	return ;
}
void http_send(struct http_request *ptr, int epfd)
{
	//printf("%d\n", ptr->alive);
	if(strcmp(ptr -> method, "GET") != 0)
	{
		request_stat(ptr -> fd, 405);
		if(ptr -> alive == 1)
		{
			epoll_mod(epfd, ptr, EPOLLIN);
		}
		else 
		{
			close(ptr -> fd);
			free(ptr);
		}
	}
	else 
	{
		char *path = ptr -> path;
		int l = strlen(path);
		if(path[l - 1] == '/')
		{
			strcat(path , "index.html");
		}
		Sendfile(ptr -> fd, path);
		if(ptr -> alive == 1)
		{
			epoll_mod(epfd, ptr, EPOLLIN);
		}
		else 
		{
			close(ptr -> fd);
			free(ptr);
		}
	}
	return ;
}
void Sendfile(int fd, char *path)
{
	//printf("sendfile:%s\n", path);
	struct stat st;
	int l = strlen(path);
	char line[1024];
	char header[1024];
	if(stat(path, &st) == 0)
	{
		if(S_ISREG(st.st_mode))
		{
			int filefd = open(path, O_RDONLY);
			strcpy(line, line200);
			if(strcasecmp(&path[l - 4], ".jpg") == 0)
			{
				sprintf(header, imgheader , st.st_size);
			}
			else 
			{
				sprintf(header, htmlheader, st.st_size);
			}
			strcat(line, header);
			write(fd, line, strlen(line));
			sendfile(fd, filefd, NULL, st.st_size);
			close(filefd);
		}
		else 
		{
			request_stat(fd, 404);
		}
	}
	else 
	{
		request_stat(fd, 404);
	}
	return ;
}
void request_stat(int fd, int stat)
{
	char line[1024];
	char header[1024];
	if(stat == 404)
	{
		strcpy(line, line404);
	}
	else if(stat == 405)
	{
		strcpy(line, line405);
	}
	else if(stat == 500)
	{
		strcpy(line, line500);
	}
	sprintf(header, htmlheader, 0);
	strcat(line, header);
	write(fd, line, strlen(line));
	return ;
}
