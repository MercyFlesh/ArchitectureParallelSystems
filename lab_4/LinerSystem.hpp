#pragma once
#include <vector>
#include <algorithm>

namespace LinerSystem {
    template<typename T>
    double GetDeterminant(const std::vector<std::vector<T>>& a)
    {
        std::vector<std::vector<double>> triangleMatrix(a.size(), std::vector<double>(a.size()));
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
            for (std::size_t j = i + 1; j < triangleMatrix.size(); j++)
            {
                if (std::abs(triangleMatrix[j][i]) > std::abs(pivot))
                {
                    pivot = triangleMatrix[j][i];
                    row = j;
                }
            }

            if (pivot == 0)
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
            for (std::size_t j = i + 1; j < triangleMatrix.size(); j++) 
            {
                for (std::size_t k = i + 1; k < triangleMatrix.size(); k++) 
                {
                    triangleMatrix[j][k] -= triangleMatrix[j][i] * triangleMatrix[i][k] / 
                                            pivot;
                }
            }
        }

        return det;
    }

    template<typename T>
    void SwapColumns(std::vector<std::vector<T>>& matrix, std::vector<T>& vec, int column)
    {
        for (std::size_t i = 0; i < vec.size(); i++)
            std::swap(matrix[i][column], vec[i]);
    }

    template<typename T>
	std::vector<double> CramerMethod(
		std::vector<std::vector<T>> a, std::vector<T> b)
    {
        double baseDet = GetDeterminant(a);
        if (baseDet == 0)
            throw std::runtime_error("Determinant is 0, no solutions");
        
        std::vector<double> res(a.size());
        for (std::size_t i = 0; i < a.size(); i++)
        {
            SwapColumns(a, b, i);
            res[i] = GetDeterminant(a) / baseDet;
            SwapColumns(a, b, i);
        }

        return res;
    }
}