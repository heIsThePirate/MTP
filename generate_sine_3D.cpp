#include<iostream>
#include<cmath>
#include<fstream>
#include<iomanip>
using namespace std;

#define PI 3.14
#define endl '\n'

int main() {
    int N_x = 50; //Max value of time step to generate data points upto in x-direction
    int N_y = 50; //Max value of time step to generate data points upto in y-direction
    int N_z = 50; //Max value of time step to generate data points upto in z-direction
    string outputFile = "function_values_3D"; //Declaring the output file name
    double dt = 1; //Time step
    
    ofstream sine_values;
    sine_values.open(outputFile); //Open the file in write mode
    if(!sine_values){
        //This means there was an error in opening the file
        cout << "Can't open the file " << outputFile << " due to an internal error." << endl;
        exit(-1);
    }

    cout << "Opened the file " << outputFile << " for writing." << endl;

    int frequency_x = 50, frequency_y = 70, frequency_z = 50; //x, y and z frequency in Hz
    double phi = PI/2; //phase difference

    //Set required precision of the output values
    sine_values << fixed << setprecision(5);
    cout << fixed << setprecision(5);

    for(double t=0; t<N_x; t+=dt) {
        for(double i=0; i<N_y; i+=dt) {
            for(double j=0; j<N_z; j+=dt) {
                double value = sin(2*PI*frequency_x*t/N_x) * sin(2*PI*frequency_y*i/N_y + phi) * sin(2*PI*frequency_z*j/N_z + phi/2); //get the function value
                sine_values << value << endl;
            }
        }
    }

    cout << "Closing the file " << outputFile << " after writing the sine value." << endl;
    sine_values.close(); //Close the file to release the memory

    return 0;
}