#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <chrono>
#include "Sort.hpp"

using namespace std;

static random_device rd;
static uniform_int_distribution<int> uid(0, 10000000);


void testPerfomance()
{
    vector<int> vecSizes{100, 1000, 10000, 100000, 500000};
    vector<int> threadNums{2, 4, 8, 16, 32};

    clog << "Vector size" << "\t" <<
            "Threads" << "\t\t" <<
            "Time" << "\n" << endl;

    for (auto& size : vecSizes)
    {
        vector<int> vec(size);
        for (auto& n : vec)
            n = uid(rd);

        auto begin = chrono::steady_clock::now();
        Sort::QuickSort(vec.begin(), vec.end());
        auto timeMs = chrono::duration_cast<chrono::microseconds>(
            chrono::steady_clock::now() - begin
        ).count();

        clog << vec.size() << "\t\t" <<
                1 << "\t\t" <<
                timeMs << "\n" << endl;

        for (auto& num : threadNums)
        {
            omp_set_num_threads(num);
            auto beginParallel = chrono::steady_clock::now();
            Sort::QuickSortParallel(vec.begin(), vec.end());
            auto timeMsParallel = chrono::duration_cast<chrono::microseconds>(
                chrono::steady_clock::now() - beginParallel
            ).count();

            clog << vec.size() << "\t\t" <<
                num << "\t\t" <<
                timeMsParallel << "\n" << endl;
        }
    }
}

int main()
{
    //testPerfomance();
    int vecSize;
    cout << "Enter count of numbers: ";
    cin >> vecSize;

    vector<int> vec(vecSize);
    for (auto& n : vec)
        n = uid(rd);

    fstream file("lab2.txt", ios::app);
    file << "Unsorted array:" << endl;
    copy(vec.begin(), vec.end(), ostream_iterator<int>(file, " "));
    file << endl;

    Sort::QuickSortParallel(vec.begin(), vec.end());

    file << "\nSorted array:" << endl;
    copy(vec.begin(), vec.end(), ostream_iterator<int>(file, " "));
    file << "\n________________________________________\n" << endl;
    file.close();

    for (auto& n : vec)
    {
        cout << n << " ";
    }
}