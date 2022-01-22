all: main

main: main.cpp bin/memory.o
	g++ main.cpp bin/memory.o -o tp2virtual

bin/memory.o: src/memory.cpp
	g++ -c src/memory.cpp -o bin/memory.o

clean:
	rm -rf bin/*.o ./tp2virtual
