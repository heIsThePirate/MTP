#include <iostream>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <mpi.h>
#include <time.h>

#include "jacobi.h"
#include "allocateArrays.h"
#include "makeDomains.h"
#include "makeSource.h"
#include "timeStamp.h"

using namespace std;

/* macro to index into a 2-D (N+2)x(N+2) array */
#define INDEX(i, j) ((N + 2) * (i) + (j))

double L = 1.0; /* linear size of square region */
int N = 32;     /* number of interior points per dim */

double *u, *u_new; /* linear arrays to hold solution */

int my_rank; /* rank of this process */

int *proc;                   /* process indexed by vertex */
int *i_min, *i_max;          /* min, max vertex indices of processes */
int *left_proc, *right_proc; /* processes to left and right */

int main(int argc, char *argv[]) {
    double change, my_change, wall_time, epsilon = 1.0E-03;
    double *f, *swap;
    char file_name[100];
    int i, j, my_n, n, numOfProcs, step;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &numOfProcs);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (1 < argc)
        sscanf(argv[1], "%d", &N);
    else
        N = 32;

    if (2 < argc)
        sscanf(argv[2], "%lf", &epsilon);
    else
        epsilon = 1.0E-03;
    
    if (3 < argc)
        strcpy(file_name, argv[3]);
    else
        strcpy(file_name, "poisson_mpi.out");

    if (my_rank == 0) {
        timestamp();
        cout << "POISSON_MPI: 2-D Poisson equation using Jacobi Iteration" << endl;
        cout << "Number of processes = " << numOfProcs << endl;
        cout << "Number of interior vertices = " << N << endl;
        cout << "Desired fractional accuracy = " << epsilon << endl;
    }

    allocateArrays(N, u, u_new);
    f = makeSource(N);
    makeDomains(N, numOfProcs, proc, i_min, i_max, left_proc, right_proc);

    step = 0;
    wall_time = MPI_Wtime();

    do {
        jacobi(L, N, numOfProcs, f, left_proc, right_proc, i_min, i_max, my_rank, u, u_new);
        ++step;

        change = 0.0;
        n = 0;

        my_change = 0.0;
        my_n = 0;

        for (i = i_min[my_rank]; i <= i_max[my_rank]; i++) 
            for (j = 1; j <= N; j++) 
                if (u_new[INDEX(i, j)] != 0.0)
                {
                    my_change = my_change + fabs(1.0 - u[INDEX(i, j)] / u_new[INDEX(i, j)]);

                    my_n = my_n + 1;
                } 

        MPI_Allreduce(&my_change, &change, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

        MPI_Allreduce(&my_n, &n, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

        if (n != 0)
          change = change / n;

        if (my_rank == 0 && (step % 10) == 0)
          cout << "N = " << N << ", n = " << n << ", my_n = " << my_n << ", Step = " << step << ", Error = " << change << endl;
        swap = u;
        u = u_new;
        u_new = swap;
    } while (epsilon < change);

    wall_time = MPI_Wtime() - wall_time;
    if (my_rank == 0)
        cout << "Wall clock time = " << wall_time << " secs" << endl;

    MPI_Finalize();

    free(f);

    if (my_rank == 0) {
        cout << "POISSON_MPI: Normal end of execution." << endl;
        timestamp();
    }

    return 0;
}