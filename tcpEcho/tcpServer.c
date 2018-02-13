#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <signal.h>

#define BUFF_SIZE	(1024 * 4)

void zombie_cleaning(int signo)
{
    int status;
    (void)signo;
    while (waitpid(-1, &status, WNOHANG) > 0);
}

int tcp_echo(int client_fd)
{
    char				buff[BUFF_SIZE]	= {0};
    ssize_t				len				= 0;

while(1)
{
    len	= read(client_fd, buff, sizeof(buff));
    if (len < 1) {
        goto err;
    }

    (void)write(client_fd, buff, (size_t)len);
}

    return EXIT_SUCCESS;
 err:
    return EXIT_FAILURE;
}

int main(int argc, char *argv[])
{
    int server_sock, conn_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t	sock_len	= sizeof(client_addr);
    pid_t	chld_pid;
    struct sigaction clean_zombie_act;
    int yes=1;
    uint16_t listen_port;

    if (argc != 2)
    {
        printf("Usage: %s port\n", argv[0]);
        exit(1);
    }

    listen_port = atoi(argv[1]);

    server_sock	= socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("socket(2) error");
        goto create_err;
    }
    else
    {
	printf("create tcp socket. fd = %d\r\n",server_sock);
    }

    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    (void)memset(&server_addr, 0, sock_len);
    server_addr.sin_family		= AF_INET;
    server_addr.sin_addr.s_addr	= htonl(INADDR_ANY);
    server_addr.sin_port		= htons(listen_port);

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr))) {
        perror("bind(2) error");
        goto err;
    }
    else
    {
	printf("bind success!\r\n");
    }

    (void)memset(&clean_zombie_act, 0, sizeof(clean_zombie_act));
    clean_zombie_act.sa_handler	= zombie_cleaning;
    if (sigaction(SIGCHLD, &clean_zombie_act, NULL) < 0) {
        perror("sigaction(2) error");
        goto err;
    }

    if (listen(server_sock, 5)) {
        perror("listen(2) error");
        goto err;
    }
    else
    {
	printf("listen...\r\n");
    }

    while (true) {
        sock_len	= sizeof(client_addr);
        conn_sock	= accept(server_sock, (struct sockaddr *)&client_addr, &sock_len);
        if (conn_sock < 0) {
            if (errno == EINTR) {
                /* restart accept(2) when EINTR */
                continue;
            }
            goto end;
        }

        printf("client from %s:%hu connected\n",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));
        fflush(stdout);

        chld_pid	= fork();
        if (chld_pid < 0) {
            /* fork(2) error */
            perror("fork(2) error");
            close(conn_sock);
            goto err;
        } else if (chld_pid == 0){
            /* child process */
            int ret_code;

            close(server_sock);
            ret_code	= tcp_echo(conn_sock);
            close(conn_sock);

            /* Is usage of inet_ntoa(2) right? why? */
            printf("client from %s:%hu disconnected\n",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));

            exit(ret_code);
        } else {
            /* parent process */
            continue;
        }
    }

 end:
    perror("exit with:");
    close(server_sock);
    return EXIT_SUCCESS;
 err:
    close(server_sock);
 create_err:
    fprintf(stderr, "server error");
    return EXIT_FAILURE;
}

