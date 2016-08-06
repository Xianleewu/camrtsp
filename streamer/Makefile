#

CC = gcc
CXX = g++
AR = ar
LD = ld

INCLUDES = \
	-I. \
	-Iinclude \
	-I/usr/include \
	-I/usr/local/include \
	-I/usr/include/liveMedia \
	-I/usr/include/groupsock \
	-I/usr/include/BasicUsageEnvironment \
	-I/usr/include/UsageEnvironment

LIB_PATH = -L/lib -L/usr/lib -L/usr/local/lib
LIBS = \
	-lx264 \
	-lavcodec \
	-lliveMedia \
	-lgroupsock \
	-lBasicUsageEnvironment \
	-lUsageEnvironment \
	-lavutil \
	-lavfilter \
	-lavdevice \
	-lavformat \
	-lswscale \
	-lswresample \
	-lpostproc \
	-lpthread \
	-lrt \
	-lm

CFLAGS = $(INCLUDES) 
CPPFLAGS = $(INCLUDES)
LDFLAGS = $(LIB_PATH) $(LIBS)
BIN_PATH = $(shell pwd)
OBJ_PATH = $(BIN_PATH)/src
EXECUTABLE = streamer

C_SRCS = $(wildcard src/*.c)
CPP_SRCS = $(wildcard src/*.cpp)

C_OBJS = $(patsubst %c, %o, $(C_SRCS))
CPP_OBJS = $(patsubst %cpp, %o, $(CPP_SRCS))
OBJS = $(C_OBJS) $(CPP_OBJS)

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CXX) -o $@ $^ $(CFLAGS) $(LDFLAGS)

%.o:%.c
	$(CC) -Wall -O2 -c -o $@ $< $(CFLAGS)

%.o:%.cpp
	$(CXX) -Wall -O2 -c -o $@ $< $(CPPFLAGS)


clean:
	rm -f  $(BIN_PATH)/$(EXECUTABLE) $(OBJ_PATH)/*.o
