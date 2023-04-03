CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

all: main

main: main.cpp
	$(CXX) $(CXXFLAGS) -o main main.cpp

clean:
	rm -f main
