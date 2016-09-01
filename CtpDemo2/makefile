cflag = -g -Wall -D _linux
all: CtpDemo2
CtpDemo2: MyMdSpi.o main.o
	g++ -o CtpDemo2 ${cflag} main.o MyMdSpi.o -lthostmduserapi -L .
main.o: main.cpp
	g++ -c ${cflag} main.cpp
MyMdSpi.o: MyMdSpi.cpp
	g++ -c ${cflag} MyMdSpi.cpp
clean :
	rm -rf *.o CtpDemo2 *.dSYM *.con
