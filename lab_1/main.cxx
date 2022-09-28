#include <iostream>
#include "MultiplyMatrix.hpp"

using namespace std;

int main(int argc, char **argv)
{
    vector<vector<int>> a{ {4}, {5}, {6} };
	vector<vector<int>> b{ {1, 2, 3} };
	for (auto& row : MultiplyMatrix::SequentialMultiply(a, b))
	{
		for (auto& num : row)
			cout << num << " ";
		cout << endl;
	}
}