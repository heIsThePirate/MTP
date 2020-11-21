#include <iostream>
#include <algorithm>

#include "allocateArrays.h"

void allocateArrays(int N, double* u, double* u_new) {
    int i, ndof;

    ndof = (N + 2) * (N + 2);

    u = (double *)malloc(ndof * sizeof(double));
    for (i = 0; i < ndof; i++)
        u[i] = 0.0;

    u_new = (double *)malloc(ndof * sizeof(double));
    for (i = 0; i < ndof; i++)
        u_new[i] = 0.0;

    return;
}