#OBJS = $(SRCS:.c=.o)

# Comment/uncomment the following line to disable/enable debugging
DEBUG=y

# Targets definition
DAEMON_TARGET=AccelSocketd
DAEMON_SRCS=/src/AccelSocketd_main.c \
						./src/AccelSocketd_server.c \
						./src/AccelSocketd_i2c.c
DAEMON_OBJS=$(DAEMON_SRCS:.c=.o)


LIB_TARGET=libAccelSocket.a
LIB_SRCS=./src/libAccelSocket.c
LIB_OBJS=$(LIB_SRCS:.c=.o)

CLIENT_TARGET=AccelSocketTestClient
CLIENT_SRCS=./src/AccelSocket_main.c
CLIENT_OBJS=$(CLIENT_SRCS:.c=.o)

# FLAGS
#CFLAGS = -Wall
#LDLIBS = -lm -lpthread
LDLIBS = 

# Folders
DIR_INC=-I./inc
DIR_LIB=-L./

#LINUXPATH=$(BUILDROOT_PATH)/output/build/linux-custom
#INC = -I${LINUXPATH}/include/
#INC += -I${BUILDROOT_PATH}/output/host/usr/arm-unknown-linux-gnu/sysroot/usr/include/

ifeq ($(DEBUG),y)
  CFLAGS+=-DDEBUG
endif


all: $(DAEMON_TARGET) $(LIB_TARGET) $(CLIENT_TARGET)

clean:
	rm -f $(DAEMON_OBJS) $(DAEMON_TARGET)
	rm -f $(LIB_OBJS) $(LIB_TARGET)
	rm -f $(CLIENT_OBJS) $(CLIENT_TARGET)

#%.o: %.c
#	$(CC) $(INC) $(CFLAGS) -c -o $@ $<
#	$(CC) $(CFLAGS) -c -o $@ $<

$(DAEMON_TARGET): $(DAEMON_OBJS)
	$(CC) -o $@ $(DAEMON_OBJS) $(LDLIBS) $(DIR_INC)
	
	
$(LIB_TARGET): $(LIB_OBJS)
	$(CC) -c $(LIB_SRCS) -o $(LIB_OBJS) $(DIR_INC)
	$(AR) rcs $(LIB_TARGET) $(LIB_OBJS)
	
	
$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CC) -o $@ $(CLIENT_OBJS) $(LDLIBS) -L./ -lAccelSocket $(DIR_INC)
	
	
.PHONY: all clean

install:
	cp $(LIB_TARGET) $(DESTDIR)/usr/lib/
	cp $(DIR_INC)/*.h $(DESTDIR)/usr/include/
	
# DO NOT DELETE THIS LINE -- make depend needs it
