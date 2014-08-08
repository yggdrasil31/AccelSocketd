#OBJS = $(SRCS:.c=.o)

# Comment/uncomment the following line to disable/enable debugging
DEBUG = y

TARGET = AccelSocketd
SRCS =	./src/AccelSocketd_main.c \
				./src/AccelSocketd_server.c
OBJS = $(SRCS:.c=.o)

# CC = gcc
# LD = gcc
CFLAGS = -Wall
LDLIBS = -lm

LINUXPATH=$(BUILDROOT_PATH)/output/build/linux-custom
#INC = -I${LINUXPATH}/include/
#INC += -I${BUILDROOT_PATH}/output/host/usr/arm-unknown-linux-gnu/sysroot/usr/include/

ifeq ($(DEBUG),y)
  CFLAGS += -DDEBUG
endif


all: $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

%.o: %.c
#	$(CC) $(INC) $(CFLAGS) -c -o $@ $<
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJS)
	$(LD) $(LDLIBS) -o $@ $(OBJS)

.PHONY: all clean

install:
	cp $(TARGET) $(DESTDIR)/bin/AccelSocketd

# DO NOT DELETE THIS LINE -- make depend needs it
