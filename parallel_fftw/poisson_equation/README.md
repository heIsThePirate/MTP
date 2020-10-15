## Solving Poisson Equation using Jacobi Iteration and OpenMP

Poisson Equation: **del^2 u = f(x,y)**

The chosen function:  **f(x,y) = -(x^2 + y^2) * sin(x * y)**

The chosen rectangle: **0 <= X <= 1, 0 <= Y <= 1**

Number of processors used for testing: **4**

### How to run the program
On any GNU compiler, open the command line (in the corresponding folder) and run the following commands:
```shell
$ g++ -fopenmp *.cpp -o main

$ ./main
```