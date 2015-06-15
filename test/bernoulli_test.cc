#include "../vg.hpp"
#include <cmath>
#include <map>
#include <iostream>

using namespace std;

int main()
{
    auto v = vg::make_generator(vg::bernoulli, vg::lagged_fibonacci)( 0.4 );

    map< int, int > sample;

    for ( unsigned long int i = 0; i != 50; ++i )
        sample[v()]++;

    for ( auto i = sample.begin(); i != sample.end(); ++i )
    {
        cout << "\n" << (*i).first << "\t";
        for ( auto j = 0; j < (*i).second; ++j ) cout << "*";
    }
    std::cout << "\n";

    return 0;
}
