
CC = g++
CFLAGS = -g -c -std=c++11 -stdlib=libc++

CFLAGS 	+= `pkg-config opencv --cflags-only-I`
LDFLAGS += `pkg-config opencv --libs`

CFLAGS 	+= `pkg-config lept --cflags-only-I`
LDFLAGS += `pkg-config lept --libs`

test: test.o

test.o: test.cpp
	$(CC)  $(CFLAGS) $< -o $@


