#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>
#include <net/if.h>
#include <linux/sockios.h>
#include <sys/ioctl.h>

#define BUF_SIZE    1024
#define INTERFAXENAME "eth0"

int sock;

void * network_recv_thread(void * arg)
{
    int n = 0;
    char buff[BUF_SIZE];
    struct sockaddr_in peer;
    int len = sizeof(peer);

    while(1)
    {
        n = recvfrom(sock, buff, BUF_SIZE, 0, (struct sockaddr *)&peer, &len);
        if (n>0)
        {
            buff[n] = 0;
            printf("received:");
            puts(buff);
        }
        else if (n==0)
        {
            printf("server closed\n");
            close(sock);
            break;
        }
        else if (n == -1)
        {
            perror("recvfrom");
            close(sock);
            break;
        }
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    int ret = 0;
    
    if (argc != 4)
    {
        printf("Usage: %s ip port interface\n", argv[0]);
        exit(1);
    }
    
    printf("This is a UDP client\n");

    if ( (sock=socket(AF_INET, SOCK_DGRAM, 0)) <0)
    {
        perror("socket");
        exit(1);
    }
    
    struct ifreq interface;

    //strncpy(interface.ifr_ifrn.ifrn_name, INTERFAXENAME, sizeof(INTERFAXENAME));
    strcpy(interface.ifr_ifrn.ifrn_name, argv[3]);
    if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, (char *)&interface, sizeof(interface))  < 0) {
           perror("SO_BINDTODEVICE failed");
    }
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    if (addr.sin_addr.s_addr == INADDR_NONE)
    {
        printf("Incorrect ip address!");
        close(sock);
        exit(1);
    }

    pthread_t pid;
    ret = pthread_create(&pid, NULL,network_recv_thread, NULL);
    if (ret)
    {
        printf("Create pthread error!\n");
        return 0;
    }
    
    char buff[BUF_SIZE];
    int n = 0;

    while (1)
    {
        gets(buff);
	if(strcmp(buff,"exit")==0)
	{
	    close(sock);
	    break;
	}

        n = sendto(sock, buff, strlen(buff), 0, (struct sockaddr *)&addr, sizeof(addr));
        if (n < 0)
        {
            perror("sendto");
            close(sock);
            break;
        }
    }

    pthread_join(pid, NULL);
    printf("good bye!\r\n");
    return 0;
}

