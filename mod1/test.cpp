#include <iostream>
#include <deque>

int main()
{
	std::deque<int>	num;
	int	n = 0;

	num.push_back(n);

	for (size_t	i = 0; i < 7; ++i)
	{
		std::deque<int>::iterator	itr = num.begin();
		size_t	num_size = num.size();

		
		for (size_t	j = 0; j < num_size; ++j)
		{
			if (j % 2 == 0)
			{
				num.push_back(n++);
				num.push_back(n++);
				num.push_back(n++);
				
				itr = num.erase(num.begin() + j);
				itr = num.erase(num.begin() + j);
			}
			else
			{
				++itr;
			}

		}	
	}
}