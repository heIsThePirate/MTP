#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <omp.h>

#include "timeStamp.h"
#include "getFunction.h"
#include "getUExact.h"
#include "uExactAtXY.h"
#include "getMatrixRMS.h"
#include "singleJacobiIteration.h"

using namespace std;

#define endl '\n'
#define Nx 161 // Interior x-grid points
#define Ny 161 // Interior y-grid points

int main(int argc, char *argv[])
{
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int i, j, id, newIter = 0, oldIter;
  double x, y, dx, dy, error, diff, uNorm, uNewNorm, wtime, tolerance = 0.000001;
  double **u, **f, **uExact, **uNew, **uDiff;
  bool converged;
  fstream outputFile;
  string outputFilename = "poisson.txt";

  u = new double *[Nx];
  f = new double *[Nx];
  uExact = new double *[Nx];
  uNew = new double *[Nx];
  uDiff = new double *[Nx];

  for (int i = 0; i < Nx; i++)
  {
    u[i] = new double[Ny];
    f[i] = new double[Ny];
    uExact[i] = new double[Ny];
    uNew[i] = new double[Ny];
    uDiff[i] = new double[Ny];
  }

  dx = 1.0 / (double)(Nx - 1); // x-grid spacing
  dy = 1.0 / (double)(Ny - 1); // y-grid spacing

  timestamp();
  cout << endl;

  cout << "Solving the Poisson Equation { del^2 u = f(x,y) } using OPENMP" << endl;
  cout << "The chosen function:  f(x,y) = -(x^2 + y^2) * sin(x * y)" << endl;
  cout << "The chosen rectangle: 0 <= X <= 1, 0 <= Y <= 1" << endl;
  cout << "Number of processors: " << omp_get_num_procs() << endl;

  cout << endl;

#pragma omp parallel
  {
    id = omp_get_thread_num();
    if (id == 0)
      cout << "Maximum number of threads available: " << omp_get_num_threads() << endl;
      cout << endl;
  }

  getFunction(Nx, Ny, f);

  for (i = 0; i < Nx; i++)
    for (j = 0; j < Ny; j++)
      if (i == 0 || i == Nx - 1 || j == 0 || j == Ny - 1)
        uNew[i][j] = f[i][j];
      else
        uNew[i][j] = 0.0;

  uNewNorm = getMatrixRms(Nx, Ny, uNew);

  for (i = 0; i < Nx; i++)
  {
    x = (double)i / (double)(Nx - 1);
    for (j = 0; j < Ny; j++)
    {
      y = (double)j / (double)(Ny - 1);
      uExact[i][j] = getUExact(x, y);
    }
  }

  uNorm = getMatrixRms(Nx, Ny, uExact);
  cout << "RMS of the exact solution = " << uNorm << endl;

  cout << endl;

  // Iterate until convergence
  converged = false;

  outputFile.open(outputFilename.c_str(), ios::out);
  if (!outputFile)
  {
    cout << "Error in opening the output file!" << endl;
    return -1;
  }

  outputFile << "Step          uNewNorm          diff          error" << endl;

  for (i = 0; i < Nx; i++)
    for (j = 0; j < Ny; j++)
      uDiff[i][j] = uNew[i][j] - uExact[i][j];

  error = getMatrixRms(Nx, Ny, uDiff);

  wtime = omp_get_wtime();

  while (true)
  {
    oldIter = newIter;
    newIter = oldIter + 500;

    singleJacobiIteration(Nx, Ny, dx, dy, u, f, uNew, oldIter, newIter);

    uNorm = uNewNorm;
    uNewNorm = getMatrixRms(Nx, Ny, uNew);

    for (i = 0; i < Nx; i++)
      for (j = 0; j < Ny; j++)
        uDiff[i][j] = uNew[i][j] - u[i][j];

    diff = getMatrixRms(Nx, Ny, uDiff);

    for (i = 0; i < Nx; i++)
      for (j = 0; j < Ny; j++)
        uDiff[i][j] = uNew[i][j] - uExact[i][j];

    error = getMatrixRms(Nx, Ny, uDiff);

    outputFile << setw(4) << newIter << setw(18) << uNewNorm << setw(14) << diff << setw(15) << error << endl;

    if (diff <= tolerance)
    {
      converged = true;
      break;
    }
  }

  if (converged)
    cout << "Yayy... the iteration has converged! It took " << newIter << " steps." << endl;
  else
    cout << "Oops... the iteration has NOT converged!!" << endl;

  cout << endl;

  wtime = omp_get_wtime() - wtime;
  cout << "Time Elapsed = " << wtime << "s" << endl;
  cout << endl;

  outputFile.close();

  timestamp();

  return 0;
}

#undef Nx
#undef Ny