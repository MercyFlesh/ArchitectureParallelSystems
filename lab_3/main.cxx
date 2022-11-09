#include <iostream>
#include "LinerSystem.hpp"

using namespace std;

int main()
{
    vector<vector<double>> matrix {{3, 5}, {4, 8}};
    vector<double> b{11, 16};

    double w = 1.5;
    int steps = 1000;

    vector<double> x = LinerSystem::sor(matrix, b, w, steps);
    for(auto& n : x)
        cout << n << " ";
    
    cout << endl;
}