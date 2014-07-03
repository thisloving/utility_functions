#ifndef _zMath_h_
#define _zMath_h_

#include <iostream>
#include <string>
#include <sstream>

namespace zMath
{
	static int RandBetween(int min, int max)
	{
		if (min == max)
		{
			return min;
		}
		else if (min > max)
		{
			return max + (int)(((double)min - (double)max + 1.0) * rand_r(time(NULL)) / (RAND_MAX + 1.0));
		}
		else
		{
			return max + (int)(((double)max - (double)min + 1.0) * rand_r(time(NULL)) / (RAND_MAX + 1.0));
		}
	}

	static bool SelectByOdds(const unsigned int up, const unsigned int down)
	{
		unsigned int m_rand;
		if (down < 1)	return false;
		if (up < 1)		return false;
		if (up > down - 1)	return false;

		m_rand = 1 + (unsigned int)((double)down * rand_r(time(NULL)) / (RAND_MAX + 1.0))

		return m_rand < up;
	}

	static bool SelectByPercent(const unsigned int percent)
	{
		return SelectByOdds(percent, 100);
	}
}

#endif  // _zMath_h_
