
CC = g++
CFLAGS = -c -std=c++11 -stdlib=libc++

CFLAGS 	+= `pkg-config opencv --cflags-only-I`
LDFLAGS += `pkg-config opencv --libs`

CFLAGS 	+= `pkg-config lept --cflags-only-I`
LDFLAGS += `pkg-config lept --libs`

videofootprint: videofootprint.o

videofootprint.o: videofootprint.cpp
	$(CC)  $(CFLAGS) $< -o $@


