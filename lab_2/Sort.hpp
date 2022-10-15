#include <algorithm>
#include <iterator>
#include <omp.h>

class Sort
{
public:
    template<typename RandomIt>
    static void QuickSort(RandomIt begin, RandomIt end)
    {
        int size = std::distance(begin, end);
        int i = 0;
	    int j = size - 1;
        typename RandomIt::value_type midd;
        midd = begin[size / 2];

	    while (i <= j)
	    {
	    	while (begin[i] < midd)
	    		i++;
	    	while (begin[j] > midd)
	    		j--;

	    	if (i <= j)
	    	{
                std::iter_swap(begin + i, begin + j);
	    		i++;
	    		j--;
	    	}
	    }

	    if (j > 0)
	    	QuickSort(begin, begin + j + 1);

	    if (size > i)
	    	QuickSort(begin + i, end);
    }

    template<typename RandomIt>
    static void QuickSortParallel(RandomIt begin, RandomIt end)
    {
        int size = std::distance(begin, end);
        int i = 0;
	    int j = size - 1;
        typename RandomIt::value_type midd;
        midd = begin[size / 2];

	    while(i <= j)
	    {
            #pragma omp parallel sections num_threads(2)
            {
                #pragma omp section
                {
                    while (begin[i] < midd)
	    		        i++;
                }                
                #pragma section
                {
                    while (begin[j] > midd)
	    		        j--;
                }   
            }
	    	
	    	if (i <= j)
	    	{
                std::iter_swap(begin + i, begin + j);
	    		i++;
	    		j--;
	    	}
	    }

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                if (j > 0)
	        	    QuickSort(begin, begin + j + 1);
            }
            #pragma omp section
            {   if (size > i)
	        	    QuickSort(begin + i, end);
            }
        }   
    }
};