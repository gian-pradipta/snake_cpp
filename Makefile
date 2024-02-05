
main: main.o snake.o
	g++ -o main main.o snake.o

main.o: main.cpp
	g++ -o main.o -c main.cpp
snake.o: snake.cpp snake.hpp
	g++ -o snake.o -c snake.cpp 
