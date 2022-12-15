#include <iostream>
#include <tuple>
#include <cassert>
#include <random>
#include <chrono>
#include "LinerSystem.hpp"
#include "LinerSystemParallel.hpp"

using namespace std;

static random_device rd;
static uniform_real_distribution<double> urd(-10, 10);

tuple<vector<vector<double>>, vector<double>> InitializeMatrix(int size)
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
    
    double det = LinerSystem::GetDeterminant(a);
    vector<double> x = LinerSystem::CramerMethod(a, b);

    assert(det == 4);
    assert(x[0] == 2);
    assert(x[1] == 1);

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
    
    cout << "Determinant = " << det << endl;
    for (size_t i = 0; i < x.size(); i++)
        cout << "x" << i + 1 << " = " << x[i] << endl;
    cout << endl;
}

void PerfomanceTest()
{
    vector<int> threadNums{ 2, 4, 8, 16, 32 };
    vector<int> matrixSizes{ 10, 50, 100, 200, 300 };
    int procesSize, procesRank;

    cout << "System size" << "\t" <<
            "Threads" << "\t\t" <<
            "Time ms." << "\n" << endl;

    for (size_t i = 0; i < matrixSizes.size(); i++)
    {
        auto [a, b] = InitializeMatrix(matrixSizes[i]);

        auto begin = chrono::steady_clock::now();
        try 
        {
            LinerSystem::CramerMethod(a, b);
        } 
        catch(const exception& ex)
        {}
        
        auto time = chrono::duration_cast<chrono::microseconds>(
            chrono::steady_clock::now() - begin
        ).count();

        cout << matrixSizes[i] << "\t\t" <<
                1 << "\t\t" <<
                time << "\n" << endl;

        for (const auto& th : threadNums)
        {
            omp_set_num_threads(th);
            begin = chrono::steady_clock::now();
            try 
            {
                LinerSystem::CramerMethodParallel(a, b);
            } 
            catch(const exception& ex)
            {}

            time = chrono::duration_cast<chrono::microseconds>(
                    chrono::steady_clock::now() - begin).count();
            
            cout << matrixSizes[i] << "\t\t" <<
                th << "\t\t" <<
                time << "\n" << endl;
        }

        cout << endl;
    }
}

int main()
{
    ValidTest();
    PerfomanceTest();
}