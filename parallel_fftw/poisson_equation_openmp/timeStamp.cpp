#include <iostream>
#include <ctime>

#include "timeStamp.h"

using namespace std;

#define endl '\n'

void timestamp()
{
#define TIME_SIZE 40

    static char time_buffer[TIME_SIZE];
    const struct tm *tm_ptr;
    time_t now;

    now = time(NULL);
    tm_ptr = localtime(&now);

    strftime(time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm_ptr);

    cout << time_buffer << endl;

    return;
#undef TIME_SIZE
}