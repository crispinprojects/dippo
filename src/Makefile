# Makefile for dippo speech synthesizer
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -I.
SOURCES = main.c synthesizer.c diphone.c dictionary.c text_processor.c g2p_engine.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = dippo

# Default target
all: $(TARGET)

$(TARGET): $(OBJECTS) 
	$(CC) -o $@ $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) *.wav
