# compiler

CC = g++

# compiler flags
# -g	: this flag adds debugging info to the executable file
# -Wall : this flag is used to turn on most compiler warnings
CFLAGS = -g -Wall

TARGET1 = image
TARGET2 = test
# The build target
build:
	$(CC) $(CFLAGS) -c $(TARGET1).cpp $(TARGET2).cpp

all:
	$(CC) $(CFLAGS) $(TARGET1).cpp $(TARGET2).cpp -o out.exe

run:
	./out.exe

clean:
	del $(TARGET).exe