CXXFLAGS += -std=c++11

all:program

program:ProcessPool.cc
	g++ $(CXXFLAGS) -o $@ $^
clean:
	rm -f program
test:
	./program
.PHONY:all clean test