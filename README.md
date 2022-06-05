# bridgesim
Bridge crossing problem

Problem Description

Some possible heuristics
(1) If moving forward, move the two slowest people
Minimize the time moving forward.

(2) If moving backward, move the fastest person
Minimize the time moving backward.

Possible strategies
(1) local
Move everyone forward one node at a time, following the
heuristics above.

(2) global
Move to the location of the fastest person, then move
the fastest node backward. Then do this for the next
fastest person. Do this until the current node is empty.

Reference:
Building the solution
Running the solution
Running the tests
Problem Description
Input Description
Soure Code Organization


