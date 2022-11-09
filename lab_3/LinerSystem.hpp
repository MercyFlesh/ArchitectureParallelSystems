#include <vector>

class LinerSystem
{
public:
    template<typename T>
    static std::vector<T> sor(
        std::vector<std::vector<T>>& a, std::vector<T>& b, 
		double w, int iter_limit)
    {
		int n = a.size();
		std::vector<T> prev_x(n, 0), curr_x(n, 0);
        
		for (int it = 0; it < iter_limit; it++)
		{
			T s1,s2;
        	std::vector<std::vector<T>> c(n, std::vector<T>(n));

    		for(int i = 0; i < n; i++)
        	{
    			s1 = s2 = 0;

    			for (int j = 0; j < i; j++)
        	    {
    				c[i][j] = -a[i][j] * w / a[i][i];
    				s1 += c[i][j] * curr_x[j];
    			}

    			for (int j = i + 1; j < n; j++)
        	    {
    				c[i][j] = -a[i][j] * w / a[i][i];
    				s2 += c[i][j] * prev_x[j];
    			}

    			b[i] = b[i] * w / a[i][i];
    			curr_x[i] = s1 + s2 + b[i] - prev_x[i] * (w - 1);
    		}

			std::copy(curr_x.begin(), curr_x.end(), prev_x.begin());
		}

		return curr_x;
    }
};