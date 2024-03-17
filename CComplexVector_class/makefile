all : run

malloc.o: malloc.cpp
	wg++ -c malloc.cpp

func.o: func.cpp
	wg++ -c func.cpp

CComplexVector.o: CComplexVector.cpp
	wg++ -c CComplexVector.cpp

run: func.o CComplexVector.o malloc.o
	wg++ func.o CComplexVector.o malloc.o -o run

clean:
	rm -rf *.o