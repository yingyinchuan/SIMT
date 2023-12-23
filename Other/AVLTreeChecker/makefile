# Makefile for AVLTreeChecker

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

SRC = main.cc
OBJ = $(SRC:.cc=.o)
TARGET = AVLTreeChecker

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(TARGET)
