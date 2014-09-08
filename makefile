#OBJS = $(SRCS:.c=.o)

# Comment/uncomment the following line to disable/enable debugging
DEBUG = y

# Targets definition
DAEMON_TARGET = AccelSocketd
DAEMON_SRCS =	./src/AccelSocketd_main.c \
							./src/AccelSocketd_server.c \
							./src/AccelSocketd_i2c.c
DAEMON_OBJS = $(DAEMON_SRCS:.c=.o)

CLIENT_TARGET = AccelSocketClient
CLIENT_SRCS =	./src/AccelSocket_main.c \
							./src/libAccelSocket.c
CLIENT_OBJS = $(CLIENT_SRCS:.c=.o)

# FLAGS
CFLAGS = -Wall
#LDLIBS = -lm -lpthread
LDLIBS = 


#LINUXPATH=$(BUILDROOT_PATH)/output/build/linux-custom
#INC = -I${LINUXPATH}/include/
#INC += -I${BUILDROOT_PATH}/output/host/usr/arm-unknown-linux-gnu/sysroot/usr/include/

ifeq ($(DEBUG),y)
  CFLAGS += -DDEBUG
endif


all: $(DAEMON_TARGET) $(CLIENT_TARGET)

clean:
	rm -f $(DAEMON_OBJS) $(DAEMON_TARGET)
	rm -f $(CLIENT_OBJS) $(CLIENT_TARGET)

%.o: %.c
#	$(CC) $(INC) $(CFLAGS) -c -o $@ $<
	$(CC) $(CFLAGS) -c -o $@ $<

$(DAEMON_TARGET): $(DAEMON_OBJS)
	$(CC) -o $@ $(DAEMON_OBJS) $(LDLIBS)
	
$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CC) -o $@ $(CLIENT_OBJS) $(LDLIBS)

.PHONY: all clean

install:
	cp $(DAEMON_TARGET) $(DESTDIR)/bin/AccelSocketd
	cp $(CLIENT_TARGET) $(DESTDIR)/bin/AccelSocketClient

# DO NOT DELETE THIS LINE -- make depend needs it
