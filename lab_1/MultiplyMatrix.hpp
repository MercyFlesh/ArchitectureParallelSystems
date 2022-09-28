#include <vector>

class MultiplyMatrix
{
public:
    template<typename T>
    static std::vector<std::vector<T>> SequentialMultiply(
        std::vector<std::vector<T>>& matrixA, 
        std::vector<std::vector<T>>& matrixB)
    {
    	if (matrixA[0].size() != matrixB.size())
    		throw std::invalid_argument("Number of columns of matrix A must be equal \
    								to the number of rows of matrix B.");

    	std::vector<std::vector<int>> resultMatrix(matrixA.size(), std::vector<int>(matrixB[0].size(), 0));
    	for (std::size_t i = 0; i < matrixA.size(); i++)
    	{
    		for (std::size_t j = 0; j < matrixB[0].size(); j++)
    		{
    			for (std::size_t k = 0; k < matrixA[0].size(); k++)
    			{
    				resultMatrix[i][j] += matrixA[i][k] * matrixB[k][j];
    			}
    		}
    	}

    	return resultMatrix;
    }
};