#include <iostream>
#include <cmath>
#include <fftw3.h>
#include <iomanip>
#include <fstream>
using namespace std;

#define N_x 100 //Window Size in x-direction
#define N_y 100 //Window Size in y-direction
#define endl '\n'

int main(void)
{
    fftw_complex in[N_x * N_y], out[N_x * N_y]; //declare the input and output complex arrays

    //(Input and Output are 1-D arrays, the index to insert will be determined using the ROW-MAJOR format)

    fftw_plan p;
    int i, j;

    string inputFile = "function_values_2D"; //declare the input file

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

    for (i = 0; i < N_x; i++)
    {
        for (j = 0; j < N_y; j++)
        {
            double p;
            sine_values >> p;
            in[i * N_y + j][0] = p; //Assign the REAL part of input
            in[i * N_y + j][1] = 0; //Assign the IMAGINARY part of input
        }
    }

    string outputFile = "dft_2D"; //declare the output file
    ofstream dft;
    dft.open(outputFile); //open the file to print the output into

    //Forward Fourier transform, save the result in 'out'
    cout << "Calculating the forward fourier transform." << endl;
    p = fftw_plan_dft_2d(N_x, N_y, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
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
    for (i = 0; i < N_x; i++)
    {
        for (j = 0; j < N_y; j++)
        {
            //Calculate the power spectral density which is the squared modulus of the 'out[i]'
            double a = out[i * N_y + j][0];
            double b = out[i * N_y + j][1];
            double c = (a * a + b * b); //Modulus
            double psd = c * c;         //Power Spectral density
            dft << i * N_y + j << " " << in[i * N_y + j][0] << " " << out[i * N_y + j][0] << " " << out[i * N_y + j][1] << " " << psd << endl;
        }
    }

    //Close and clean all the allocated memories
    fftw_destroy_plan(p);
    sine_values.close();
    cout << "Closing the file " << outputFile << " after writing the dft values." << endl;
    dft.close();

    fftw_cleanup();
    return 0;
}