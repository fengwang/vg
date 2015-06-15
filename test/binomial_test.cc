#include "../vg.hpp"
#include <cmath>
#include <map>
#include <iostream>

using namespace std;

int main()
{
    auto v = vg::make_generator(vg::binomial)( 40, 0.5 );

    map<int, int> sample;

    for ( unsigned long int i = 0; i != 500; ++i )
        sample[static_cast<int>(std::round(10*v()))]++;

    for ( auto element : sample )
    {
        cout << "\n" << element.first << "\t";
        for ( auto j = 0; j < element.second; ++j ) cout << "*";
    }
    std::cout << "\n";

    return 0;
}
