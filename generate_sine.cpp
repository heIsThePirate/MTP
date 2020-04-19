#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>
using namespace std;

#define PI 3.14
#define endl '\n'

int main()
{
    int N = 1000;                          //Max value of time step to generate data points upto
    string outputFile = "function_values"; //Declaring the output file name
    double dt = 1;                         //Time step

    ofstream sine_values;
    sine_values.open(outputFile); //Open the file in write mode
    if (!sine_values)
    {
        //This means there was an error in opening the file
        cout << "Can't open the file " << outputFile << " due to an internal error." << endl;
        exit(-1);
    }

    cout << "Opened the file " << outputFile << " for writing." << endl;

    int frequency_1 = 50, frequency_2 = 70; //frequency in Hz
    double phi = PI / 2;                    //phase difference

    //Set required precision of the output values
    sine_values << fixed << setprecision(5);
    cout << fixed << setprecision(5);

    for (double t = 0; t < N; t += dt)
    {
        //double value = sin(2*PI*frequency_2*t/N + phi);
        double value = sin(2 * PI * frequency_1 * t / N) + sin(2 * PI * frequency_2 * t / N + phi); //get the function value
        sine_values << value << endl;
    }

    cout << "Closing the file " << outputFile << " after writing the sine value." << endl;
    sine_values.close(); //Close the file to release the memory

    return 0;
}