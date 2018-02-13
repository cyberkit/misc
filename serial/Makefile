TOOLCHAIN=
CC = $(TOOLCHAIN)gcc
CFLAGS=-I./ -Wall
LDFLAGS=-L/usr/lib
#LIBS = -lpthread
LIBS=

TARGETNAME=serial

all:$(TARGETNAME)
.PHONY:all
$(TARGETNAME):$(TARGETNAME).o
	$(CC) $(CFLAGS)  -o $(TARGETNAME) $(TARGETNAME).o $(LDFLAGS) $(LIBS)

$(TARGETNAME).o:$(TARGETNAME).c
	$(CC) $(CFLAGS)  -c $(TARGETNAME).c $(LDFLAGS) $(LIBS)

.PHONY: clean
clean : cleanobj cleantarget
cleanobj :
	@rm -vf *.o
cleantarget:
	@rm -vf $(TARGETNAME)
