CC = gcc
CFLAGS  = -g -Wall
TARGET = tv

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)

run:
	./$(TARGET)
	
