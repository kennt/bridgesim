
CFLAGS = -Wall -std=c++14 -D DEBUG -g

all: sim

sim: sim_main.o \
	bridge_sim.o \
	config.o \
	strategy.o
	g++ -o sim sim_main.o bridge_sim.o config.o strategy.o

sim_main.o : sim_main.cc \
	bridge_sim.h \
	config.h \
	strategy.h
	g++ -c sim_main.cc ${CFLAGS}

bridge_sim.o : bridge_sim.cc \
	bridge_sim.h
	g++ -c bridge_sim.cc ${CFLAGS}

config.o : config.cc config.h \
	bridge_sim.h
	g++ -c config.cc ${CFLAGS}

strategy.o : strategy.cc strategy.h \
	bridge_sim.h
	g++ -c strategy.cc ${CFLAGS}

test: test_main.o \
	bridge_sim.o \
	config.o \
	strategy.o \
	test_bridge_sim.o \
	test_config.o \
	test_strategy.o
	g++ -o test test_main.o \
				bridge_sim.o \
				config.o \
				strategy.o \
				test_bridge_sim.o \
				test_config.o \
				test_strategy.o

test_main.o : test_main.cc
	g++ -c test_main.cc ${CFLAGS}

test_config.o : test_config.cc \
	config.cc \
	config.h \
	bridge_sim.h
	g++ -c test_config.cc ${CFLAGS}

test_bridge_sim.o : test_bridge_sim.cc \
	bridge_sim.h
	g++ -c test_bridge_sim.cc ${CFLAGS}

test_strategy.o : test_strategy.cc \
	bridge_sim.h \
	strategy.h \
	strategy.cc
	g++ -c test_strategy.cc ${CFLAGS}

clean:
	rm -rf *.o sim test
