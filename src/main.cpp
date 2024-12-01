#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>

struct sRequest
{
    int sector;
    int arrival;
};

std::vector<sRequest> theRequests;

void readfile( const std::string& fname )
{
    // The input will be of the following format with the first number representing the sector to be serviced
    // and the second representing the arrival time (sorted by arrival time asc):

}
main()
{
    readfile( "../dat/test1.txt");

    return 0;
}
