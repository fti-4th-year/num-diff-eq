.phony: all clean

all: run

run: main.cpp
	g++ -std=c++11 $^ -o $@

clean:
	rm ./run
