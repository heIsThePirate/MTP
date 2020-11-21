#include <iostream>
#include <algorithm>

#include "makeSource.h"

#define sqr(x) ((x) * (x))

/* macro to index into a 2-D (N+2)x(N+2) array */
#define INDEX(i, j) ((N + 2) * (i) + (j))

double *makeSource(int N) {
    double *in;
    int i, j, k;
    double q;

    in = (double *) malloc(sqr(N + 2) * sizeof(double));

    for (i = 0; i < sqr(N + 2); i++)
        in[i] = 0.0;
    
//   Make a dipole.
    q = 10.0;

    i = 1 + N / 4;
    j = i;
    k = INDEX(i, j);
    in[k] = q;

    i = 1 + 3 * N / 4;
    j = i;
    k = INDEX(i, j);
    in[k] = -q;

    return in;
}