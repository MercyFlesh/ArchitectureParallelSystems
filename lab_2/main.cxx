#include <iostream>
#include <vector>
#include "Sort.hpp"

using namespace std;


int main()
{
    vector<int> vec{4, 1, 7, 5, 3, 6};
    Sort::QuickSort(vec.begin(), vec.end());

    for (auto& n : vec)
    {
        cout << n << " ";
    }
}