all : run

malloc.o: malloc.cpp
	wg++ -c malloc.cpp

func.o: func.cpp
	wg++ -c func.cpp

main.o: main.cpp
	wg++ -c main.cpp	

run: main.o func.o malloc.o
	wg++  main.o func.o malloc.o -o run

clean:
	rm -rf *.o