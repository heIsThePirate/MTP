#include <cmath>
#include "getMatrixRMS.h"

using namespace std;

double getMatrixRms(int Nx, int Ny, double** mat)
{
  double rms = 0.0;

  for(int i = 0; i < Nx; i++)
    for(int j = 0; j < Ny; j++)
      rms += mat[i][j] * mat[i][j];
  
  rms = sqrt(rms / (double) (Nx * Ny));

  return rms;
}