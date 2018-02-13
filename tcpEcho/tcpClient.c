#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>


#define BUFF_SIZE	(1024)

uint32_t nsend=0;
uint32_t nrecv=0;

int main(int argc, char *argv[])
{
    int sock;
    char buff[128];
    char test_str[BUFF_SIZE]	= "tcp echo test";
    struct sockaddr_in	server_addr;
    struct ifreq interface;

    int ret = 0;
    time_t t;
    struct tm *t_tm;
    int delay=10;
    nsend = 0;
    nrecv = 0;
    int yes = 1;
    int n;

    if (argc != 5)
    {
        printf("Usage: %s ip port interface\n", argv[0]);
        exit(1);
    }

    delay = atoi(argv[4]);

_create_sock:
    sock	= socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket(2) error");
        goto create_err;
    }

    strcpy(interface.ifr_ifrn.ifrn_name, argv[3]);
    if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, (char *)&interface, sizeof(interface))  < 0) {
           perror("SO_BINDTODEVICE failed");
    }
    else
    {
	printf("BINDTODEVICE : [%s] success!\r\n",interface.ifr_ifrn.ifrn_name);
    }

    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));


    (void)memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family		= AF_INET;
    server_addr.sin_port		= htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    if (server_addr.sin_addr.s_addr == INADDR_NONE)
    {
        printf("Incorrect ip address!");
        close(sock);
        exit(1);
    }


    printf("connecting server...\r\n");
    if (connect(sock,
                (struct sockaddr *)&server_addr,
                sizeof(server_addr)) < 0) {
        perror("connect(2) error");
	close(sock);
        sleep(15);
      
        goto _create_sock;
    }
    else
    {
	printf("connected!\r\n");
    }

while(1)
{
	t = time(NULL);
	t_tm = localtime(&t);	
    	/* generate timestamp */
	sprintf(buff, "%04d-%02d-%02d %02d:%02d:%02d",
                    t_tm->tm_year + 1900,
                    t_tm->tm_mon + 1,
                    t_tm->tm_mday,
                    t_tm->tm_hour,
                    t_tm->tm_min,
                    t_tm->tm_sec);

    if (write(sock, buff, strlen(buff)) < 0) {
        perror("send data error");
        close(sock);
        goto _create_sock;
    }
    nsend++;
    printf("send : %s\t%d\r\n",buff,nsend);

    //(void)memset(test_str, 0, BUFF_SIZE);
    n = read(sock, test_str, BUFF_SIZE);
    if (n < 0) {
        perror("receive data error");
        close(sock);
        goto _create_sock;
    }
    else if(n==0)
    {
        close(sock);
        goto _create_sock;
    }

    nrecv++;

    test_str[n] = 0;
    printf("recv : %s\t%d\t%d\r\n",test_str,nrecv,nsend-nrecv);

    fflush(stdout);

    sleep(delay);
}
    return EXIT_SUCCESS;

 err:
    close(sock);
 create_err:
    fprintf(stderr, "client error");
    return EXIT_FAILURE;
}

