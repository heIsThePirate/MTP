#include <cmath>
#include "uExactAtXY.h"

using namespace std;

double uExactAtXY(double x, double y) {
    double exactValue;

    exactValue = -(x * x + y * y) * sin(x * y);

    return exactValue;
}