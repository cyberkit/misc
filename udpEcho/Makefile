TOOLCHAIN=
CC = $(TOOLCHAIN)gcc
CFLAGS=-I./
LDFLAGS=-L/usr/lib
LIBS = -lpthread


TARGET1=udpClient
TARGET2=udpClientAuto
TARGET3=udpServer

all:$(TARGET1) $(TARGET2) $(TARGET3)
.PHONY:all

$(TARGET1):$(TARGET1).o
	$(CC) $(CFLAGS)  -o $(TARGET1) $(TARGET1).o $(LDFLAGS) $(LIBS)
$(TARGET1).o:$(TARGET1).c
	$(CC) $(CFLAGS)  -c $(TARGET1).c

$(TARGET2):$(TARGET2).o
	$(CC) $(CFLAGS)  -o $(TARGET2) $(TARGET2).o $(LDFLAGS) $(LIBS)
$(TARGET2).o:$(TARGET2).c
	$(CC) $(CFLAGS)  -c $(TARGET2).c

$(TARGET3):$(TARGET3).o
	$(CC) $(CFLAGS)  -o $(TARGET3) $(TARGET3).o $(LDFLAGS) $(LIBS)
$(TARGET3).o:$(TARGET3).c
	$(CC) $(CFLAGS)  -c $(TARGET3).c

.PHONY: clean
clean : cleanobj cleantarget
cleanobj :
	@rm -vf *.o
cleantarget:
	@rm -vf $(TARGET1) $(TARGET2) $(TARGET3) *~
#	rm $(TARGET1)
#	rm $(TARGET2)
#	rm $(TARGET3)

