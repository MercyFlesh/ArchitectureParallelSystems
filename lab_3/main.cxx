#include <iostream>
#include <vector>
#include <random>
#include <tuple>
#include <chrono>
#include <cassert>
#include <mpi.h>
#include "LinerSystem.hpp"
#include "LinerSystemParallel.hpp"

using namespace std;

static random_device rd;
static uniform_real_distribution<double> urd(-10, 10);
static const double w = 1.2;
static const int limit_iterations = 1500;
static vector<int> sizes {10, 100, 1000, 2000};


tuple<vector<vector<double>>, vector<double>> initializer_matrix(int size)
{
    vector<vector<double>> a(size, vector<double>(size));
    vector<double> b(size);
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
            a[i][j] = urd(rd);
        b[i] = urd(rd);
    }

    return {move(a), move(b)};
}

void ValidTest()
{
    vector<vector<int>> a{{3, 5}, {4, 8}};
    vector<int> b{11, 16};

    vector<double> x = LinerSystem::Sor(a, b, w, limit_iterations);

    for (size_t i = 0; i < a.size(); i++)
    {
        for (size_t j = 0; j < a[0].size(); j++)
        {
            cout << "(x" << j + 1 << " * " << a[i][j] << ") ";
            if (j != a[0].size() - 1)
                cout << "+ ";
        }

        cout << "= " << b[i] << endl;
    }

    for (size_t i = 0; i < x.size(); i++)
        cout << "x" << i + 1 << " = " << x[i] << endl;
}

void PerfomanceSequentialTest()
{
    cout << "Sequential algorithm performance" << endl;
    cout << "Matrix size\t" <<  "Time ms." << endl;

    for (size_t i = 0; i < sizes.size(); i++)
    {
        auto [a, b] = initializer_matrix(sizes[i]);
        auto begin = chrono::steady_clock::now();
        LinerSystem::Sor(a, b, w, limit_iterations);
        auto time = chrono::duration_cast<chrono::microseconds>(
                chrono::steady_clock::now() - begin).count();
        
        cout << sizes[i] << "\t\t" << time << endl;
    }
}

void PerfomanceParallelTest()
{
    int procesSize, procesRank;
    MPI_Comm_size(MPI_COMM_WORLD, &procesSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &procesRank);

    if (procesRank == 0)
    {
        cout << "Parallel algorithm performance with " << procesSize << " processes" << endl;
        cout << "Matrix size\t" <<  "Time ms." << endl;
    }

    for (size_t i = 0; i < sizes.size(); i++)
    {
        auto [a, b] = initializer_matrix(sizes[i]);
        auto begin = chrono::steady_clock::now();
        LinerSystem::ParallelSor(a, b, w, limit_iterations);
        auto time = chrono::duration_cast<chrono::microseconds>(
                chrono::steady_clock::now() - begin).count();

        if (procesRank == 0)
        {
            cout << sizes[i] << "\t\t" << time << endl;
        }
    }
}


int main(int argc, char* argv[])
{
    //ValidTest();
    //PerfomanceSequentialTest();

    MPI_Init(&argc, &argv);
    
    PerfomanceParallelTest();

    MPI_Finalize();
}