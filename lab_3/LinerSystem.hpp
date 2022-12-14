#pragma once
#include <vector>

namespace LinerSystem {
    template<typename T>
	static std::vector<double> Sor(
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
				double sum = 0;
				for (std::size_t j = 0; j < n; j++)
				{
					sum -= a[i][j] * x[j];
				}

				sum = w * (sum + b[i]) / a[i][i];
				x[i] += sum;
			}

			for (std::size_t i = 0; i < n; i++)
			{
				double sum = 0;
				for (std::size_t j = 0; j < n; j++)
				{
					sum += a[i][j] * x[j];
				}

				res[i] = b[i] - sum;
				if (std::abs(res[i - 1]) > err) 
				{
 					err = std::abs(res[i - 1]);
 				}
			}

			if (err <= e)
 				break;
		}

		return x;
	}
}