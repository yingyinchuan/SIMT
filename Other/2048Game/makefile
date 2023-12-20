CC = g++
CFLAGS = -std=c++11 -lncurses

SRCS = main.cc
OBJS = $(SRCS:.cc=.o)
TARGET = 2048_game

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.cc
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)
