#include <iostream>
#include <mpi.h>

#include "jacobi.h"

/* macro to index into a 2-D (N+2)x(N+2) array */
#define INDEX(i, j) ((N + 2) * (i) + (j))

void jacobi(int L, int N, int numOfProcs, double* f, int* left_proc, int* right_proc, int* i_min, int* i_max, int my_rank, double* u, double* u_new) {
    double h;
    int i, j, requests;
    MPI_Request request[4];
    MPI_Status status[4];

    // h is the lattice spacing.
    h = L / (double)(N + 1);

    // Update ghost layers using non-blocking send/receive 
    requests = 0;

    if (left_proc[my_rank] >= 0 && left_proc[my_rank] < numOfProcs) {
        MPI_Irecv(u + INDEX(i_min[my_rank] - 1, 1), N, MPI_DOUBLE, left_proc[my_rank], 0, MPI_COMM_WORLD, request + requests++);

        MPI_Isend(u + INDEX(i_min[my_rank], 1), N, MPI_DOUBLE, left_proc[my_rank], 1, MPI_COMM_WORLD, request + requests++);
    }

    if (right_proc[my_rank] >= 0 && right_proc[my_rank] < numOfProcs) {
        MPI_Irecv(u + INDEX(i_max[my_rank] + 1, 1), N, MPI_DOUBLE, right_proc[my_rank], 1, MPI_COMM_WORLD, request + requests++);

        MPI_Isend(u + INDEX(i_max[my_rank], 1), N, MPI_DOUBLE, right_proc[my_rank], 0, MPI_COMM_WORLD, request + requests++);
    }

    // Jacobi update for internal vertices in my domain.
    for (i = i_min[my_rank] + 1; i <= i_max[my_rank] - 1; i++)
        for (j = 1; j <= N; j++)
            u_new[INDEX(i, j)] = 0.25 * (u[INDEX(i - 1, j)] + u[INDEX(i + 1, j)] + u[INDEX(i, j - 1)] + u[INDEX(i, j + 1)] + h * h * f[INDEX(i, j)]);

    // Wait for all non-blocking communications to complete.
    MPI_Waitall(requests, request, status);
     
    // Jacobi update for boundary vertices in my domain.
    i = i_min[my_rank];
    for (j = 1; j <= N; j++)
        u_new[INDEX(i, j)] = 0.25 * (u[INDEX(i - 1, j)] + u[INDEX(i + 1, j)] + u[INDEX(i, j - 1)] + u[INDEX(i, j + 1)] + h * h * f[INDEX(i, j)]);

    i = i_max[my_rank];
    if (i != i_min[my_rank])
        for (j = 1; j <= N; j++)
            u_new[INDEX(i, j)] = 0.25 * (u[INDEX(i - 1, j)] + u[INDEX(i + 1, j)] + u[INDEX(i, j - 1)] + u[INDEX(i, j + 1)] + h * h * f[INDEX(i, j)]);

    return;
}