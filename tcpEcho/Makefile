TOOLCHAIN=
CC = $(TOOLCHAIN)gcc
CFLAGS=-I./ -Wall

LDFLAGS=
#LDFLAGS+=-L/usr/lib
LIBS=
LIBS += -lpthread

TARGET=tcpServer
TARGET2=tcpClient

all:$(TARGET) $(TARGET2)
.PHONY:all

$(TARGET):$(TARGET).o
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o $(LDFLAGS) $(LIBS)

$(TARGET).o:$(TARGET).c
	$(CC) $(CFLAGS) -c $(TARGET).c $(LDFLAGS) $(LIBS)

$(TARGET2):$(TARGET2).o
	$(CC) $(CFLAGS) -o $(TARGET2) $(TARGET2).o $(LDFLAGS) $(LIBS)

$(TARGET2).o:$(TARGET2).c
	$(CC) $(CFLAGS) -c $(TARGET2).c $(LDFLAGS) $(LIBS)



.PHONY: clean
clean : cleanobj cleantarget
cleanobj :
	@rm -vf *.o
cleantarget:
	@rm -vf $(TARGET) $(TARGET2) *~
