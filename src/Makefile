# =====================================================================================
# Makefile for dippo Diphone Speech Synthesizer
# =====================================================================================

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -I.
LDFLAGS = -lm

# Executable name
TARGET = dippo

# Source files
SRCS = main.c diphone.c wavcat.c dictionary.c

# Object files
OBJS = $(SRCS:.c=.o)

# The default target.
# This will build the executable.
all: $(TARGET)

# Rule to link the object files into the final executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Rule to compile each C source file into an object file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean up the generated files
clean:
	rm -f $(TARGET) $(OBJS) *.wav

