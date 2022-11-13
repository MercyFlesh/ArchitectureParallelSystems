#pragma once
#include <iostream>
#include <vector>
#include <mpi.h>


namespace LinerSystem {
	template<typename T>
	std::vector<double> ParallelSor(
		std::vector<std::vector<T>>& a, std::vector<T>& b, 
		double w, int iter_limit)
	{
		double e = 1.0e-10;
		std::size_t n = a.size();
		std::vector<double> x(n, 0), res(n, 0);
		
		for (int it = 0; it < iter_limit; it++)
		{
			double err = 0;

			for (std::size_t i = 0; i < n; i++)
			{
				double procSum = 0;
				for (std::size_t j = 0; j < n; j++)
				{
					procSum -= a[i][j] * x[j];
				}

				double totalSum;
				MPI_Reduce(&procSum, &totalSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
				x[i] += w * (totalSum + b[i]) / a[i][i];
			}

			for (std::size_t i = 0; i < n; i++)
			{
				double procSum = 0;
				for (std::size_t j = 0; j < n; j++)
				{
					procSum += a[i][j] * x[j];
				}
				
				double totalSum;
				MPI_Reduce(&procSum, &totalSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
				res[i] = b[i] - totalSum;
				if (std::abs(res[i - 1]) > err) 
				{
 					err = std::abs(res[i - 1]);
 				}
			}

			//MPI_Bcast(&err, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
			if (err <= e)
 				break;
		}

		return x;
	}
}