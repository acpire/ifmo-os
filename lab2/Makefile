.PHONY: all clean
CC=g++
CFLAGS=-c -Wall
LDFLAGS=
CPP_FILES := $(wildcard ./*.cpp)
OBJ_FILES := $(CPP_FILES:.cpp=.o)
EXE_FILES := parent child

all: $(EXE_FILES)

clean:
	@rm $(EXE_FILES) *.o

$(EXE_FILES): $(OBJ_FILES)
	@$(CC) $(LDFLAGS) $@.o -o $@

%.o: %.cpp
	@$(CC) $(CFLAGS) $< -o $@

