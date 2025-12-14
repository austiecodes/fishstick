# Makefile for the project

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall

# Source files
SRCS = src/main.cpp

# Object files directory
OBJ_DIR = build
# Executable name
TARGET = build/main

# Phony targets
.PHONY: all build run clean

all: build

build: $(TARGET)

$(TARGET): $(SRCS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

run: build
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR)
