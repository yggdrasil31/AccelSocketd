OBJS = $(SRCS:.c=.o)

# Comment/uncomment the following line to disable/enable debugging
DEBUG = y

TARGET = AccelSocketd
SRCS = ./src/AccelSocketd_main.c
OBJS = $(SRCS:.c=.o)

# CC = gcc
# LD = gcc
CFLAGS = -Wall
LDLIBS = -lm

ifeq ($(DEBUG),y)
  CFLAGS += -DDEBUG
endif


all: $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJS)
	$(LD) $(LDLIBS) -o $@ $(OBJS)

.PHONY: all clean

install:
	cp $(TARGET) $(DESTDIR)/bin/AccelSocketd

# DO NOT DELETE THIS LINE -- make depend needs it
