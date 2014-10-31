
CC = g++
CFLAGS = -c -std=c++11 -stdlib=libc++

CFLAGS 	+= `pkg-config opencv --cflags-only-I`
LDFLAGS += `pkg-config opencv --libs`

kmeans: kmeans.o

kmeans.o: kmeans.cpp
	$(CC)  $(CFLAGS) $< -o $@
