#include <bits/stdc++.h>
#include <omp.h>

#include "timeStamp.h"

using namespace std;

#define endl '\n'
#define sqr(x) ((x) * (x))
#define numOfIter 5000
#define numOfVar 50000

int main()
{
    double *x, *xNew, *b;
    double d, r, t, wtime;
    int i, iter;
    fstream outputFile;
    string outputFilename = "jacobi.txt";

    b = new double[numOfVar];
    x = new double[numOfVar];
    xNew = new double[numOfVar];

    timestamp();
    cout << endl;

    cout << "Using OpenMP to run Jacobi iteration for getting the solution of the equation A*x=b" << endl;
    cout << "Number of variables, N = " << numOfVar << endl;
    cout << "Number of iterations, M = " << numOfIter << endl;

    outputFile.open(outputFilename.c_str(), ios::out);
    if (!outputFile)
    {
        cout << "Error in opening the output file!" << endl;
        return -1;
    }

    outputFile << "i               x" << endl;

    wtime = omp_get_wtime();

#pragma omp parallel private(i)
    {
#pragma omp for
        for (i = 0; i < numOfVar; i++)
            b[i] = 0.0;

        b[numOfVar - 1] = (double)(numOfVar + 1);

// Initializing the solution to 0
#pragma omp for
        for (i = 0; i < numOfVar; i++)
            x[i] = 0.0;
    }

    // Doing the iteration
    for (iter = 0; iter < numOfIter; iter++)
    {
#pragma omp parallel private(i, t)
        {
#pragma omp for
            for (i = 0; i < numOfVar; i++)
            {
                xNew[i] = b[i];
                if (i > 0)
                    xNew[i] += x[i - 1];
                if (i < numOfVar - 1)
                    xNew[i] += x[i + 1];

                xNew[i] /= 2.0;
            }

            d = 0.0;
#pragma omp for reduction(+ \
                          : d)
            for (i = 0; i < numOfVar; i++)
                d += sqr(x[i] - xNew[i]);

// Updating the solution
#pragma omp for
            for (i = 0; i < numOfVar; i++)
                x[i] = xNew[i];

            r = 0.0;
#pragma omp for reduction(+ \
                          : r)
            for (i = 0; i < numOfVar; i++)
            {
                t = b[i] - 2.0 * x[i];
                if (i > 0)
                    t += x[i - 1];
                if (i < numOfVar - 1)
                    t += x[i + 1];

                r += sqr(t);
            }
        }
    }

    for (i = 0; i < numOfVar; i++)
        outputFile << setw(1) << i << setw(16) << x[i] << endl;

    delete[] b;
    delete[] x;
    delete[] xNew;

    cout << endl;
    wtime = omp_get_wtime() - wtime;

    cout << "Time elapsed: " << wtime << endl;

    cout << endl;
    timestamp();

    return 0;
}

#undef sqr
#undef numOfIter
#undef numOfVar