#pragma once
#include <vector>
#include <mpi.h>

namespace LinerSystem {
    void LUDecompositionParallel(
        const std::vector<std::vector<double>>& a, 
        std::vector<std::vector<double>>& l,
        std::vector<std::vector<double>>& u)
    {
        for(std::size_t i = 0; i < a.size(); i++)
        {
            for(std::size_t j = i; j < a.size(); j++)
            {
                u[i][j] = a[i][j];
                for(std::size_t k = 0; k < i; k++)
                    u[i][j] -= l[i][k] * u[k][j];
            }

            MPI_Bcast(&u[i], u.size() - i, MPI_DOUBLE, 0, MPI_COMM_WORLD);

            for(std::size_t j = i; j < a.size(); j++)
            {
                if (j == i)
                {
                    l[i][i] = 1;
                }
                else
                {
                    l[j][i] = a[j][i] / u[i][i];
                    for(std::size_t k = 0; k < i; k++)
                        l[j][i] -= (l[j][k] * u[k][i]) / u[i][i];
                }
            }

            MPI_Bcast(&l[i], l.size() - i, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        }
    }

    std::vector<double> LUSolveParallel(
        const std::vector<std::vector<double>>& a,
        const std::vector<double>& b)
    {
        int procesSize, procesRank;
        MPI_Comm_size(MPI_COMM_WORLD, &procesSize);
        MPI_Comm_rank(MPI_COMM_WORLD, &procesRank);

        std::vector<std::vector<double>> l(a.size(), std::vector<double>(a.size()));
        std::vector<std::vector<double>> u(a.size(), std::vector<double>(a.size()));
        
        LUDecompositionParallel(a, l, u);

        std::vector<double> y(a.size(), 0);
        std::vector<double> x(a.size(), 0);

        for (std::size_t i = 0; i < a.size(); i++)
        {
            y[i] = b[i] / l[i][i];
            for (std::size_t j = 0; j < i; j++)
                y[i] -= (l[i][j] * y[j]) / l[i][i];
        }

        MPI_Bcast(&y, y.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);

        for (int i = a.size() - 1; i >= 0; i--)
        {
            x[i] = y[i] / u[i][i];
            for (int j = a.size() - 1; j > i; j--)
                x[i] -= (u[i][j] * x[j]) / u[i][i];
        }

        MPI_Bcast(&x, x.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);

        return x;
    }
}