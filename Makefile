CC = gcc
CXX = g++

INCLUDE_PATH = \
	-I. \
	-Iinclude \
	-I/usr/include \
	-I/usr/local/include \
	-I/usr/include/liveMedia \
	-I/usr/include/groupsock \
	-I/usr/include/BasicUsageEnvironment \
	-I/usr/include/UsageEnvironment

LIB_PATH = -L/lib -L/usr/lib -L/usr/local/lib
CFLAGS = $(INCLUDE_PATH) -DLINUX -std=gnu99
CPPFLAGS = 
LDFLAGS = $(LD_PATH) -lx264 -lv4l1 -lv4l2
EXECUTABLE = encoder

$(EXECUTABLE):main.o
	$(CC) -O2 -o $@ $^ $(CFLAGS) $(LDFLAGS)

%.o:%.c
	$(CC) -O2 -c -o $@ $< $(CFLAGS)

clean:
	rm -f  *.o $(EXECUTABLE)
