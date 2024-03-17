all : run

CDomain.o: CDomain.cpp
	wg++ -c CDomain.cpp

func.o: func.cpp
	wg++ -c func.cpp	

run: CDomain.o func.o
	wg++  CDomain.o func.o -o run

clean:
	rm -rf *.o