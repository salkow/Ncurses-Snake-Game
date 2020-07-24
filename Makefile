CPPFLAGS := -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wmissing-declarations \
            -Wredundant-decls -Winline \
            -Wuninitialized -g -lncurses

all: main.o snake.o
	g++ $(CPPFLAGS) -o snake snake.o main.o

main.o: main.cpp snake.hpp
	g++ $(CPPFLAGS) -c main.cpp

snake.o: snake.cpp snake.hpp
	g++ $(CPPFLAGS) -c snake.cpp

