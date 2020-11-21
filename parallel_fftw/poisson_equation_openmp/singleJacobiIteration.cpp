#include <iostream>
#include <omp.h>

#include "singleJacobiIteration.h"

using namespace std;

void singleJacobiIteration(int Nx, int Ny, double dx, double dy, double **u, double **f, double **uNew, int iter_old, int iter_new)
{
    int i, j, iter;

#pragma omp parallel shared(Nx, Ny, dx, dy, u, f, uNew, iter_old, iter_new) private(i, j, iter)

    for (iter = iter_old + 1; iter <= iter_new; iter++)
    {
#pragma omp for
        for (int i = 0; i < Nx; i++)
            for (int j = 0; j < Ny; j++)
                u[i][j] = uNew[i][j];

#pragma omp for
        for (int i = 0; i < Nx; i++)
            for (int j = 0; j < Ny; j++)
                if (i == 0 || j == 0 || i == Nx - 1 || j == Ny - 1)
                    uNew[i][j] = f[i][j];
                else
                    uNew[i][j] = 0.25 * (u[i - 1][j] + u[i][j + 1] + u[i][j - 1] + u[i + 1][j] + f[i][j] * dx * dy);
    }

    return;
}