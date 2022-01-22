all: main

main: main2.cpp bin/memory.o
	g++ main2.cpp bin/memory.o -o tp2virtual

bin/memory.o: src/memory.cpp
	g++ -c src/memory.cpp -o bin/memory.o

clean:
	rm -rf bin/*.o ./tp2virtual
