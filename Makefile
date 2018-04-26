# the compiler: gcc for C program
CC=gcc
LIBS = -pthread -lm
CFLAGS  = -Wall
TARGET = rtes2


all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c $(LIBS)

clean:
	$(RM) $(TARGET)
