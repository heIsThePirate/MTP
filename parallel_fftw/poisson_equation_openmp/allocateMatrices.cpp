#include "allocateMatrices.h"

void allocateMatrices(int Nx, int Ny, double **u, double **f, double **uExact, double **uNew, double **uDiff) {
    u = new double *[Nx];
    f = new double *[Nx];
    uExact = new double *[Nx];
    uNew = new double *[Nx];
    uDiff = new double *[Nx];

    for (int i = 0; i < Nx; i++) {
        u[i] = new double[Ny];
        f[i] = new double[Ny];
        uExact[i] = new double[Ny];
        uNew[i] = new double[Ny];
        uDiff[i] = new double[Ny];
    }

    return;
};