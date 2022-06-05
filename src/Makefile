
CFLAGS = -Wall -std=c++14 -D DEBUG

all: sim

sim: SimMain.o
	g++ -o sim SimMain.o

test: TestMain.o
	g++ -o test TestMain.o

SimMain.o : SimMain.cc
	g++ -c SimMain.cc ${CFLAGS}

TestMain.o : TestMain.cc
	g++ -c TestMain.cc ${CFLAGS}

clean:
	rm -rf *.o sim test
