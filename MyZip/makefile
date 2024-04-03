CXX = g++

CXXFLAGS = -Wall -Wextra -std=c++11

TARGET = program

SRCS = main.cc

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS)

clean:
	rm -f $(TARGET)
	rm -f test.myzip unzip.txt

test:
	./program
