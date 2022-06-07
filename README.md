# Bridge crossing problem

## Problem Description


## Some heuristics


* __Minimize the total time moving forward__
	
	When moving the light forward, use the two slowest people to cross the bridge. Because the speed is limited to the slower person, move with the next slowest person.  This is only used if we can use a faster person to move backwards.

* __Minimize the time moving backward__

	When moving the light backward, use the fastest person to cross the bridge.



## Strategies

We will try two strategies:

### __Local__
	
Move everyone across a bridge before moving to the next bridge. So we start at the first bridge and then process each bridge in turn until we reach the end.

Also, we can use the heuristics above.  If there is a faster person at the next node, then we (1) move the two slowest nodes to the next node and (2) use the faster node to return back.

__Example__

```
  (light)
A(speed:100)
B(speed:10)   ---bridge1---  D(speed:200)  ---bridge2---
C(speed:20)
```

Since D(speed 200) is faster than A,B, and C, we move the two slowest nodes first.

```
                               (light)
                             B(speed:10)
A(speed:100)  ---bridge1---  C(speed:20)   --bridge2---
                             D(speed:200)
```

In order to have A cross the bridge, we then move D back across, since it is the fastest.

```
  (light)
A(speed:100)  ---bridge1--- B(speed:10)  ---bridge2---
D(speed:200)                C(speed:20)
```

Finally, we can move A and D, thus completing the movement across bridge1.  We can now run the same process to cross bridge2

```
                  (light)
  ---bridge1--- A(speed:100)  ---bridge2---
                B(speed:10)
                C(speed:20)
                D(speed:200)
```

### __Global__ (not implemented)

This is very similar to the __Local__ strategy. The difference is that we attempt to grab the fastest person (wherever it may be).  So we would move the two slowest nodes forward until we reach the fastest node.  We then return with the fastest node backwards.

After that we can use the __Local__ strategy. Or we could try to obtain the next fastest node in a similar fashion.

## Reference:

### Building and running the code
```
$ cd src
$ make sim
g++ -c sim_main.cc -Wall -std=c++14 -D DEBUG -g
g++ -c bridge_sim.cc -Wall -std=c++14 -D DEBUG -g
g++ -c config.cc -Wall -std=c++14 -D DEBUG -g
g++ -c strategy.cc -Wall -std=c++14 -D DEBUG -g
g++ -o sim sim_main.o bridge_sim.o config.o strategy.o

$ ./sim ../test.yaml
Loading data from: ../test.yaml
Results:
  Total time: 291.5
  Bridge[0] : 19
  Bridge[1] : 169
  Bridge[2] : 291.5
```

### Building and running the tests
```
$ cd src
$ make test
g++ -c test_main.cc -Wall -std=c++14 -D DEBUG -g
g++ -c bridge_sim.cc -Wall -std=c++14 -D DEBUG -g
g++ -c config.cc -Wall -std=c++14 -D DEBUG -g
g++ -c strategy.cc -Wall -std=c++14 -D DEBUG -g
g++ -c test_bridge_sim.cc -Wall -std=c++14 -D DEBUG -g
g++ -c test_config.cc -Wall -std=c++14 -D DEBUG -g
g++ -c test_strategy.cc -Wall -std=c++14 -D DEBUG -g
g++ -o test test_main.o \
			bridge_sim.o \
			config.o \
			strategy.o \
			test_bridge_sim.o \
			test_config.o \
			test_strategy.o

$ ./test
====================================================================
All tests passed (115 assertions in 5 test cases)

```

### Input Example

```

bridges:
  -
    length: 100
    people:
      -
        name: A
        speed: 100
      -
        name: B
        speed: 50
      -
        name: C
        speed: 20
      -
        name: D
        speed: 10
  -
    length: 250
    people:
      -
        name: E
        speed: 2.5
  -
    length: 150
    people:
      -
        name: F
        speed: 25
      -
        name: G
        speed: 15

```

### Assumptions/Limitations

* Elapsed time is recorded as a float
* Integers are used indexes/identifiers, so this limits the number of people/bridges.
* The torch is started at the first bridge.


