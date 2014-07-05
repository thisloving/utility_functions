#ifndef _Algorithm_h
#define _Algorithm_h

#include "zType.h"

bool binary_search(std::vector<int>& a, int key)
{
	int low	= 0;
	int high= SAFE_SUB(a.size(), 1);

	while (low <= high)
	{
		int mid = low + SAFE_SUB(high, low) / 2;
		int mid_value = a.at(mid);

		if (mid_value < key)
		{
			low = mid + 1;
		}
		else if (mid_value > key)
		{
			high = SAFE_SUB(high, 1);
		}
		else 
		{
			return true;
		}
	}

	return false;
}

#endif  //_Algorithm_h
