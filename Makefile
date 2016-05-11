all: main

main: main.cpp
	g++ main.cpp -o main -lncurses -pthread
clean:
	rm -rf *.o main
