#ifndef _MY_SOCKET_H_
#define _MY_SOCKET_H_

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <pthread.h>



#undef TRUE
#undef FALSE
#define TRUE 1
#define FALSE 0


typedef pthread_t ortp_thread_t;
typedef pthread_mutex_t ortp_mutex_t;
typedef pthread_cond_t ortp_cond_t;


typedef int ortp_pipe_t;
#define ORTP_PIPE_INVALID (-1)



#if 0

#define CHECK_FORMAT_ARGS(m,n) __attribute__((format(printf,m,n)))


static inline void CHECK_FORMAT_ARGS(1,2) ortp_message(const char *fmt,...)
{
	va_list args;
	va_start (args, fmt);
	ortp_logv(ORTP_MESSAGE, fmt, args);
	va_end (args);
}

#endif


int socket_server_open(int port);
int socket_server_accept(int sockfd);
int socket_read(int sockfd, char *data, int length);
int socket_write(int sockfd, const char *fmt, ...);
int socket_close(int sockfd);



int ortp_server_pipe_create(const char *name);
/*
* warning: on win32 ortp_server_pipe_accept_client() might return INVALID_HANDLE_VALUE without
* any specific error, this happens when ortp_server_pipe_close() is called on another pipe.
* This pipe api is not thread-safe.
*/
int ortp_server_pipe_accept_client(int server);
int ortp_server_pipe_close(int spipe);
int ortp_server_pipe_close_client(int client);

int ortp_client_pipe_connect(const char *name);
int ortp_client_pipe_close(int sock);

int ortp_pipe_read(int p, uint8_t *buf, int len);
int ortp_pipe_write(int p, const uint8_t *buf, int len);


#define ortp_thread_create	__ortp_thread_create
#define ortp_thread_join	__ortp_thread_join
#define ortp_thread_exit	pthread_exit
#define ortp_mutex_init		pthread_mutex_init
#define ortp_mutex_lock		pthread_mutex_lock
#define ortp_mutex_unlock	pthread_mutex_unlock
#define ortp_mutex_destroy	pthread_mutex_destroy
#define ortp_cond_init		pthread_cond_init
#define ortp_cond_signal	pthread_cond_signal
#define ortp_cond_broadcast	pthread_cond_broadcast
#define ortp_cond_wait		pthread_cond_wait
#define ortp_cond_destroy	pthread_cond_destroy


#endif /* end of include guard: _SOCKET_H_ */


