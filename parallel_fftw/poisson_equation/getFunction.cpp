#include "getFunction.h"
#include "getUExact.h"
#include "uExactAtXY.h"

void getFunction(int Nx, int Ny, double** f) {
  double x, y, fNorm;
  int i, j;

  for (i = 0; i < Nx; i++) {
    x = (double) i / (double) (Nx - 1);
    for (j = 0; j < Ny; j++) {
      y = (double) j / ( double ) ( Ny - 1 );
      if (i == 0 || i == Nx - 1 || j == 0 || j == Ny - 1 )
        f[i][j] = getUExact(x, y);
      else
        f[i][j] = -uExactAtXY(x, y);
    }
  }

  return;
}