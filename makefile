# Comment/uncomment the following line to disable/enable debugging
DEBUG=y
ifeq ($(DEBUG),y)
	EXTRA_CFLAGS =-DDEBUG
else
	EXTRA_CFLAGS =
endif


# List of files to compile
DAEMON_TARGET=AccelSocketd
DAEMON_SRCS=./src/AccelSocketd_main.c \
						./src/AccelSocketd_i2c.c \
						./src/AccelSocketd_server.c
DAEMON_OBJS=$(DAEMON_SRCS:.c=.o)

LIB_TARGET=libAccelSocket.a
LIB_SRCS=./src/libAccelSocket.c
LIB_OBJS=$(LIB_SRCS:.c=.o)

CLIENT_TARGET=AccelSocketTestClient
CLIENT_SRCS=./src/AccelSocket_main.c
CLIENT_OBJS=$(CLIENT_SRCS:.c=.o)


# Include folder
INC=./inc

# Libs folder
LDLIBS=

# CFlags
EXTRA_CFLAGS+=


# Pattern rule to generate .o out of .c
%.o: %.c
	$(CC) -I$(INC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $<


# Target definition
all: $(DAEMON_TARGET) $(LIB_TARGET) $(CLIENT_TARGET)

clean:
	rm -f $(DAEMON_OBJS) $(DAEMON_TARGET)
	rm -f $(LIB_OBJS) $(LIB_TARGET)
	rm -f $(CLIENT_OBJS) $(CLIENT_TARGET)

$(DAEMON_TARGET): $(DAEMON_OBJS)
	$(CC) -o $@ $(DAEMON_OBJS)
	
$(LIB_TARGET): $(LIB_OBJS)
	$(CC) -I$(INC) $(CFLAGS) $(EXTRA_CFLAGS) -c $(LIB_SRCS) -o $(LIB_OBJS)
	$(AR) rcs $(LIB_TARGET) $(LIB_OBJS)
	
$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CC) -I$(INC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $(CLIENT_OBJS) $(LDLIBS) -L./ -lAccelSocket

install:
	cp $(LIB_TARGET) $(DESTDIR)/usr/lib/
	cp $(INC)/*.h $(DESTDIR)/usr/include/

# DO NOT DELETE THIS LINE -- make depend needs it
