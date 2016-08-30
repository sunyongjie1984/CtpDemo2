cflag = -g -Wall -std=c++11 -D _linux
all: CtpDemo1
CtpDemo1: MyTraderSpi.o main.o
	g++ -o CtpDemo1 ${cflag} main.o MyTraderSpi.o -lthosttraderapi -L .
main.o: main.cpp
	g++ -c ${cflag} main.cpp
MyTraderSpi.o: MyTraderSpi.cpp
	g++ -c ${cflag} MyTraderSpi.cpp
clean :
	rm -rf *.o CtpDemo1 *.dSYM *.log *.con
