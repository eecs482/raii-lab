CC=g++ -g -Wall -std=c++20

UNAME := $(shell uname -s)
ifeq ($(UNAME),Darwin)
    CC+=-D_XOPEN_SOURCE
    LIBTHREAD=libthread_macos.o
else
    LIBTHREAD=libthread.o
endif

FILE_SIZE_COUNTER_SOURCES=file_size_counter.cpp
FILE_SIZE_COUNTER_OBJS=${FILE_SIZE_COUNTER_SOURCES:.cpp=.o}

all: file_size_counter unique_pointers

file_size_counter: ${FILE_SIZE_COUNTER_OBJS} ${LIBTHREAD}
	${CC} -o $@ $^ -ldl -pthread

UNIQUE_POINTERS_SOURCES=unique_pointers.cpp
UNIQUE_POINTERS_OBJS=${UNIQUE_POINTERS_SOURCES:.cpp=.o}

unique_pointers: ${UNIQUE_POINTERS_OBJS}
	${CC} -o $@ $^

%.o: %.cpp
	${CC} -c $<
%.o: %.cc
	${CC} -c $<

clean:
	rm -f ${FILE_SIZE_COUNTER_OBJS} file_size_counter ${UNIQUE_POINTERS_OBJS} unique_pointers