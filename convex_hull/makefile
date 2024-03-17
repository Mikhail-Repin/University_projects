all : run

malloc.o: malloc.cpp
	wg++ -c malloc.cpp

convex.o: convex.cpp
	wg++ -c convex.cpp

main.o: main.cpp
	wg++ -c main.cpp	

run: convex.o main.o malloc.o
	wg++  convex.o main.o malloc.o -o run

clean:
	rm -rf *.o