#include <iostream>
#include <chrono>
#include <random>
#include <tuple>
#include <mpi.h>
#include "LinerSystem.hpp"
#include "LinerSystemParallel.hpp"

using namespace std;

static random_device rd;
static uniform_real_distribution<double> urd(-10, 10);
static vector<int> sizes {10, 100, 1000, 1500};

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
    {
        vector<vector<double>> a{{1, 4, -3}, {-2, 8, 5}, {3, 4, 7}};
        vector<vector<double>> l(a.size(), vector<double>(a.size()));
        vector<vector<double>> u(a.size(), vector<double>(a.size()));

        LinerSystem::LUDecomposition(a, l, u);
        cout << "LU-Decomposition matrix: " << endl;
        cout << "A: ";
        for (const auto& row : a)
        {
            for (const auto& num : row)
                cout << num << " ";
            cout << "\n   ";
        }

        cout << "\nL: ";
        for (const auto& row : l)
        {
            for (const auto& num : row)
                cout << num << " ";
            cout << "\n   ";
        }

        cout << "\nU: ";
        for (const auto& row : u)
        {
            for (const auto& num : row)
                cout << num << " ";
            cout << "\n   ";
        }
        
        cout << endl;
    }
    {
        vector<vector<double>> a{{3, 5}, {4, 8}};
        vector<double> b{11, 16};

        vector<double> x = LinerSystem::LUSolve(a, b);

        cout << "Solution of a system of equations: " << endl;
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

        cout << endl;
    }
}

void PerfomanceSequentialTest()
{
    cout << "Sequential algorithm performance" << endl;
    cout << "Matrix size\t" <<  "Time ms." << endl;

    for (size_t i = 0; i < sizes.size(); i++)
    {
        auto [a, b] = initializer_matrix(sizes[i]);
        auto begin = chrono::steady_clock::now();
        LinerSystem::LUSolve(a, b);
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
        LinerSystem::LUSolveParallel(a, b);
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