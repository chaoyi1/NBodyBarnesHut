# NBodyBarnesHut

N-body simulation made using the Barnes-Hut algorithm.
Barnes-Hut algorithm: Divide 3D space into octants recursively and represent it as an Octree data structure. Insert the bodies into Octree such that each octant contains only 1 or 0 body. Instead of calculating net acceleration with between a body and all others pair-wise, if the bodies are sufficiently far enough, they can all be approximated as a single body. This reduces the time complexity of net force/acceleration calculation from O(N<sup>2</sup>) to O(NlogN) where N is the number of bodies in the simulation.

References:

- [Wikipedia article on Barnes-Hut algorithm](https://en.wikipedia.org/wiki/Barnes%E2%80%93Hut_simulation)
- [Princeton CS126 assignment](https://www.cs.princeton.edu/courses/archive/fall03/cs126/assignments/barnes-hut.html)
- [2D Galaxy Simulator](https://beltoforion.de/en/barnes-hut-galaxy-simulator/)

The following is a plot of N=1000 bodies over 50000 time steps:
![1k bodies 50k steps](/img/NBBH1k50ksteps.png?raw=true "Figure 1")

The simulation itself is written in C++.  
The visualisation is written in Python 3.8.

To compile and run the simulation, type the following commands in bash:
g++ \*.cpp -o output
./output

To run the visualisation (visualise.py), run the following command in bash:
python visualise.py

Plan for improvements (in no particular order):

1. Optimise for memory.
2. Optimise classes/introduce more classes.
3. Animation in C++.
4. Parallelisation.
