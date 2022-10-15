#include <iostream>
#include <cstring>
#include <vector>
#include <random>
#include <chrono>
#include <omp.h>

using namespace std;


template<typename T>
double ScalarMultiplyVector(vector<T>& a, vector<T>& b)
{
    double result = 0;
    for (size_t i = 0; i < a.size(); i++)
        result += a[i] * b[i];

    return result;
}

template<typename T>
double ScalarMultiplyVectorParallel(vector<T>& a, vector<T>& b, int countThreads)
{
    double result = 0;
    
    omp_set_num_threads(countThreads);
    #pragma omp parallel for reduction(+:result)
    for (size_t i = 0; i < a.size(); i++)
        result += a[i] * b[i];

    return result;
}

void CheckMultiplyVector(vector<int>& a, vector<int>& b, int countThreads)
{
    //not parallel
    auto begin = chrono::steady_clock::now();
    ScalarMultiplyVector(a, b);
    auto timeMs = chrono::duration_cast<chrono::microseconds>(
        chrono::steady_clock::now() - begin
    ).count();
    
    //parallel
    auto beginParallel = chrono::steady_clock::now();
    ScalarMultiplyVectorParallel(a, b, countThreads);
    auto timeMsParallel = chrono::duration_cast<chrono::microseconds>(
        chrono::steady_clock::now() - beginParallel
    ).count();

    clog << "Vector size: " << a.size() << "\n" <<
        "ScalarMultiplyVector time (ms): " << timeMs << "\n" <<
         "ScalarMultiplyVectorParallel time (ms): " << timeMsParallel << "\n" << endl;
}

int main(int argc, char* argv[])
{
    if (argc != 3 || strcmp(argv[1], "-t") != 0)
    {
        cerr << "Incorrect arguments\n" <<
        "Use the -t option to specify the number of threads" << endl;
        return -1;
    }
    
    vector<int> exampleA{1, 2, 3};
    vector<int> exampleB{4, 5, 6};
    cout << "Synchronized scalar multiply: {1, 2, 3} * {4, 5, 6} = " << ScalarMultiplyVector(exampleA, exampleB) << endl;
    cout << "Parallel scalar multiply: {1, 2, 3} * {4, 5, 6} = " << ScalarMultiplyVectorParallel(exampleA, exampleB, atoi(argv[2])) << "\n" << endl;

    random_device rd;
    mt19937 gen(rd());

    vector<int> sizes{10, 20, 50, 200, 500, 1000, 10000, 100000, 400000};
    vector<vector<int>> a(sizes.size());
    vector<vector<int>> b(sizes.size());

    for (size_t i = 0; i < sizes.size(); i++)
    {
        a[i] = vector<int>(sizes[i]);
        b[i] = vector<int>(sizes[i]);
        uniform_int_distribution<int> distrib(0, sizes[i] * 5);
        for (size_t j = 0; j < sizes[i]; j++)
        {
            a[i][j] = distrib(gen);
            b[i][j] = distrib(gen);
        }

        CheckMultiplyVector(a[i], b[i], atoi(argv[2]));
    }

    return 0;
}