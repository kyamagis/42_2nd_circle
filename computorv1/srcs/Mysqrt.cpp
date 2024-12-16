// https://qiita.com/PlanetMeron/items/09d7eb204868e1a49f49
// https://ja.wikipedia.org/wiki/%E3%83%8B%E3%83%A5%E3%83%BC%E3%83%88%E3%83%B3%E6%B3%95
// https://www.isc.meiji.ac.jp/~syazaki/1107-11Shimane/Newton-method.pdf
// https://www.akita-pu.ac.jp/system/elect/ins/kusakari/japanese/teaching/Old/Programming/2009/text/6p.pdf

// https://github.com/pfsense/FreeBSD-src/blob/devel-main/lib/msun/src/e_sqrt.c
// 

// ニュートン法

#include "../includes/Mysqrt.hpp"

double	SqrtPureNewtonMethod(double num, double accuracy)
{
	double	xn = num;
	double	xn1;

	if (num == 0.0)
	{
		return 0.0;
	}

	if (accuracy < 0.0)
	{
		accuracy = 0.0;
	}
	else if (MAX_ACCURACY < accuracy)
	{
		accuracy = MAX_ACCURACY;
	}

	if (xn < 1.0)
	{
		xn = 1.0;
	}

	while (true)
	{
		xn1 = (xn + num / xn) / 2.0;
		if (accuracy < xn1 / xn)
		{
			return xn1;
		}
		xn = xn1;
	}
	return num;
}

/* #include <iostream>
int	main(void)
{
	double num = 0.0000000004;
	std::cout << SqrtPureNewtonMethod(num, MAX_ACCURACY) <<  std::endl;
} */
