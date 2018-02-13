#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>




#include "socket.h"
//#include "ulog.h"

#define _LOG_ERROR printf

static char buffer[1024 * 64];

int socket_server_open(int port)
{
    int sockfd;
    struct sockaddr_in servaddr = {0};
    int on = 1;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        return -1;

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        return -1;

    if (listen(sockfd, SOMAXCONN) < 0)
        return -1;

    return sockfd;
}

int socket_server_accept(int sockfd)
{
    struct sockaddr_in clientaddr = {0};
    int addrlen = sizeof(clientaddr);

    return accept(sockfd, (struct sockaddr *)&clientaddr, (socklen_t *)&addrlen);
}

int socket_read(int sockfd, char *data, int length)
{
    int len;
    len = recv(sockfd, data, length-1, 0);
    if (len > 0)
        data[len] = '\0';

    return len;
}

static int socket_internal_write(int sockfd, char *data, int length)
{
    int retlen, write_len;

    write_len = 0;
    while (write_len < length) {
        retlen = send(sockfd, &data[write_len], length - write_len, 0);
        if (retlen < 0) {
            return -1;
        }

        write_len += retlen;
    }

    return write_len;
}

int socket_write(int sockfd, const char *fmt, ...)
{
    int count;
    va_list argp;

    va_start(argp, fmt);
    count = vsprintf(buffer, fmt, argp);
    va_end(argp);

    buffer[count] = '\0';

    return socket_internal_write(sockfd, buffer, count);
}

int socket_close(int sockfd)
{
    return close(sockfd);
}


#include <sys/un.h>
#include <sys/stat.h>





/* portable named pipes */
int ortp_server_pipe_create(const char *name)
{
	struct sockaddr_un sa;
	char pname[256];
	char *pipename=pname;//make_pipe_name(name);	
	int sock;
	sock=socket(AF_UNIX,SOCK_STREAM,0);
	sa.sun_family=AF_UNIX;

	snprintf(pname,256,"/tmp/%s",name);
	
	strncpy(sa.sun_path,pipename,sizeof(sa.sun_path)-1);
	unlink(pipename);/*in case we didn't finished properly previous time */
	
	//ortp_free(pipename);
	
	fchmod(sock,S_IRUSR|S_IWUSR);
	if (bind(sock,(struct sockaddr*)&sa,sizeof(sa))!=0){
		_LOG_ERROR("Failed to bind command unix socket: %s",strerror(errno));
		return -1;
	}
	listen(sock,1);
	return sock;
}


int ortp_server_pipe_accept_client(int server)
{
	struct sockaddr_un su;
	socklen_t ssize=sizeof(su);
	int client_sock=accept(server,(struct sockaddr*)&su,&ssize);
	return client_sock;
}


int ortp_server_pipe_close_client(int client){
	return close(client);
}

int ortp_server_pipe_close(int spipe){
	struct sockaddr_un sa;
	socklen_t len=sizeof(sa);
	int err;
	/*this is to retrieve the name of the pipe, in order to unlink the file*/
	err=getsockname(spipe,(struct sockaddr*)&sa,&len);
	if (err==0){
		unlink(sa.sun_path);
	}else _LOG_ERROR("getsockname(): %s",strerror(errno));
	return close(spipe);
}

int ortp_client_pipe_connect(const char *name){
	int sock = -1;
	struct sockaddr_un sa;
	struct stat fstats;
	char pname[256];
	
	char *pipename=pname;//make_pipe_name(name);
	uid_t uid = getuid();

	snprintf(pname,256,"/tmp/%s",name);

	// check that the creator of the pipe is us
	if( (stat(name, &fstats) == 0) && (fstats.st_uid != uid) )
	{
		_LOG_ERROR("UID of file %s (%lu) differs from ours (%lu)", pipename, (unsigned long)fstats.st_uid, (unsigned long)uid);
		return -1;
	}

	sock = socket(AF_UNIX,SOCK_STREAM,0);
	sa.sun_family=AF_UNIX;
	strncpy(sa.sun_path,pipename,sizeof(sa.sun_path)-1);
	//ortp_free(pipename);
	
	if (connect(sock,(struct sockaddr*)&sa,sizeof(sa))!=0){
		close(sock);
		return -1;
	}
	return sock;
}

int ortp_pipe_read(int p, uint8_t *buf, int len){
	return read(p,buf,len);
}

int ortp_pipe_write(int p, const uint8_t *buf, int len){
	return write(p,buf,len);
}

int ortp_client_pipe_close(int sock){
	return close(sock);
}


int __ortp_thread_create(pthread_t *thread, pthread_attr_t *attr, void * (*routine)(void*), void *arg){
	pthread_attr_t my_attr;
	pthread_attr_init(&my_attr);
	if (attr)
		my_attr = *attr;
#ifdef ORTP_DEFAULT_THREAD_STACK_SIZE
	if (ORTP_DEFAULT_THREAD_STACK_SIZE!=0)
		pthread_attr_setstacksize(&my_attr, ORTP_DEFAULT_THREAD_STACK_SIZE);
#endif
	return pthread_create(thread, &my_attr, routine, arg);
}


int __ortp_thread_join(ortp_thread_t thread, void **ptr){
	int err=pthread_join(thread,ptr);
	if (err!=0) {
		printf("pthread_join error: %s",strerror(err));
	}
	return err;
}





