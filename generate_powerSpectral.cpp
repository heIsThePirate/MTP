#include <iostream>
#include <cmath>
#include <fftw3.h>
#include <iomanip>
#include <fstream>
using namespace std;

#define N 1000 //Max value of time step to generate data points upto
#define endl '\n'

int main(void)
{
    fftw_complex in[N], out[N]; //declare the input and output complex arrays
    fftw_plan p;
    int i;

    string inputFile = "function_values"; //declare the input file

    ifstream sine_values;
    sine_values.open(inputFile); //open the file containing the function values

    if (sine_values)
        cout << "Opened the input file successfully." << endl;
    else
    {
        //This means there was an error in opening the file
        cout << "Can't open the file " << inputFile << " due to an internal error." << endl;
        exit(-1);
    }

    i = 0;
    while (sine_values && (i++) <= N)
    {
        double p;
        sine_values >> p;
        if (i == 1) //Skipping the repeated initial value
            continue;
        in[i - 1][0] = p; //Assign the REAL part of input
        in[i - 1][1] = 0; //Assign the IMAGINARY part of input
    }

    string outputFile = "dft"; //declare the output file
    ofstream dft;
    dft.open(outputFile); //open the file to print the output into

    //Forward Fourier transform, save the result in 'out'
    cout << "Calculating the forward fourier transform." << endl;
    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);

    if (dft)
        cout << "Opened the output file successfully." << endl;
    else
    {
        //This means there was an error in opening the file
        cout << "Can't open the file " << outputFile << " due to an internal error." << endl;
        exit(-1);
    }

    //Set required precision of the output values
    dft << fixed << setprecision(5);
    cout << fixed << setprecision(5);

    //Save the result in the outputFile
    for (i = 0; i < N; i++)
    {
        //Calculate the power spectral density which is the squared modulus of the 'out[i]'
        double a = out[i][0];
        double b = out[i][1];
        double c = (a * a + b * b); //Modulus
        double psd = c * c;         //Power Spectral density
        dft << i << " " << in[i][0] << " " << out[i][0] << " " << out[i][1] << " " << psd << endl;
    }

    //Close and clean all the allocated memories
    fftw_destroy_plan(p);
    sine_values.close();
    cout << "Closing the file " << outputFile << " after writing the dft values." << endl;
    dft.close();

    fftw_cleanup();
    return 0;
}