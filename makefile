# 'make depend' uses makedepend to automatically generate dependencies
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files


# define the Build environment
#export CC_ARM = ${BUILDROOT_PATH}/output/host/usr/bin/arm-linux-gcc
#export PATH=$(BUILDROOT_PATH)/output/host/usr/bin:/usr/bin:$PATH
#LINUXPATH=$(BUILDROOT_PATH)/output/build/linux-custom

# define any compile-time flags
CFLAGS = -Wall -g -pthread


# define any directories containing header files other than /usr/include
#
INCLUDES = -I{LINUXPATH}/include/
INCLUDES += -I${BUILDROOT_PATH}/output/host/usr/arm-unknown-linux-gnu/sysroot/usr/include/


# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
#LFLAGS = -L/home/newhall/lib  -L../lib
LFLAGS =


# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname
#   option, something like (this will link in libmylib.so and libm.so:
#LIBS = -lpthread -ldbus-1
LIBS = -lpthread


# define the C source files
SRCS =  ./src/AccelSocketd_main.c


# define the C object files
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
OBJS = $(SRCS:.c=.o)


# define the executable file
EXEC = AccelSocketd


# The following part of the makefile is generic; it can be used to
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'

.PHONY: depend clean

all:			$(EXEC)
					@echo Done compiling $(EXEC)

$(EXEC):	$(OBJS)
					$(GCC) $(CFLAGS) $(INCLUDES) -o $(EXEC) $(OBJS) $(LFLAGS) $(LIBS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file)
# (see the gnu make manual section about automatic variables)
.c.o:
					$(GCC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
					$(RM) *.o *~ $(EXEC)

depend:		$(SRCS)
					makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it
