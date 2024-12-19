INC_FOLDER=include# contains .h
SRC_FOLDER=src# contains .c
BIN_FOLDER=bin# contains .o

CC = gcc
CFLAGS = -Wall -I$(INC_FOLDER)

# ArgSolver or ArgSolver.exe depend of OS
TARGET=ArgSolver

OS := unknown
ifdef ComSpec
    OS := Windows
else
    OS := $(shell uname -s || echo Unknown)
endif

SOURCES := $(wildcard $(SRC_FOLDER)/*.c)
OBJECTS := $(patsubst $(SRC_FOLDER)/%.c,$(BIN_FOLDER)/%.o,$(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
ifeq ($(OS),Windows)
	$(CC) $(OBJECTS) -o $(TARGET).exe $(LDFLAGS)
else
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
endif

$(BIN_FOLDER)/%.o: $(SRC_FOLDER)/%.c
ifeq ($(OS),Windows)
	@if not exist $(BIN_FOLDER) mkdir $(BIN_FOLDER)
else
	mkdir -p $(BIN_FOLDER)
endif
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY : all clean

clean:
ifeq ($(OS),Windows)
	del /Q /F bin\* 2>NUL || true
	del /Q /F $(TARGET).exe 2>NUL || true
else
	rm -rf bin/*
	rm -f $(TARGET)
endif