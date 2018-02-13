#################################################################
#					Toolchain configuration						#
#################################################################
TOOLCHAIN = mipsel-openwrt-linux-
CC = $(TOOLCHAIN)gcc
AS = $(TOOLCHAIN)gcc
AR = $(TOOLCHAIN)ar
LD = $(TOOLCHAIN)gcc
RM = rm -f

#################################################################
#					Project configuration						#
#################################################################

PROJ_NAME=shell
# C source files
C_SRCS =
C_SRCS += shell.c
C_SRCS += webserver/socket.c


#################################################################
#						Do formatting							#
#################################################################
LIBS = -Wl,--start-group -lpthread -ldl -lm -Wl,--end-group
LDFLAGS = -L/work/openwrt/staging_dir/target-mipsel_24kec+dsp_uClibc-0.9.33.2/usr/lib/


# flags for C compiler  -Wall -Werror
CFLAGS = -I./ 
CFLAGS = -I./webserver
CFLAGS+=-I/work/openwrt/staging_dir/target-mipsel_24kec+dsp_uClibc-0.9.33.2/usr/include

CFLAGS+= -g -std=c99


OBJS = $(patsubst %.c,%.o,$(C_SRCS))
# ROOT=$(shell pwd)

.PHONY: clean

all:  $(PROJ_NAME)

#####################################
#			.o -> .elf				#
#####################################
$(PROJ_NAME): $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@ $(LIBS)
	cp $(PROJ_NAME) /work/dev/spi/

#####################################
#			.c -> .o				#
#####################################
%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	@echo Remove *.o
	@$(RM) -f $(OBJS) $(PROJ_NAME)
	@echo Done.

	
