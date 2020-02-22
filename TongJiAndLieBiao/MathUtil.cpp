#include "stdafx.h"
#include "MathUtil.h"


CMathUtil::CMathUtil()
{
}


CMathUtil::~CMathUtil()
{
}

double CMathUtil::PI() {
	return   3.14159265358979323846;
}

bool CMathUtil::IsEqual(double a, double b, double tol)
{
	return fabs(a - b) < tol;
}

int CMathUtil::Round(double a)
{
	return(int)(a + 0.5);
}

double CMathUtil::Round(double a, int precision)
{
	return (floor(a*pow(10.0, precision) + 0.5) / pow(10.0, precision));
}

int CMathUtil::GetRand(int minValue, int maxValue)
{
	assert(maxValue - minValue);

	int value = rand();
	int rc=minValue+(int)CMathUtil::Round(((double)value)/RAND_MAX*
		(maxValue - minValue));
	return rc;
}
