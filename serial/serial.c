#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>  
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <termios.h>
#include <errno.h>   
#include <limits.h> 
#include <asm/ioctls.h>
#include <time.h>
#include <pthread.h>

#define DATA_LEN                0x00  /* test data's len              */

static int openSerial(char *cSerialName)
{
    int iFd;

    struct termios opt; 

    iFd = open(cSerialName, O_RDWR | O_NOCTTY);                        
    if(iFd < 0) {
        perror(cSerialName);
        return -1;
    }

    tcgetattr(iFd, &opt);      

	cfsetispeed(&opt, B230400);
    cfsetospeed(&opt, B230400);

    /*
     * raw mode
     */
    opt.c_lflag   &=   ~(ECHO   |   ICANON   |   IEXTEN   |   ISIG);
    opt.c_iflag   &=   ~(BRKINT   |   ICRNL   |   INPCK   |   ISTRIP   |   IXON);
    opt.c_oflag   &=   ~(OPOST);
    opt.c_cflag   &=   ~(CSIZE   |   PARENB);
    opt.c_cflag   |=   CS8;

    /*
     * 'DATA_LEN' bytes can be read by serial
     */
    opt.c_cc[VMIN]   =   DATA_LEN;                                      
    opt.c_cc[VTIME]  =   5;

    if (tcsetattr(iFd,   TCSANOW,   &opt)<0) 
	{
        return   -1;
    }

    return iFd;
}

int main(void) 
{
	char tmp[1024];
	int len;
	int fd, i;

	printf("B0     \t%X\r\n",B0);
	printf("B50    \t%X\r\n",B50);
	printf("B75    \t%X\r\n",B75);
	printf("B110   \t%X\r\n",B110);
	printf("B134   \t%X\r\n",B134);
	printf("B150   \t%X\r\n",B150);
	printf("B200   \t%X\r\n",B200);
	printf("B300   \t%X\r\n",B300);
	printf("B600   \t%X\r\n",B600);
	printf("B1200  \t%X\r\n",B1200);
	printf("B1800  \t%X\r\n",B1800);
	printf("B2400  \t%X\r\n",B2400);
	printf("B4800  \t%X\r\n",B4800);
	printf("B9600  \t%X\r\n",B9600);
	printf("B19200 \t%X\r\n",B19200);
	printf("B38400 \t%X\r\n",B38400);
	printf("B57600 \t%X\r\n",B57600);
	printf("B115200\t%X\r\n",B115200);
	printf("B230400\t%X\r\n",B230400);


	fd = openSerial("/dev/ttyS1");
	sprintf(tmp,"hello world\r\n");
	write(fd, tmp, strlen(tmp));

	while (1) 
	{
		len = read(fd, tmp, 0x01);	
		/*
		for(i = 0; i < len; i++)
			printf("key=%c(%d,%02X)", tmp[i],tmp[i],tmp[i]);
		printf("\n");
		*/
		for(i=0;i<len;i++)
		{
			printf("%c",tmp[i]);
		}
		fflush(stdout);
	}
}

