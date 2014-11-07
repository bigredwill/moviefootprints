
CC = g++
CFLAGS = -c -std=c++11 -stdlib=libc++

# opencv
CFLAGS 	+= `pkg-config opencv --cflags-only-I`
LDFLAGS += `pkg-config opencv --libs`

# jsoncpp
CFLAGS 	+= `pkg-config jsoncpp --cflags-only-I`
LDFLAGS += `pkg-config jsoncpp --libs`

footprint: footprint.o

footprint.o: footprint.cpp
	$(CC)  $(CFLAGS) $< -o $@
