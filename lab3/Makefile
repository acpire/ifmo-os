.PHONY: all clean
CC=g++
CFLAGS=-c -Wall -g
LDFLAGS=-pthread
CPP_FILES := TWorker.cpp ConnectionThread.cpp ThreadPool.cpp MessageLog.cpp lab3server.cpp
OBJ_FILES := $(CPP_FILES:.cpp=.o)
EXECUTABLE := lab3server

all: $(CCP_FILES) $(EXECUTABLE)

clean:
	@rm $(EXECUTABLE) *.o

$(EXECUTABLE): $(OBJ_FILES)
	@$(CC) $(LDFLAGS) $(OBJ_FILES) -o $@

.cpp.o:
	@$(CC) $(CFLAGS) $< -o $@

