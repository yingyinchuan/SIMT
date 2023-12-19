CC = g++
CFLAGS = -std=c++11

SRCS = main.cc
OBJS = $(SRCS:.cc=.o)
TARGET = rock_paper_scissors_game

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.cc
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)
