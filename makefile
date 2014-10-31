
CC = g++
CFLAGS = -c -std=c++11 -stdlib=libc++

CFLAGS 	+= `pkg-config opencv --cflags-only-I`
LDFLAGS += `pkg-config opencv --libs`

headtrack: headtrack.o

headtrack.o: headtrack.cpp
	$(CC)  $(CFLAGS) $< -o $@
