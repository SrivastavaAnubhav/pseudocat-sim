# pseudocat-sim
A better write-up about pseudocatenation and the research problem can be found [here](https://colab.research.google.com/drive/104Z2AcU6fmMrgQoABmBmCV2JFrCP1Mp9#scrollTo=iCI0OLusNGN2).

### Files in this repository
* automata_sqr: the simulator for the pseudocatenation automaton that we are using
* automata_cube: the simulator for the pseudocube automaton that we are using
* indistinguishable_states: output for the N=4 case of the pseudocube automaton

### Running the simulator
Just compile the simulator you need with your desired C++ compiler and run it. O2 is recommended since the runtime is exponential (because of subset construction). If you want to try bigger N, you must edit the C++ file (this is done because we are using N at compile time to set the size of the bitsets which the program uses to simulate the automaton under the hood).

```
$ g++ -std=c++14 -O2 automata_cube.cc -o automaton
$ ./automaton
Number of reachable states: 29365
The following states are indistinguishable:
0010    0011    1001    1111    1111
0010    1001    0011    1111    1111
0100    0101    0011    1011    1111
0100    0011    0101    1011    1111
1000    0001    0011    1010    1111
0001    1010    1000    0011    1111
1000    0011    0001    1010    1111
0100    0001    0111    1010    1111
                    .
                    .
                    .
Number of reachable and indistinguishable states: 22050
```
