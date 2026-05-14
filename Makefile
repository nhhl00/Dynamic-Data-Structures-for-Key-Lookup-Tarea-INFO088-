all: solucion1 solucion2

solucion1: solucion1.cpp
	g++ -o solucion1 solucion1.cpp

solucion2: solucion2.cpp
	g++ -o solucion2 solucion2.cpp

clean:
	rm -f solucion1 solucion2
