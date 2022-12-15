#pragma once
#include <vector>
#include <algorithm>
#include <omp.h>

namespace LinerSystem {
    template<typename T>
    double GetDeterminantParallel(const std::vector<std::vector<T>>& a)
    {
        const double e = 1E-9;
        std::vector<std::vector<double>> triangleMatrix(a.size(), std::vector<double>(a.size()));
        
        #pragma omp parallel for
        for (std::size_t i = 0; i < a.size(); i++)
        {
            std::transform(a[i].begin(), a[i].end(), triangleMatrix[i].begin(), 
                            [](int x) { return static_cast<double>(x); });
        }
        
        double det = 1;
        for (std::size_t i = 0; i < triangleMatrix.size(); i++)
        {
            double pivot = triangleMatrix[i][i];
            int row = i;

            #pragma omp parallel for private(pivot, row) shared(i)
            for (std::size_t j = i + 1; j < triangleMatrix.size(); j++)
            {
                if (std::abs(triangleMatrix[j][i]) > std::abs(pivot))
                {
                    pivot = triangleMatrix[j][i];
                    row = j;
                }
            }

            if (std::abs(pivot) < e)
            {
                det = 0;
                break;
            }

            if (row != i)
            {
                triangleMatrix[i].swap(triangleMatrix[row]);
                det = -det;
            }

            det *= pivot;

            #pragma omp parallel for
            for (std::size_t j = i + 1; j < triangleMatrix.size(); j++) 
            {
                if (std::abs(triangleMatrix[j][i]) > e)
                {
                    for (std::size_t k = i + 1; k < triangleMatrix.size(); k++) 
                    {
                        triangleMatrix[j][k] -= triangleMatrix[j][i] * triangleMatrix[i][k] / 
                                                pivot;
                    }
                }
            }
        }

        return det;
    }

    template<typename T>
    std::vector<std::vector<T>> SwapColumnsParallel(std::vector<std::vector<T>> matrix, const std::vector<T>& vec, int column)
    {
        #pragma omp parallel for
        for (std::size_t i = 0; i < vec.size(); i++)
            matrix[i][column] = vec[i];
        
        return matrix;
    }

    template<typename T>
	std::vector<double> CramerMethodParallel(
		std::vector<std::vector<T>> a, std::vector<T> b)
    {
        double baseDet = GetDeterminantParallel(a);
        if (baseDet == 0)
            throw std::runtime_error("Determinant is 0, no solutions");
        
        std::vector<double> res(a.size());

        #pragma omp parallel for
        for (std::size_t i = 0; i < a.size(); i++)
        {
            res[i] = GetDeterminantParallel(SwapColumnsParallel(a, b, i)) / baseDet;
        }

        return res;
    }
}