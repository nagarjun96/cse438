# the compiler: gcc for C program
CC = gcc
LIBS = -pthread -lm -lrt
CFLAGS  = -Wall
DEPS = task3_2.h
TARGET = task3_2


all: $(TARGET)

$(TARGET): $(TARGET).c $(DEPS)
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c $(LIBS)

clean:
	$(RM) $(TARGET)
