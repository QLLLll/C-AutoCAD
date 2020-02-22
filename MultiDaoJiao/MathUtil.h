#pragma once
class CMathUtil
{
public:
	CMathUtil();
	~CMathUtil();
	//const double PI = std::atan(1.0)*4;
	static double CMathUtil::PI();
	static bool IsEqual(double a, double b, double tol = 1.0E-7);
	static int Round(double a);
	static double Round(double a, int precision);
	static int GetRand(int minValue, int maxValue);
};

