#include <iostream>
#include <algorithm>

#include "makeDomains.h"

/*
    This function sets up the information defining the process domains.
*/
void makeDomains(int N, int numOfProcs, int* proc, int* i_min, int* i_max, int* left_proc, int* right_proc) {
    double d, eps, x_max, x_min;
    int i, p;

    // Allocate arrays for process information.
    proc = (int *)malloc((N + 2) * sizeof(int));
    i_min = (int *)malloc(numOfProcs * sizeof(int));
    i_max = (int *)malloc(numOfProcs * sizeof(int));
    left_proc = (int *)malloc(numOfProcs * sizeof(int));
    right_proc = (int *)malloc(numOfProcs * sizeof(int));


    // Divide the range [(1-eps)..(N+eps)] evenly among the processes.
    eps = 0.0001;
    d = (N - 1.0 + 2.0 * eps) / (double)numOfProcs;

    for (p = 0; p < numOfProcs; p++) {
        // The I indices assigned to domain P will satisfy X_MIN <= I <= X_MAX.
        x_min = -eps + 1.0 + (double)(p * d);
        x_max = x_min + d;
        
        // For the node with index I, store in PROC[I] the process P it belongs to.
        for (i = 1; i <= N; i++)
            if (x_min <= i && i < x_max)
                proc[i] = p;
    }

    // Now find the lowest index I associated with each process P.
    for (p = 0; p < numOfProcs; p++) {
        for (i = 1; i <= N; i++)
            if (proc[i] == p)
                break;
        i_min[p] = i;

        // Find the largest index associated with each process P.
        for (i = N; 1 <= i; i--)
            if (proc[i] == p)
                break;
        i_max[p] = i;
    
        // Find the processes to left and right. 
        left_proc[p] = -1;
        right_proc[p] = -1;

        if (proc[p] != -1)
            if (1 < i_min[p] && i_min[p] <= N)
                left_proc[p] = proc[i_min[p] - 1];
            if (0 < i_max[p] && i_max[p] < N)
                right_proc[p] = proc[i_max[p] + 1];
    }

    return;
}