# Makefile for C++ Speech Synthesizer
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -O2 -pthread
TARGET = dippo
SOURCES = main.cpp g2p_engine.cpp diphone.cpp synthesizer.cpp text_processor.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Link object files into executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) -lm

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)

# Run the application
run: $(TARGET)
	./$(TARGET)

# Run with text argument
run-text: $(TARGET)
	./$(TARGET) "hello world"

.PHONY: all clean run run-text
