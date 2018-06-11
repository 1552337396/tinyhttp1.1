#ifndef _HTTP_ACCEPT_H_
#define _HTTP_ACCEPT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sendfile.h>
#include <errno.h>
#include "epoll.h"


#define line200 "HTTP/1.1 200 OK\r\n"
#define line404 "HTTP/1.1 404 NOT FOUND\r\n"
#define line405 "HTTP/1.1 405 NOT ALLOW\r\n"
#define line500 "HTTP/1.1 500 SERVER ERROR\r\n"
#define htmlheader "Server: http1.1\r\nConnection: Keep-Alive\r\nContent-Type: text/html\r\nContent-Length: %lu\r\n\r\n"
#define imgheader "Server: http1.1\r\nConnection: Keep-Alive\r\nContent-Type: image/jpeg\r\nContent-Length: %lu\r\n\r\n"

void http_recv(struct http_request*, int);
void http_send(struct http_request*, int);
void *wait_task(void *);
void request_stat(int ,int);
void Sendfile(int , char *);

#endif
