CXXFLAGS += -std=c++11

all:program

program:testPipe.cpp
	g++ $(CXXFLAGS) -o $@ $^
clean:
	rm -f program
test:
	./program
.PHONY:all clean test