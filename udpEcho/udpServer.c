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

#define SERVER_PORT	((uint16_t)7007)
#define BUFF_SIZE	(1024 * 4)

int udp_echo(int client_fd)
{
    char				buff[BUFF_SIZE]	= {0};
    ssize_t				len				= 0;
    struct sockaddr_in	source_addr;
    socklen_t	addr_len	= sizeof(source_addr);

    (void)memset(&source_addr, 0, addr_len);
    len	= recvfrom(client_fd, buff, BUFF_SIZE, 0,
                   (struct sockaddr *)&source_addr, &addr_len);
    if (len < 1) {
        perror("recvfrom(2) error");
        goto err;
    }

    len	= sendto(client_fd, buff, len, 0,
                 (struct sockaddr *)&source_addr, addr_len);
    if (len < 1) {
        perror("sendto(2) error");
        goto err;
    }

    printf("Served client %s:%hu\n",
           inet_ntoa(source_addr.sin_addr),
           ntohs(source_addr.sin_port));
    fflush(stdout);

    return EXIT_SUCCESS;
 err:
    return EXIT_FAILURE;
}

int main(void)
{
    int server_sock;
    struct sockaddr_in server_addr;
    socklen_t	sock_len	= sizeof(server_addr);

    server_sock	= socket(AF_INET, SOCK_DGRAM, 0);
    if (server_sock < 0) {
        perror("socket(2) error");
        goto create_err;
    }

    (void)memset(&server_addr, 0, sock_len);
    server_addr.sin_family		= AF_INET;
    server_addr.sin_addr.s_addr	= htonl(INADDR_ANY);
    server_addr.sin_port		= htons(SERVER_PORT);

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr))) {
        perror("bind(2) error");
        goto err;
    }

    while (true) {
        if (udp_echo(server_sock) != EXIT_SUCCESS) {
            goto err;
        }
    }

    perror("exit with:");
    close(server_sock);
    return EXIT_SUCCESS;
 err:
    close(server_sock);
 create_err:
    fprintf(stderr, "server error");
    return EXIT_FAILURE;
}
