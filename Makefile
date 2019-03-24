CC = g++
CFLAGS = -std=c++11 -O3 -Wall -lm
all:
	g++ ${CFLAGS} -o proj list1.cpp

